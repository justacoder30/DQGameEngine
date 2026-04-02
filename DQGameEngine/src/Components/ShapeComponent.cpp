#include "pch.h"
#include "ShapeComponent.h"
#include "Components/GameApp.h"

void ShapeComponent::OnAttach()
{
	GameApp::GetCollisionSystem()->Add(this);
}

void ShapeComponent::OnDetach()
{
	GameApp::GetCollisionSystem()->Remove(this);
}
