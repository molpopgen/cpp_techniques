// Models current proposal in fwdpp to refactor def'n of diploid

// The goal is:
// pair<int-like,int-like>, aka pii is a single-locus diploid
// tuple<pii, T> is a single-locus-diploid with metadata
// array_like<pii> is a multi-locus diploid.  array_like could be std::vector,
// std::array, or any duck-type thereof
// tuple<array_like<pii>,T> is a multi-locus diploid with meta-data.
// We should not care if the tuples are longer than two elements.

#include <type_traits>
#include <cassert>
#include <utility>
#include <vector>
#include <array>
#include <tuple>
#include <iostream>
#include <typeinfo>

using namespace std;

// http://stackoverflow.com/questions/11813940/possible-to-use-type-traits-sfinae-to-find-if-a-class-defines-a-member-typec
template <typename...> struct void_t
{
    typedef void type;
};

template <typename T, typename = void> struct is_pair_like : false_type
{
};

template <typename T>
struct is_pair_like<
    T, typename void_t<typename T::first_type, typename T::second_type>::type>
    : true_type
{
};

template <typename T, typename = void>
struct is_single_region_diploid : false_type
{
};

template <typename T>
struct is_single_region_diploid<
    T, typename void_t<typename tuple_element<1, T>::type>::type>
    : std::integral_constant<
          bool, (is_integral<typename tuple_element<0, T>::type>::value
                 && is_integral<typename tuple_element<1, T>::type>::value
                 && is_pair_like<T>::value)
                    || is_pair_like<typename tuple_element<0, T>::type>::value>
{
};

template <typename T, typename = void>
struct is_multi_region_diploid : false_type
{
};

template <typename T>
struct is_multi_region_diploid<T,
                               typename void_t<typename T::value_type>::type>
    : is_pair_like<typename T::value_type>
{
};

template <typename T>
struct is_multi_region_diploid<
    T, typename void_t<typename tuple_element<0, T>::type::value_type>::type>
    : is_pair_like<typename tuple_element<0, T>::type::value_type>
{
};

template <typename T, typename = void> struct is_custom_diploid : false_type
{
};

template <typename T>
struct is_custom_diploid<
    T, typename void_t<typename tuple_element<1, T>::type>::type>
    : std::integral_constant<
          bool,
          is_single_region_diploid<typename tuple_element<0, T>::type>::value
              || is_multi_region_diploid<
                     typename tuple_element<0, T>::type>::value>
{
};

using dip = pair<int, int>;
using cdip = std::tuple<pair<int, int>, int>;
using mdip = std::tuple<vector<pair<int, int>>, int>;

// The proposal will benefit from helper functions
// and tag distpatch.  This stuff is pretty standard,
// but requires some boilerplate code. The compiler
// will optimize most or all of the dispatch code away,
// resulting in direct access.

struct custom_diploid_tag
{
};

struct standard_diploid_tag
{
};

template <typename T> struct get_diploid_tag
{
    using type = typename std::conditional<
        is_custom_diploid<typename std::remove_const<T>::type>::value,
        custom_diploid_tag, standard_diploid_tag>::type;
};

template <typename T>
auto
get_first_dispatch(T& t, custom_diploid_tag) -> decltype(std::get<0>(t).first)
{
    return std::get<0>(t).first;
}

template <typename T>
inline auto
get_first_dispatch(T& t, standard_diploid_tag) -> decltype(std::get<0>(t))
{
    return std::get<0>(t);
}

template <typename T>
inline auto
get_first(T& t)
    -> decltype(get_first_dispatch(t, typename get_diploid_tag<T>::type()))
{
    return get_first_dispatch(t, typename get_diploid_tag<T>::type());
}

template <typename T>
inline auto
get_second_dispatch(T& t, custom_diploid_tag)
    -> decltype(std::get<0>(t).second)
{
    return std::get<0>(t).second;
}

template <typename T>
inline auto
get_second_dispatch(T& t, standard_diploid_tag) -> decltype(std::get<0>(t))
{
    return std::get<1>(t);
}

template <typename T>
inline auto
get_second(T& t)
    -> decltype(get_second_dispatch(t, typename get_diploid_tag<T>::type()))
{
    return get_second_dispatch(t, typename get_diploid_tag<T>::type());
}

int
main(int argc, char** argv)
{
	static_assert(is_single_region_diploid<dip>::value,"foo");
	static_assert(!is_custom_diploid<dip>::value,"foo");
	static_assert(is_same<get_diploid_tag<dip>::type,standard_diploid_tag>::value,"foo");

	static_assert(is_single_region_diploid<cdip>::value,"foo");
	static_assert(is_custom_diploid<cdip>::value,"foo");
	static_assert(is_same<get_diploid_tag<cdip>::type,custom_diploid_tag>::value,"foo");

    //cout << "dip is single region: " << is_single_region_diploid<dip>::value
    //     << '\n';
    //cout << "dip is custom: " << is_custom_diploid<dip>::value << '\n';
    //cout << "cdip is single region: " << is_single_region_diploid<cdip>::value
    //     << '\n';
    //cout << "cdip is custom: " << is_custom_diploid<cdip>::value << '\n';
    //cout << "int is custom dip: " << is_custom_diploid<int>::value << '\n';
    //cout << "mdip is single region: " << is_single_region_diploid<mdip>::value
    //     << '\n'
    //     << "mdip is multi region: " << is_multi_region_diploid<mdip>::value
    //     << '\n'
    //     << "mdip is custom: " << is_custom_diploid<mdip>::value << '\n';
    //cout << "vector<pair<int,int>> is multi region: "
    //     << is_multi_region_diploid<vector<pair<int, int>>>::value << '\n';
    //cout << "array<pair<int,int>,2> is multi region: "
    //     << is_multi_region_diploid<array<pair<int, int>, 2>>::value << '\n';
    //cout << "pair<int,int> is single region: "
    //     << is_single_region_diploid<pair<int, int>>::value << '\n'
    //     << "pair<pair<int,int>,int> is single region "
    //     << is_single_region_diploid<pair<pair<int, int>, int>>::value << '\n'
    //     << "pair<tuple<int,int>,int> is single region "
    //     << is_single_region_diploid<pair<tuple<int, int>, int>>::value << '\n'
    //     << "tuple<pair<int,int>,int> is single region "
    //     << is_single_region_diploid<tuple<pair<int, int>, int>>::value << '\n'
    //     << "tuple<int,int> is single region: "
    //     << is_single_region_diploid<tuple<int, int>>::value << '\n';

    pair<int, int> x{ 1, 2 };
    assert(get_first(x) == 1);
    assert(get_second(x) == 2);
    get_second(x) = 2;
    assert(get_second(x) == 2);
    get_first(x) += 4;
    assert(get_first(x) == 5);
    const pair<int, int> y{ -3, 5 };
    assert(get_first(y) == -3);
    assert(get_second(y) == 5);
    // Fails to compile:
    // get_first(y) += 3;

    static_assert(
        !is_const<remove_reference<decltype(get_first(x))>::type>::value,
        "foo");
    static_assert(is_reference<decltype(get_first(x))>::value, "foo");
    static_assert(
        !is_const<remove_reference<decltype(get_first(x))>::type>::value,
        "foo");
    static_assert(is_reference<decltype(get_second(x))>::value, "foo");
    static_assert(
        is_const<remove_reference<decltype(get_first(y))>::type>::value,
        "foo");
    static_assert(is_reference<decltype(get_first(y))>::value, "foo");
    static_assert(
        is_const<remove_reference<decltype(get_second(y))>::type>::value,
        "foo");
    static_assert(is_reference<decltype(get_second(y))>::value, "foo");
}
