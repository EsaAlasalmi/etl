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

#include "private/tuple_element.h"
#include "private/tuple_size.h"

#include <assert.h> // <<< TODO Remove
#include <stddef.h>

#if ETL_USING_STL
  #include <tuple>
#endif

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
        : value(v)
      {
      }

      //*********************************
      tuple_element(T&& v)
        : value(etl::move(v))
      {
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

      T value;
    };

    //*************************************************************************
    /// tuple_base
    /// Generates the tuple_element base classes by recursive inheritance.
    //*************************************************************************  
    //*********************************
    /// Declaration.
    // Used when there are no types left.
    //*********************************
    template <size_t Index, typename... TTypes>
    class tuple_base
    {
    };

    //*********************************
    /// Recursive base template.
    //*********************************
    template <size_t Index, typename TFirst, typename... TRest>
    class tuple_base<Index, TFirst, TRest...> : public tuple_element<Index, etl::remove_reference_t<TFirst>>,
                                                public tuple_base<Index + 1U, TRest...>
    {
    public:

      //*********************************
      /// Default constructor.
      //*********************************
      tuple_base()
      {
      }

      //*********************************
      // Constructor.
      //*********************************
      tuple_base(const TFirst& first, const TRest&... rest)
        : tuple_element<Index, etl::remove_reference_t<TFirst>> (first)
        , tuple_base<Index + 1U, TRest...>(rest...)
      {
      }

      //*********************************
      /// Constructor.
      //*********************************
      template <typename TF, typename... TR>
      tuple_base(TF&& first, TR&&... rest)
        : tuple_element<Index, etl::remove_reference_t<TFirst>>(etl::forward<TFirst>(first))
        , tuple_base<Index + 1U, TRest...>(etl::forward<TRest>(rest)...)
      {
      }

      //*********************************
      /// Move Constructor.
      //*********************************
      tuple_base(TFirst&& first, TRest&&... rest)
        : tuple_element<Index, etl::remove_reference_t<TFirst>>(etl::forward<TFirst>(first))
        , tuple_base<Index + 1U, TRest...>(etl::forward<TRest>(rest)...)
      {
      }
    };
  }

  //***************************************************************************
  /// tuple 
  //***************************************************************************
  template <typename... TTypes>
  class tuple : public private_tuple::tuple_base<0U, TTypes...>
  {
  public:

    static ETL_CONSTANT size_t Size = sizeof...(TTypes);

    //*************************************************************************
    ETL_CONSTEXPR tuple()
    {
    }

    //*************************************************************************
    ETL_CONSTEXPR tuple(const TTypes&... args)
      : private_tuple::tuple_base<0U, TTypes...>(args...)
    {
    }

    //*************************************************************************
    template <typename... TArgs>
    ETL_CONSTEXPR tuple(TArgs&&... args)
      : private_tuple::tuple_base<0U, TTypes...>(etl::forward<TArgs>(args)...)
    {
    }

    //*************************************************************************
    template <typename... UTypes>
    ETL_CONSTEXPR tuple(tuple<UTypes...>& other)
    {
      assert(false);
    }

    //*************************************************************************
    template <typename... UTypes>
    ETL_CONSTEXPR tuple(const tuple<UTypes...>& other)
    {
      assert(false);
    }

    //*************************************************************************
    template <typename... UTypes>
    ETL_CONSTEXPR tuple(tuple<UTypes...>&& other)
    {
      assert(false);
    }

    //*************************************************************************
    template <typename... UTypes>
    ETL_CONSTEXPR tuple(const tuple<UTypes...>&& other)
    {
      assert(false);
    }

#if ETL_USING_STL
    //*************************************************************************
    /// For tuple-like types.
    //*************************************************************************
    template <typename TTuple>
    ETL_CONSTEXPR tuple(TTuple&& t)
    {
      //etl::get<TTypes, >
    }
#endif

    //*************************************************************************
    template <typename U1, typename U2>
    ETL_CONSTEXPR tuple(ETL_OR_STD::pair<U1, U2>& p)
      : private_tuple::tuple_base<0, TTypes...>(p.first, p.second)
    {
    }

    //*************************************************************************
    template <typename U1, typename U2>
    ETL_CONSTEXPR tuple(const ETL_OR_STD::pair<U1, U2>& p)
      : private_tuple::tuple_base<0, TTypes...>(p.first, p.second)
    {
    }

    //*************************************************************************
    template <typename U1, typename U2>
    ETL_CONSTEXPR tuple(ETL_OR_STD::pair<U1, U2>&& p)
      : private_tuple::tuple_base<0, TTypes...>(etl::move(p.first), etl::move(p.second))
    {
    }

    //*************************************************************************
    template <typename U1, typename U2>
    ETL_CONSTEXPR tuple(const ETL_OR_STD::pair<U1, U2>&& p)
      : private_tuple::tuple_base<0, TTypes...>(etl::move(p.first), etl::move(p.second))
    {
    }

    //*************************************************************************
    tuple(const tuple& other) = default;
    tuple(tuple&& other) = default;

    //*************************************************************************
    ETL_CONSTEXPR size_t size()
    {
      return Size;
    }

    //*************************************************************************
    template <typename... TArgs>
    friend ETL_CONSTEXPR bool operator ==(tuple<TArgs...>& t1, tuple<TArgs...>& t2);
  };

  //***************************************************************************
  /// Size of the tuple (static member constant)
  //***************************************************************************
  template <typename... TTypes>
  ETL_CONSTANT size_t tuple<TTypes...>::Size;

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
  etl::remove_reference_t<etl::nth_type_t<Index, TArgs...>>& get(etl::tuple<TArgs...>& t) ETL_NOEXCEPT
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
  const etl::remove_reference_t<etl::nth_type_t<Index, TArgs...>>& get(const etl::tuple<TArgs...>& t) ETL_NOEXCEPT
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
  etl::remove_reference_t<etl::nth_type_t<Index, TArgs...>>&& get(etl::tuple<TArgs...>&& t) ETL_NOEXCEPT
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
  const etl::remove_reference_t<etl::nth_type_t<Index, TArgs...>>&& get(const etl::tuple<TArgs...>&& t) ETL_NOEXCEPT
  {
    using value_type   = etl::remove_reference_t<etl::nth_type_t<Index, TArgs...>>;
    using element_type = const private_tuple::tuple_element<Index, value_type>&&;

    return etl::move((static_cast<element_type>(t)).get());
  }

  //***************************************************************************
  /// Get the value from the tuple at the index.
  //***************************************************************************
  //template <typename T, typename... Types >
  //constexpr T& get(tuple<Types...>& t) noexcept
  //{
  //}

  //template <typename T, typename... Types >
  //constexpr T&& get(tuple<Types...>&& t) noexcept
  //{
  //}
  //
  //template <typename T, typename... Types >
  //constexpr const T& get(const tuple<Types...>& t) noexcept
  //{
  //}

  //template <typename T, typename... Types >
  //constexpr const T&& get(const tuple<Types...>&& t) noexcept
  //{
  //}

  //***************************************************************************
  /// Get the size of the tuple.
  //***************************************************************************
  template <typename... TTypes>
  struct tuple_size<etl::tuple<TTypes...>> : etl::integral_constant<size_t, sizeof...(TTypes)>
  {
  };

#if ETL_USING_CPP11 && ETL_USING_STL
  template <typename... TTypes>
  struct tuple_size<std::tuple<TTypes...>> : etl::integral_constant<size_t, sizeof...(TTypes)>
  {
  };
#endif

  //***************************************************************************
  /// ignore
  /// An object of unspecified type such that any value can be assigned to it with no effect. 
  /// Intended for use with etl::tie when unpacking a etl::tuple, as a placeholder for the arguments that are not used.
  /// https://en.cppreference.com/w/cpp/utility/tuple/ignore
  //***************************************************************************
  namespace private_tuple
  {
    struct ignore_t
    {
      template <typename T>
      ETL_CONSTEXPR void operator =(T&&) const ETL_NOEXCEPT 
      {
      }
    };
  }

#if ETL_USING_CPP17
  inline constexpr private_tuple::ignore_t ignore;
#else
  static const private_tuple::ignore_t ignore;
#endif

  //***************************************************************************
  /// Get the element type at the index in the tuple.
  //***************************************************************************
  template<size_t Index, typename... TTypes>
  struct tuple_element<Index, etl::tuple<TTypes...>>
  {
    using type = etl::remove_reference_t<etl::nth_type_t<Index, TTypes...>>;
  };

  //***************************************************************************
  /// tie
  //***************************************************************************
  template <typename... TArgs>
  ETL_CONSTEXPR etl::tuple<TArgs&...> tie(TArgs&... args) noexcept 
  {
    return { args... };
  }
}

#endif
#endif
