//
// Created by dxz on 2023/8/26.
//

#ifndef FARRAGO_OBJ_CREATOR_H
#define FARRAGO_OBJ_CREATOR_H

#include <tuple>
#include <type_traits>

namespace farrago::obj_creator {

template <typename T, std::size_t N> using At = T;

struct FarragoNull {};

template <typename Creator, typename... Args> struct AnyRefType {
  template <typename T,
            typename = std::enable_if_t<!(std::is_same<T, Args>::value || ...)>>
  operator T() {
    return creator_->template Create<T>();
  }

  template <typename T,
            typename = std::enable_if_t<(std::is_same<T, Args>::value || ...)>>
  operator const T &() {
    return creator_->template GetDependency<T>();
  }

  template <typename T, typename = std::enable_if_t<(
                            std::is_same<std::decay_t<T>, Args>::value || ...)>>
  constexpr operator T &() {
    return const_cast<T &>(creator_->template GetDependency<T>());
  }

  template <typename T, typename = std::enable_if_t<(
                            std::is_same<std::decay_t<T>, Args>::value || ...)>>
  constexpr operator T &&() {
    return static_cast<T &&>(
        const_cast<T &>(creator_->template GetDependency<T>()));
  }

  Creator *creator_ = nullptr;
};

template <typename Creator, typename Src, typename... Args>
struct AnyFirstType {
  template <typename T, typename = std::enable_if_t<!std::is_same_v<Src, T>>>
  constexpr operator T() {
    return creator_->template Create<T>();
  }

  template <typename T,
            typename = std::enable_if_t<(std::is_same<T, Args>::value || ...)>>
  constexpr operator const T &() {
    return creator_->template GetDependency<T>();
  }

  Creator *creator_ = nullptr;
};

template <typename Creator, typename Src, typename... Args>
struct AnyFirstRefType {
  template <typename T,
            typename = std::enable_if_t<!std::is_same_v<Src, std::decay_t<T>>>,
            typename = std::enable_if_t<
                (std::is_same<std::decay_t<T>, Args>::value || ...)>>
  constexpr operator T &() {
    return const_cast<T &>(creator_->template GetDependency<T>());
  }

  template <typename T,
            typename = std::enable_if_t<!std::is_same_v<Src, std::decay_t<T>>>,
            typename = std::enable_if_t<
                (std::is_same<std::decay_t<T>, Args>::value || ...)>>
  constexpr operator T &&() {
    return static_cast<T &&>(
        const_cast<T &>(creator_->template GetDependency<T>()));
  }

  Creator *creator_ = nullptr;
};

template <typename... Args> class ObjectCreator {
public:
  template <typename... Ts>
  explicit ObjectCreator(Ts &&...args)
      : dependency_(std::forward<Ts>(args)...) {}

  template <typename T> const T &GetDependency() {
    return std::get<const T &>(dependency_);
  }

  template <typename T> T Create() {
    if constexpr ((std::is_same<T, Args>::value || ...)) {
      return std::get<const T &>(dependency_);
    } else if constexpr (std::is_default_constructible_v<T>) {
      return T{};
    } else if constexpr (std::is_constructible<
                             T, AnyFirstRefType<ObjectCreator, T, FarragoNull,
                                                Args...>>::value) {
      return T{AnyFirstRefType<ObjectCreator, T, FarragoNull, Args...>{this}};
    } else if constexpr (std::is_constructible<
                             T, AnyFirstType<ObjectCreator, T, FarragoNull,
                                             Args...>>::value) {
      return T{AnyFirstType<ObjectCreator, T, FarragoNull, Args...>{this}};
    } else {
      return CreateMoreParamObject<T>(std::make_index_sequence<10>{});
    }
  }

private:
  template <typename T, std::size_t... Ns>
  T CreateMoreParamObject(const std::index_sequence<Ns...> &) {
    //        static_assert(sizeof...(Ns) >= 2, "create failed");

    if constexpr (std::is_constructible_v<
                      T, At<AnyRefType<ObjectCreator, FarragoNull, Args...>,
                            Ns>...>) {
      return T{
          At<AnyRefType<ObjectCreator, FarragoNull, Args...>, Ns>{this}...};
    } else {
      return CreateMoreParamObject<T>(
          std::make_index_sequence<sizeof...(Ns) - 1>{});
    }
  }

private:
  std::tuple<const Args &...> dependency_;
};

} // namespace farrago::obj_creator

#endif // FARRAGO_OBJ_CREATOR_H
