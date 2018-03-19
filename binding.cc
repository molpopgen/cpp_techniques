#include <functional>
#include <iostream>

using namespace std;

void
foo(int &x)
{
    cout << x << ' ';
    ++x;
    cout << x << '\n';
}

int
main(int argc, char **argv)
{
    int x{ 1 };
    auto a = bind(foo, x);
    auto a2 = bind(foo, ref(x));
    auto b = [&x]() { foo(x); };
    a();
    cout << "Early binding: " << x << '\n';
    a2();
    cout << "Eearly binding via std::ref: " << x << '\n';
    b();
    cout << "Late binding via lambda: " << x << '\n';
}
