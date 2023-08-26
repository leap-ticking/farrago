//
// Created by dxz on 2023/8/26.
//

#ifndef FARRAGO_FARRAGO_H
#define FARRAGO_FARRAGO_H

#include <type_traits>
#include <tuple>

namespace farrago {

template<typename T, std::size_t N>
using At = T;

struct FarragoNull{};

template <typename Creator, typename... Args>
struct AnyRefType {
    template <typename T>
    operator T() {
        return creator_->template Create<T>();
    }

    template <typename T, typename = std::enable_if_t<(std::is_same<T, Args>::value || ...)>>
    operator const T&() {
        return creator_->template GetDependency<T>();
    }

    Creator* creator_ = nullptr;
};

template <typename Creator, typename Src, typename... Args>
struct AnyFirstRefType {
    template <typename T, typename = std::enable_if_t<!std::is_same_v<Src, T>>>
    constexpr operator T() {
        return creator_->template Create<T>();
    }

    template <typename T, typename = std::enable_if_t<(std::is_same<T, Args>::value || ...)>>
    constexpr operator const T&() {
        return creator_->template GetDependency<T>();
    }

    Creator* creator_ = nullptr;
};


template<typename... Args>
class ObjectCreator {
public:
    template<typename... Ts>
    explicit ObjectCreator(Ts&&... args) : dependency_(std::forward<Ts>(args)...) {}

    template<typename T>
    const T& GetDependency() {
        return std::get<const T&>(dependency_);
    }

    template<typename T>
    T Create() {
        if constexpr ((std::is_same<T, Args>::value || ...)) {
            return std::get<const T&>(dependency_);
        }
        else if constexpr (std::is_default_constructible_v<T>) {
            return T{};
        }
        else if constexpr (std::is_constructible_v<T, AnyFirstRefType<ObjectCreator, T, FarragoNull, Args...>>) {
            return T{AnyFirstRefType<ObjectCreator, T, FarragoNull, Args...>{this}};
        }
        else {
            return CreateMoreParamObject<T>(std::make_index_sequence<10>{});
        }
    }

private:
    template<typename T, std::size_t... Ns>
    T CreateMoreParamObject(const std::index_sequence<Ns...>&) {
        if constexpr (std::is_constructible_v<T, At<AnyRefType<ObjectCreator, FarragoNull, Args...>, Ns>...>) {
            return T{At<AnyRefType<ObjectCreator, FarragoNull, Args...>, Ns>{this}...};
        }
        else {
            return CreateMoreParamObject<T>(std::make_index_sequence<sizeof...(Ns) - 1>{});
        }
    }


private:
    std::tuple<const Args& ...> dependency_;
};




}

#endif //FARRAGO_FARRAGO_H
