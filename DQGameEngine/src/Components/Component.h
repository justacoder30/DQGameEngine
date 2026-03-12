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

protected:

    virtual void OnUpdate(float dt) {}
    virtual void OnDraw() {}

private:

    std::vector<Component*> m_Children;
};