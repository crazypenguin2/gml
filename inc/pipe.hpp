#ifndef GML_INC_PIPE_HPP
#define GML_INC_PIPE_HPP

#include <tuple>

/* change_last_type<Template<Ts...>, LT>
 * this template changes last parameter in Template's parameter pack Ts...
 * with LT.
 */

template <template<class...> class Template, class LT, class T, class... Ts>
struct change_last_type_base;

template <template<class...> class Template, class LT, std::size_t... I, class... Ts>
struct change_last_type_base<Template, LT, std::index_sequence<I...>, Ts...>
{
  using type = Template<typename std::tuple_element<I, std::tuple<Ts...>>::type..., LT>;
};

template <template<class...> class Template, class LT, class... Ts>
struct change_last_type
  : change_last_type_base<Template, LT,
                          std::make_index_sequence<sizeof...(Ts) - 1>, Ts...>
{
  using type =
    typename change_last_type_base<Template, LT,
                     std::make_index_sequence<sizeof...(Ts) - 1>, Ts...>::type;
};


class none;

template<class... E>
class pipe 
{};

template<template<class...> class S, class... E, class... PP>
class pipe<S<PP...>, E...> : public pipe<E...>
{
public:
  using type = typename change_last_type<S, typename pipe<E...>::type, PP...>::type;
};

template<>
class pipe<>
{
public:
  class type{};
};

template <class... E>
auto make_pipe(E... e)
{
  using P = typename pipe<E...>::type;
  return P{}; 
};

#endif //GML_INC_PIPE_HPP
