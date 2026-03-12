#include "pch.h"
#include "Component.h"

void Component::Add(Component* component)
{
    m_Children.push_back(component);
}

void Component::Update(float dt)
{
    OnUpdate(dt);

    for (auto& child : m_Children)
    {
        child->Update(dt);
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