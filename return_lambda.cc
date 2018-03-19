#include <functional>
#include <iostream>

using namespace std;

function<void(int)>
f()
{
    return [](int x) { cout << x << '\n'; };
}

using ftype = void (*)(int);

ftype
f2()
{
    return [](int x) { cout << x << '\n'; };
}

function<void(int)>
f3(int y)
{
	// Testing suggest capture by ref or value here works
	// on GCC 4.8.5, but that seems hard to believe...
    return [y](int x) { cout << x << ' '  << x + y << '\n'; };
}

//Very cool, but C++14 only
#if __cplusplus > 201103L
auto
f4(int y)
{
	// MUST capture by value here, and
	// not by reference
    return [y](int x) { cout << x << ' ' << x : y << '\n'; };
}
#endif

int
main(int argc, char** argv)
{
    auto a = f();
    auto b = f2();
    auto c = f3(2);
    a(1);
    b(1);
    c(1);
#if __cplusplus > 201103L
	auto d = f4(2);
	d(1);
	cout << typeid(c).name() << ' ' << typeid(d).name() << '\n';
#endif
}
