///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.

Copyright(c) 2014 jwellbelove

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

#ifndef __ETL_IN_ILIST_H__
#error This header is a private element of etl::list & etl::ilist
#endif

#ifndef __ETL_LIST_BASE__
#define __ETL_LIST_BASE__

#include <cstddef>
#include "exception.h"

namespace etl
{
#ifdef ETL_USE_EXCEPTIONS
  //***************************************************************************
  /// Exception for the list.
  ///\ingroup list
  //***************************************************************************
  class list_exception : public exception
  {
  public:

    list_exception(const char* what)
      : exception(what)
    {
    }
  };

  //***************************************************************************
  /// Full exception for the list.
  ///\ingroup list
  //***************************************************************************
  class list_full : public list_exception
  {
  public:

    list_full()
      : list_exception("List full")
    {
    }
  };

  //***************************************************************************
  /// Iterator exception for the list.
  ///\ingroup list
  //***************************************************************************
  class list_iterator : public list_exception
  {
  public:

    list_iterator()
      : list_exception("Iterator problem")
    {
    }
  };
#endif

  //***************************************************************************
  /// The base class for all lists.
  ///\ingroup list
  //***************************************************************************
  class list_base
  {
  public:

    typedef size_t size_type; ///< The type used for determining the size of list.

    //*************************************************************************
    /// Gets the size of the list.
    //*************************************************************************
    size_type size() const
    {
      return count;
    }

    //*************************************************************************
    /// Gets the maximum possible size of the list.
    //*************************************************************************
    size_type max_size() const
    {
      return MAX_SIZE;
    }

    //*************************************************************************
    /// Checks to see if the list is empty.
    //*************************************************************************
    bool empty() const
    {
      return count == 0;
    }

    //*************************************************************************
    /// Checks to see if the list is full.
    //*************************************************************************
    bool full() const
    {
      return count == MAX_SIZE;
    }

  protected:

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    list_base(size_type max_size)
      : next_free(0),
        count(0),
        MAX_SIZE(max_size)

    {
    }

    size_type next_free;      ///< The index of the next free node.
    size_type count;          ///< The number of the used nodes.
    const size_type MAX_SIZE; ///< The maximum size of the list.
  };
}

#endif
