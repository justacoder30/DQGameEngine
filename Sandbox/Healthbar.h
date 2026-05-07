#pragma once
#include "DQEngine.h"
class Healthbar : public PositionComponent
{
public :
	Healthbar(const Vector& position, const Vector& size);
	void SetHealth(int current, int max);
	void SetColor(const Color& color) { bar->SetColor(color); }
	void Show();

	SpriteComponent* bar;
protected:
	void OnLoad() override;
	void OnUpdate(float dt) override;

private:
	float m_Timer = 0.0f;
	float m_ShowDuration = 5.0f;
	float m_MaxWidth = 0.0f;
};

