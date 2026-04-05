#pragma once

#include <vector>
#include <memory>

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

	Component* GetParent() { return m_Parent; }
	std::vector<Component*> GetChildren() { return m_Children; }

protected:

    virtual void OnDetach() {}
    virtual void OnLoad() {}
    virtual void OnUpdate(float dt) {}
    virtual void OnDraw() {}
    virtual void OnAttach() {}

    Component* m_Parent = nullptr;

private:
    bool m_IsRemoved = false;
    std::vector<Component*> m_Children;
};