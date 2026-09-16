#include "pch.h"
#include "Component.h"

namespace dqengine {

Component::~Component() { ClearChildren(); }

void Component::Add(Component* component)
{
    if (!component || component == this || component->m_Parent) return;
    AddOwned(UniquePtr<Component>(component));
}

void Component::AddOwned(UniquePtr<Component> owned)
{
    if (!owned) return;
    if (m_TraversingChildren) {
        QueueOwned(std::move(owned));
        return;
    }
    auto* component = owned.get();
    m_Children.push_back(std::move(owned));
    component->m_Parent = this;
    m_ComponentMap[std::type_index(typeid(*component))] = component;
    component->OnAttach();
    if (m_IsLoaded) component->Load();
}

void Component::QueueAdd(Component* component)
{
    if (!component || component == this || component->m_Parent) return;
    QueueOwned(UniquePtr<Component>(component));
}

void Component::QueueOwned(UniquePtr<Component> owned)
{
    if (!owned) return;
    auto* component = owned.get();
    m_QueuedChildren.push_back(std::move(owned));
    component->m_Parent = this;
}

void Component::FlushQueuedChildren()
{
    if (m_QueuedChildren.empty()) return;
    // A snapshot prevents OnAttach/OnLoad spawns from extending this batch.
    auto queued = std::move(m_QueuedChildren);
    m_QueuedChildren.clear();
    m_TraversingChildren = true;
    for (auto& child : queued) {
        if (m_IsRemoved || child->m_IsRemoved) {
            child->ClearChildren();
            child->m_Parent = nullptr;
            continue;
        }
        auto* component = child.get();
        m_Children.push_back(std::move(child));
        m_ComponentMap[std::type_index(typeid(*component))] = component;
        component->OnAttach();
        if (m_IsLoaded) component->Load();
    }
    m_TraversingChildren = false;
}

void Component::Load()
{
    if (m_IsLoaded) return;
    OnLoad();
    m_TraversingChildren = true;
    for (auto& owned : m_Children)
        if (!owned->m_IsRemoved) owned->Load();
    m_IsLoaded = true;
    m_TraversingChildren = false;
    FlushQueuedChildren();
}

void Component::UnindexChild(Component* child)
{
    const auto type = std::type_index(typeid(*child));
    const auto entry = m_ComponentMap.find(type);
    if (entry == m_ComponentMap.end() || entry->second != child) return;
    m_ComponentMap.erase(entry);
    // Several heads (or enemies) may have the same type.
    for (auto it = m_Children.rbegin(); it != m_Children.rend(); ++it) {
        if (it->get() != child && !(*it)->m_IsRemoved && std::type_index(typeid(**it)) == type) {
            m_ComponentMap[type] = it->get();
            break;
        }
    }
}

void Component::Update(float dt)
{
    m_TraversingChildren = true;
    if (!m_IsRemoved) OnUpdate(dt);

    for (auto it = m_Children.begin(); it != m_Children.end();) {
        auto* child = it->get();

        if (!m_IsRemoved && !child->m_IsRemoved) 
            child->Update(dt);

        if (child->m_IsRemoved) {

            child->ClearChildren();
            child->OnDetach();
            child->m_Parent = nullptr;
            UnindexChild(child);
            it = m_Children.erase(it);

        } else ++it;

    }
    m_TraversingChildren = false;
    FlushQueuedChildren();
}

void Component::Draw()
{
    if (m_IsRemoved) return;
    m_TraversingChildren = true;
    OnDraw();
    for (auto& owned : m_Children)
        if (!owned->m_IsRemoved) owned->Draw();
    m_TraversingChildren = false;
}

void Component::RemoveFromParent()
{
    m_IsRemoved = true;
}

void Component::ClearChildren()
{
    m_TraversingChildren = true;
    for (auto& owned : m_Children) {
        auto* child = owned.get();
        child->ClearChildren();
        child->OnDetach();
        child->m_Parent = nullptr;
    }
    m_Children.clear();
    m_ComponentMap.clear();
    // Queued children have not received OnAttach, so do not call OnDetach.
    auto queued = std::move(m_QueuedChildren);
    m_QueuedChildren.clear();
    for (auto& child : queued) {
        child->ClearChildren();
        child->m_Parent = nullptr;
    }
    m_TraversingChildren = false;
}



} // namespace dqengine
