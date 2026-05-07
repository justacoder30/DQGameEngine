#pragma once

#include <unordered_map>
#include <typeindex>

class Component
{
public:
    Component() = default;
    virtual ~Component() = default;

    void Add(Component *component);
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

	Component* GetParent() { return m_Parent; }
    const std::vector<Component*>& GetChildren() const { return m_Children; }

protected:

    virtual void OnDetach() {}
    virtual void OnLoad() {}
    virtual void OnUpdate(float dt) {}
    virtual void OnDraw() {}
    virtual void OnAttach() {}

    Component* m_Parent = nullptr;

private:
    bool m_IsRemoved = false;
    bool m_IsLoaded = false;
    std::vector<Component*> m_Children;
    std::unordered_map<std::type_index, Component*> m_ComponentMap;
};


