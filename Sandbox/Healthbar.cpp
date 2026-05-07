#include "Healthbar.h"
#include <iostream>

Healthbar::Healthbar(const Vector& position, const Vector& size)
{
	bar = new SpriteComponent();
	bar->position = position;
	bar->size = size;
	m_MaxWidth = size.x;
	bar->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });	

	bar->m_Visible = true;
}

void Healthbar::SetHealth(int current, int max)
{
	float percent = (float)current / (float)max;

	if (percent < 0.0f) percent = 0.0f;

	bar->size.x = m_MaxWidth * percent;

	Show();
}

void Healthbar::Show()
{
	bar->m_Visible = true;
	m_Timer = m_ShowDuration;
}

void Healthbar::OnLoad()
{
	Add(bar);
	bar->layer = layer;
}

void Healthbar::OnUpdate(float dt)
{
	if (!bar->m_Visible)
		return;

	m_Timer -= dt;

	if (m_Timer <= 0.0f)
	{
		bar->m_Visible = false;
	}

	
	PositionComponent::OnUpdate(dt);
}
