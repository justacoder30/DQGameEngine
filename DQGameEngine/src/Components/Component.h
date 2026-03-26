#pragma once

#include <vector>
#include <memory>

class Component
{
public:
    Component() = default;
    virtual ~Component() = default;

    void Add(Component *component);
    void Update(float dt);
    void Draw();
	Component* GetParent() { return m_Parent; }

protected:

    virtual void OnUpdate(float dt) {}
    virtual void OnDraw() {}
    virtual void OnAttach() {}

    Component* m_Parent = nullptr;

private:
	
    std::vector<Component*> m_Children;
};