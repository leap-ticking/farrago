//
// Created by dxz on 2023/8/26.
//

#include <iostream>
#include <cassert>

#include "farrago.h"

struct Member{
    Member() = default;
    Member(int x): x{x} {}
    Member(const Member& m) : x(m.x) {
        std::cout << "Member(const Member& m) " << std::endl;
    }

    int x = 10;
};

struct Member1 {
    Member1() = default;
    Member1(int x): x{x} {}
    Member1(const Member1& m) : x(m.x) {
        std::cout << "Member1(const Member& m) " << std::endl;
    }

    int x = 11;
};

class Example1{
public:
//    Example1(Member x) {
//        std::cout << x.x << std::endl;
//    }

//    Example1(const Member& x) {
//        std::cout << x.x << std::endl;
//    }

//    Example1(Member& x) {
//        std::cout << x.x << std::endl;
//    }

//    Example1(Member&& x) {
//        std::cout << x.x << std::endl;
//    }

//    Example1(Member& x, const Member1& x1) {
//        std::cout << x.x << std::endl;
//        std::cout << x1.x << std::endl;
//    }

    Example1(Member x, Member1 x1) {
        std::cout << x.x << std::endl;
        std::cout << x1.x << std::endl;
    }

//    Example1(Member&& x, Member1 x1) {
//        std::cout << x.x << std::endl;
//        std::cout << x1.x << std::endl;
//    }
};

int main() {
    Member m{6};
    Member1 m1{7};
//    auto t1 = farrago::ObjectCreator<Member, Member1>(m, m1).template Create<Example1>();
    auto t2 = farrago::ObjectCreator<>().template Create<Example1>();

//    std::move()

    return 0;
}