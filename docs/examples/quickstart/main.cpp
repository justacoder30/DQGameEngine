#include <DQEngine/DQEngine.h>
#include <algorithm>

class MovingSquare : public dqengine::SpriteComponent {
public:
    MovingSquare() {
        position = dqengine::Vector(100.f, 200.f);
        size = dqengine::Vector(48.f, 48.f);
        SetColor(dqengine::Color(0.2f, 0.6f, 1.f, 1.f));
    }

protected:
    void OnUpdate(float dt) override {
        const bool left = dqengine::Key[SDL_SCANCODE_A] || dqengine::Key[SDL_SCANCODE_LEFT];
        const bool right = dqengine::Key[SDL_SCANCODE_D] || dqengine::Key[SDL_SCANCODE_RIGHT];
        const float direction = static_cast<float>(right) - static_cast<float>(left);
        position.x = std::clamp(position.x + direction * 220.f * dt, 0.f, 800.f - size.x);

        // Refresh the sprite's drawing rectangle after moving it.
        dqengine::SpriteComponent::OnUpdate(dt);
    }
};

class QuickstartGame : public dqengine::GameApp {
public:
    QuickstartGame() : GameApp(800, 450, "DQGameEngine - A/D or arrows to move") {}

protected:
    void OnLoad() override {
        Add(dqengine::Unique<MovingSquare>());
        auto* camera = Add(dqengine::Unique<dqengine::CameraComponent>(800.f, 450.f));
        camera->SetOrigin(dqengine::Vector(0.f, 0.f));
    }
};

int main() {
    QuickstartGame game;
    game.Run();
    return 0;
}
