# farrago
C++ some tool

## 1. create a object
Create object and no need pass full params
```c++
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
    auto t1 = farrago::ObjectCreator<Member>(m).template Create<Example1>(); // bind m
    auto t2 = farrago::ObjectCreator<>().template Create<Example1>(); // default Member object

    return 0;
}
```
