#include "pch.h"
#include "Component.h"

void Component::Add(Component* component)
{
	component->m_Parent = this; 
    m_Children.push_back(component);
	component->OnAttach();
    m_ComponentMap[std::type_index(typeid(*component))] = component;

    if (m_IsLoaded)
    {
        component->Load();
    }
}

void Component::Load()
{
    if (m_IsLoaded)
        return;

	OnLoad();

    for (auto& child : m_Children)
    {
        child->Load();
    }
    
    m_IsLoaded = true;
}

void Component::Update(float dt)
{
    OnUpdate(dt);

    for (auto it = m_Children.begin(); it != m_Children.end(); )
    {
        Component* child = *it;

        child->Update(dt);
        //++it;
        if (child->m_IsRemoved)
        {
            child->ClearChildren();
            child->OnDetach();
            child->m_Parent = nullptr;
            m_ComponentMap.erase(std::type_index(typeid(*child)));

            delete child;                
            it = m_Children.erase(it); 
        }
        else
        {
            ++it;
        }
    }
}

void Component::Draw()
{
    OnDraw();

    for (auto& child : m_Children)
    {
        child->Draw();
    }
}

void Component::RemoveFromParent()
{
    m_IsRemoved = true;
}

void Component::ClearChildren()
{
    for (auto child : m_Children)
    {
        m_ComponentMap.erase(std::type_index(typeid(*child)));
        child->OnDetach();     // 👈 thêm
        child->m_Parent = nullptr;
        delete child;
    }
    m_Children.clear();
}


