///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2023 John Wellbelove

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#ifndef ETL_TUPLE_INCLUDED
#define ETL_TUPLE_INCLUDED

///\defgroup tuple tuple
/// tuple implementation
///\ingroup utilities

#include "platform.h"

#if ETL_NOT_USING_CPP11
  #if !defined(ETL_IN_UNIT_TEST)
    #error NOT SUPPORTED FOR C++03 OR BELOW
  #endif
#else

#include "nth_type.h"
#include "type_traits.h"
#include "utility.h"

#include <stddef.h>

namespace etl
{
  namespace private_tuple
  {
    //*************************************************************************
    template <size_t Index, typename T>
    class tuple_element
    {
    public:

      //*********************************
      tuple_element()
        : value()
      {
      }

      //*********************************
      tuple_element(const T& v)
      {
        value = v;
      }

      //*********************************
      tuple_element(T&& v)
      {
        value = etl::move(v);
      }

      //*********************************
      T& get()
      {
        return value;
      }

      //*********************************
      const T& get() const
      {
        return value;
      }

    private:

      T value;
    };

    //*************************************************************************
    template <size_t Index, typename... TTypes>
    class tuple_base
    {
    };

    //*********************************
    template <size_t Index, typename TFirst, typename... TRest>
    class tuple_base<Index, TFirst, TRest...> : public tuple_element<Index, typename etl::remove_reference<TFirst>::type>,
                                                public tuple_base<Index + 1U, TRest...>
    {
    public:

      /// Constructor.
      tuple_base()
      {
      }

      /// Constructor.
      template <typename TF, typename... TR>
      tuple_base(TF&& first, TR&&... rest) 
        : tuple_element<Index, typename etl::remove_reference_t<TF>>(etl::forward<TF>(first))
        , tuple_base<Index + 1U, TRest...>(etl::forward<TRest>(rest)...)
      {
      }
    };
  }

  //***************************************************************************
  /// tuple 
  //***************************************************************************
  template <typename TFirst, typename... TRest>
  class tuple : public private_tuple::tuple_base<0, TFirst, TRest...>
  {
  public:

    //*************************************************************************
    ETL_CONSTEXPR tuple()
    {
    }

    //*************************************************************************
    template <typename... TArgs>
    ETL_CONSTEXPR tuple(TArgs&&... args)
      : private_tuple::tuple_base<0, TFirst, TRest...>(etl::forward<TArgs>(args)...)
    {
    }

    //*************************************************************************
    template <typename... TArgs>
    friend ETL_CONSTEXPR bool operator ==(tuple<TArgs...>& t1, tuple<TArgs...>& t2);
  };

#if ETL_USING_CPP17
  //***************************************************************************
  /// Template deduction guideline
  //***************************************************************************
  template <typename... TArgs>
  tuple(TArgs... args) -> tuple<TArgs...>;
#endif

  //***************************************************************************
  /// Get the value from the tuple at the index.
  //***************************************************************************
  template <size_t Index, typename... TArgs>
  ETL_NODISCARD
  ETL_CONSTEXPR
  etl::remove_reference_t<etl::nth_type_t<Index, TArgs...>>& get(etl::tuple<TArgs...>& t)
  {
    using value_type   = etl::remove_reference_t<etl::nth_type_t<Index, TArgs...>>;
    using element_type = private_tuple::tuple_element<Index, value_type>&;

    return (static_cast<element_type>(t)).get();
  }

  //***************************************************************************
  /// Get the value from the tuple at the index.
  //***************************************************************************
  template <size_t Index, typename... TArgs>
  ETL_NODISCARD
  ETL_CONSTEXPR
  etl::remove_reference_t<etl::nth_type_t<Index, TArgs...>>& get(const etl::tuple<TArgs...>& t)
  {
    using value_type = etl::remove_reference_t<etl::nth_type_t<Index, TArgs...>>;
    using element_type = const private_tuple::tuple_element<Index, value_type>&;

    return (static_cast<element_type>(t)).get();
  }

  //***************************************************************************
  /// Get the value from the tuple at the index.
  //***************************************************************************
  template <size_t Index, typename... TArgs>
  ETL_NODISCARD
  ETL_CONSTEXPR
  etl::remove_reference_t<etl::nth_type_t<Index, TArgs...>>&& get(etl::tuple<TArgs...>&& t)
  {
    using value_type   = etl::remove_reference_t<etl::nth_type_t<Index, TArgs...>>;
    using element_type = private_tuple::tuple_element<Index, value_type>&&;

    return etl::move((static_cast<element_type>(t)).get());
  }

  //***************************************************************************
  /// Get the value from the tuple at the index.
  //***************************************************************************
  template <size_t Index, typename... TArgs>
  ETL_NODISCARD
  ETL_CONSTEXPR 
  const etl::remove_reference_t<etl::nth_type_t<Index, TArgs...>>&& get(const etl::tuple<TArgs...>&& t)
  {
    using value_type   = etl::remove_reference_t<etl::nth_type_t<Index, TArgs...>>;
    using element_type = const private_tuple::tuple_element<Index, value_type>&&;

    return etl::move((static_cast<element_type>(t)).get());
  }
}

#endif
#endif
