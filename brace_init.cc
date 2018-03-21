// Playing with "uniform initialization"
#include <iostream>
#include <vector>
#include <string>

struct foo
{
    int a;
    int b;
};

struct bar
{
    int a;
    int b;
    bar(int a_):a(a_),b(a)
    {
    }
};

struct foobar
{
    std::string a;
    int b;
};

template<typename T>
struct make_type
{
    template<typename... Args>
    inline T operator()(Args... args) const
    {
        return T{std::forward<Args>(args)...};
    }
};

int main(int argc, char **argv)
{
    foo a{1,2};
    foo a2{2};
    std::cout << a.a << ' ' << a.b << '\n'
        << a2.a << ' ' << a2.b << '\n';
    bar b{1};
    // Fails to compile:
    //bar c{1,2};
    
    foobar fb{};
    std::cout << fb.a << ' ' << fb.b << '\n';
    fb=foobar{"x",1};
    std::cout << fb.a << ' ' << fb.b << '\n';

    a = make_type<foo>()(1,2);
    std::cout << a.a << ' ' << a.b << '\n';
    a = make_type<foo>()(2);
    std::cout << a.a << ' ' << a.b << '\n';

    b = make_type<bar>()(1);

    std::vector<foo> vfoo;
    vfoo.emplace_back(foo{1,2});
}
