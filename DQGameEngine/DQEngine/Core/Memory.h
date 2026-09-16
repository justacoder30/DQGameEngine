#pragma once
#include <memory>
#include <utility>

namespace dqengine {

template<class T, class Deleter = std::default_delete<T>>
using UniquePtr = std::unique_ptr<T, Deleter>;
template<class T> using SharedPtr = std::shared_ptr<T>;
template<class T> using WeakPtr = std::weak_ptr<T>;

template<class T, class... Args>
[[nodiscard]] UniquePtr<T> Unique(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}
template<class T, class... Args>
[[nodiscard]] SharedPtr<T> Shared(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

}
