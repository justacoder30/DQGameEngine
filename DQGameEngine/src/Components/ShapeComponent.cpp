#include "pch.h"
#include "ShapeComponent.h"
#include "Components/GameApp.h"

void ShapeComponent::OnAttach()
{
	GameApp::GetBoardphase()->Add(this);
	if (m_Parent) {
		auto callbacks = dynamic_cast<CollisionCallbacks*>(m_Parent);
		if (callbacks) {
			callback = callbacks;
		}
	}
}

void ShapeComponent::OnDetach()
{
	GameApp::GetBoardphase()->Remove(this);
}

void ShapeComponent::OnUpdate(float dt)
{
	isColliding = false;
}
