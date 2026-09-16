#pragma once

#include <unordered_map>
#include <typeindex>
#include <vector>
#include "Core/Memory.h"

namespace dqengine {

class Component
{
public:
    Component() = default;
    virtual ~Component();

    // Preferred ownership API. The returned pointer is a non-owning observer.
    template<class T> T* Add(UniquePtr<T> component) {
        auto* observer = component.get();
        AddOwned(std::move(component));
        return observer;
    }
    template<class T> T* QueueAdd(UniquePtr<T> component) {
        auto* observer = component.get();
        QueueOwned(std::move(component));
        return observer;
    }

    // Legacy overloads adopt ownership; never pass stack or already-owned objects.
    void Add(Component *component);
    // Takes ownership immediately; attaches after traversal and updates next frame.
    void QueueAdd(Component* component);
    bool IsRemovalPending() const { return m_IsRemoved; }
	void Load();    
    void Update(float dt);
    void Draw();

    void RemoveFromParent();
    void ClearChildren();

    template<typename T>
    inline T* GetComponent()
    {
        auto it = m_ComponentMap.find(std::type_index(typeid(T)));
        if (it != m_ComponentMap.end())
            return static_cast<T*>(it->second);

        return nullptr;
    }

	Component* GetParent() const { return m_Parent; }
    const std::vector<UniquePtr<Component>>& GetChildren() const { return m_Children; }

	bool DebugMode = false;

protected:

    virtual void OnDetach() {}
    virtual void OnLoad() {}
    virtual void OnUpdate(float dt) {}
    virtual void OnDraw() {}
    virtual void OnAttach() {}

    Component* m_Parent = nullptr;

private:
    void AddOwned(UniquePtr<Component> component);
    void QueueOwned(UniquePtr<Component> component);
    bool m_IsRemoved = false;
    bool m_IsLoaded = false;
    bool m_TraversingChildren = false;
    void FlushQueuedChildren();
    void UnindexChild(Component* child);
    std::vector<UniquePtr<Component>> m_QueuedChildren;
    std::vector<UniquePtr<Component>> m_Children;
    std::unordered_map<std::type_index, Component*> m_ComponentMap;
};



} // namespace dqengine
