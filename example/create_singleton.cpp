//
// Created by dxz on 2024/5/13.
//

#include <iostream>
#include <string_view>

#include "singleton_manager.h"

struct Logger;
struct Foo;
using SingletonManager = farrago::singleton_mgr::SingletonManager<Logger, Foo>;

struct Logger {
    Logger() {
        Info("Logger construct");
    }

    ~Logger() {
        Info("Logger destruct");
    }

    void Error(std::string_view v) {
        std::cout << "[ERROR]" << v << std::endl;
    }

    void Info(std::string_view v) {
        std::cout << "[INFO]" << v << std::endl;
    }
};

struct Foo{
    Foo() {
        SingletonManager::Get<Logger>().Info("Foo construct");
    }

    ~Foo() {
        SingletonManager::Get<Logger>().Info("Foo destruct");
    }

    void Method() const {
        SingletonManager::Get<Logger>().Error("Foo Method");
    }
};

int main() {
    SingletonManager::Get<Foo>().Method();
    return 0;
}