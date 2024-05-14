//
// Created by dxz on 2024/5/13.
//

#ifndef FARRAGO_SINGLETON_MANAGER_H
#define FARRAGO_SINGLETON_MANAGER_H

#include <cstddef>
#include <optional>

namespace farrago::singleton_mgr {

template<typename ...Types>
struct SingletonManager {
    static constexpr std::size_t size = sizeof...(Types);
    static constexpr bool noexcept_init = (std::is_nothrow_constructible_v<Types> && ...);
    static constexpr bool noexcept_destroy = (std::is_nothrow_destructible_v<Types> && ...);

    SingletonManager() noexcept(noexcept_init) {
        (SingletonManager::Storage<Types>().emplace(), ...);
    }

    ~SingletonManager() noexcept(noexcept_destroy) {
        char dummy{};
        ((SingletonManager::Storage<Types>().reset(), dummy) = ...);
    }

    template<typename Obj, std::enable_if_t<(std::is_same_v<Obj, Types> || ...)> * = nullptr>
    static Obj &Get() {
        return SingletonManager::Storage<Obj>().value();
    }

    template<typename Obj>
    static std::optional<Obj>& Storage() {
        static std::optional<Obj> data;
        return data;
    }

    static const SingletonManager &Local() {
        static SingletonManager mgr;
        return mgr;
    }

    static auto Instance() {
        const auto &dummy = self_;
        return [](){};
    }

    static const SingletonManager &self_;
};

#if defined(__GNUC__) || defined(__GNUG__) // GCC or Clang
    template<typename ...Types>
#   ifdef __clang__
    __attribute__((used))
#   elif defined(__GNUC__)
    __attribute__((used)) __attribute__((visibility("default")))
#   endif
    const SingletonManager<Types...> &SingletonManager<Types...>::self_ = SingletonManager::Local();
#endif



}

#endif //FARRAGO_SINGLETON_MANAGER_H
