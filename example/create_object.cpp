//
// Created by dxz on 2023/8/26.
//

#include <iostream>
#include <cassert>

#include "farrago.h"

struct Member{
    Member() = default;
    Member(const Member& m) {
        std::cout << "Member(const Member& m) " << std::endl;
    }

    int x = 10;
};

class Example1{
public:
    Example1(const Member& x) {
        assert(x.x == 10);
    }
};

int main() {
    Member m;
    auto t1 = farrago::ObjectCreator<Member>(m).template Create<Example1>();
    auto t2 = farrago::ObjectCreator<>().template Create<Example1>();

    return 0;
}