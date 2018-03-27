// This works with C++14, but
// can it be modified to work with C++11
// GCC: conda/4.8.5
// clang: whatever's on OS X
#include <functional>
#include <iostream>

void
foo(int a, int & b, double & c)
{
    std::cout << a << ' ' << b << ' ' << c << '\n';
    b++;
    c--;
    std::cout << a << ' ' << b << ' ' << c << '\n';
}

template <typename... Args>
std::function<void(int)>
bindit(Args &&... args)
{
    auto x = [&,args...](int a) mutable {foo(a,args...);};
    return x;
}

int
main(int argc, char **argv)
{
    int a = 2;
    double d = 1.1;
    auto x = bindit(std::ref(a),std::ref(d));
    x(-2);
    std::cout << a << ' ' << d << '\n';
};
