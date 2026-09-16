#pragma once
#include "Component.h"
#include "Core/IState.h"
#include "Core/StateMachine.h"

namespace dqengine {

class StateMachineComponent : public Component
{
public:
    template<class T, class... Args> T* CreateState(Args&&... args) {
        auto owned = Unique<T>(std::forward<Args>(args)...);
        auto* observer = owned.get();
        states.push_back(std::move(owned));
        return observer;
    }
    void ChangeState(IState* state);
protected:
    void OnUpdate(float dt) override;
private:
    std::vector<UniquePtr<IState>> states;
    StateMachine fsm;
    
};


} // namespace dqengine
