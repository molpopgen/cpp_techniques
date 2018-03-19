// Models current proposal in fwdpp to refactor def'n of diploid

#include <type_traits>
#include <vector>
#include <tuple>
#include <iostream>
#include <typeinfo>

using namespace std;

// This exists in C++14 but not C++11
template <class T>
struct is_null_pointer
    : std::is_same<std::nullptr_t, typename std::remove_reference<
                                       typename std::remove_cv<T>::type>::type>
{
};

// http://stackoverflow.com/questions/11813940/possible-to-use-type-traits-sfinae-to-find-if-a-class-defines-a-member-typec
template <typename...> struct void_t
{
    typedef void type;
};

template <typename T, typename = void> struct is_custom_diploid : false_type
{
};

template <typename T>
struct is_custom_diploid<
    T, typename void_t<typename tuple_element<0, T>::type>::type>
    : std::integral_constant<
          bool, !is_null_pointer<typename tuple_element<0, T>::type>::value>
{
};

using dip = std::tuple<nullptr_t, pair<int, int>>;
using cdip = std::tuple<int, pair<int, int>>;

void f(nullptr_t) { cout << "nullptr\n"; }
void
f(int)
{
    cout << "int\n";
}

template <typename T, typename = decltype(get<0>(T{}))>
void
a(const T& t)
{
    auto y = std::get<0>(t);
    cout << typeid(y).name() << '\n';
    cout << typeid(decltype(get<0>(T{}))).name() << '\n';
}

struct S
{
    // template<typename A>
    // inline void operator()(const A &)const
    // {
    // 	cout << "not nullptr\n";
    // }
    template <typename A,
              typename B
              = typename is_null_pointer<decltype(get<0>(A{}))>::type>
    inline void
    operator()(const A&) const
    {
        cout << B() << '\n';
    }
};

int
main(int argc, char** argv)
{
    cout << "dip is custom: " << is_custom_diploid<dip>::value << '\n';
    cout << "cdip is custom: " << is_custom_diploid<cdip>::value << '\n';
    cout << "int is custom dip: " << is_custom_diploid<int>::value << '\n';
    cout << is_null_pointer<decltype(get<0>(dip{}))>::value << '\n';
    cout << is_null_pointer<nullptr_t>::value << '\n';
    cout
        << is_null_pointer<decltype(get<0>(cdip{}))>::value << ' '
        << is_same<int,
                   remove_reference<decltype(get<0>(cdip{}))>::type>::value
        << ' '
        << is_integral<remove_reference<decltype(get<0>(cdip{}))>::type>::value
        << '\n';
    f(get<0>(dip{}));
    f(get<0>(cdip{}));

    a(dip());
    a(cdip());

    S()(dip());
    S()(cdip());
}
