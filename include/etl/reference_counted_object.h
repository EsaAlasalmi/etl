///******************************************************************************
//The MIT License(MIT)
//
//Embedded Template Library.
//https://github.com/ETLCPP/etl
//https://www.etlcpp.com
//
//Copyright(c) 2021 jwellbelove
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
//******************************************************************************/

#ifndef ETL_REFERENCE_COUNTED_OBJECT_INCLUDED
#define ETL_REFERENCE_COUNTED_OBJECT_INCLUDED

#include <stdint.h>

#include "platform.h"
#include "atomic.h"

namespace etl
{
  //***************************************************************************
  /// The base of all reference counters.
  //***************************************************************************
  class ireference_counter
  {
  public:

    virtual ~ireference_counter() {};
    virtual void set_reference_count(int32_t value) = 0;
    virtual void increment_reference_count() = 0;
    ETL_NODISCARD virtual int32_t decrement_reference_count() = 0;
    ETL_NODISCARD virtual int32_t get_reference_count() const = 0;
  };

  //***************************************************************************
  /// A specific type of reference counter.
  //***************************************************************************
  template <typename TCounter>
  class reference_counter : public ireference_counter
  {
  public:

    //***************************************************************************
    /// Constructor.
    //***************************************************************************
    reference_counter()
      : reference_count(0)
    {
    }

    //***************************************************************************
    /// Set the reference count.
    //***************************************************************************
    virtual void set_reference_count(int32_t value) ETL_OVERRIDE
    {
      reference_count = value;
    }

    //***************************************************************************
    /// Increment the reference count.
    //***************************************************************************
    virtual void increment_reference_count() ETL_OVERRIDE
    {
      ++reference_count;
    }

    //***************************************************************************
    /// Decrement the reference count.
    //***************************************************************************
    ETL_NODISCARD virtual int32_t decrement_reference_count() ETL_OVERRIDE
    {
      assert(reference_count > 0);

      return int32_t(--reference_count);
    }

    //***************************************************************************
    /// Get the current reference count.
    //***************************************************************************
    ETL_NODISCARD virtual int32_t get_reference_count() const ETL_OVERRIDE
    {
      return int32_t(reference_count);
    }

  private:

    TCounter reference_count; // The reference count object.
  };

  //***************************************************************************
  /// A null reference counter.
  /// Always returns the reference count as 1, for persistent objects.
  //***************************************************************************
  class null_reference_counter : public ireference_counter
  {
  public:

    //***************************************************************************
    /// Set the reference count.
    //***************************************************************************
    virtual void set_reference_count(int32_t value) ETL_OVERRIDE
    {
    }

    //***************************************************************************
    /// Increment the reference count.
    //***************************************************************************
    virtual void increment_reference_count() ETL_OVERRIDE
    {
    }

    //***************************************************************************
    /// Decrement the reference count.
    //***************************************************************************
    ETL_NODISCARD virtual int32_t decrement_reference_count() ETL_OVERRIDE
    {
      return int32_t(1);
    }

    //***************************************************************************
    /// Get the current reference count.
    //***************************************************************************
    ETL_NODISCARD virtual int32_t get_reference_count() const ETL_OVERRIDE
    {
      return int32_t(1);
    }
  };

  //***************************************************************************
  /// Base for all reference counted objects.
  //***************************************************************************
  class ireference_counted_object
  {
  public:

    virtual ~ireference_counted_object() {}
    ETL_NODISCARD virtual ireference_counter& get_reference_counter() = 0;
    ETL_NODISCARD virtual const ireference_counter& get_reference_counter() const = 0;
  };

  //***************************************************************************
  /// Class for creating reference counted objects.
  /// \tparam TObject  The type to be reference counted.
  /// \tparam TCounter The type to use as the counter.
  //***************************************************************************
  template <typename TObject, typename TCounter>
  class reference_counted_object : public etl::ireference_counted_object
  {
  public:

    typedef TObject  value_type;
    typedef TCounter counter_type;

    //***************************************************************************
    /// Constructor.
    //***************************************************************************
    reference_counted_object(const TObject& object_)
      : object(object_)
    {
    }

    //***************************************************************************
    /// Get a const reference to the counted object.
    //***************************************************************************
    ETL_NODISCARD const value_type& get_object() const
    {
      return object;
    }

    //***************************************************************************
    /// Get a reference to the reference counter.
    //***************************************************************************
    ETL_NODISCARD virtual ireference_counter& get_reference_counter() ETL_OVERRIDE
    {
      return reference_counter;
    }

    //***************************************************************************
    /// Get a const reference to the reference counter.
    //***************************************************************************
    ETL_NODISCARD virtual const ireference_counter& get_reference_counter() const ETL_OVERRIDE
    {
      return reference_counter;
    }

  private:

    // This class must not be default contructed, copy constructed or assigned.
    reference_counted_object() ETL_DELETE;
    reference_counted_object(const reference_counted_object&) ETL_DELETE;
    reference_counted_object& operator =(const reference_counted_object&) ETL_DELETE;
        
    const TObject object;                               ///< The object being reference counted.
    etl::reference_counter<TCounter> reference_counter; ///< The reference counter.
  };

  //***************************************************************************
  /// Persistent message type.
  /// The message type will always have a reference count of 1. 
  /// \tparam TObject  The type stored in the object.
  //***************************************************************************
  template <typename TObject>
  class persistent_object : public etl::ireference_counted_object
  {
  public:

    typedef TObject value_type;

    //***************************************************************************
    /// Constructor.
    //***************************************************************************
    persistent_object(const TObject& object_)
      : object(object_)
    {
    }

    //***************************************************************************
    /// Get a const reference to the counted object.
    //***************************************************************************
    ETL_NODISCARD const value_type& get_object() const
    {
      return object;
    }

    //***************************************************************************
    /// Get a reference to the reference counter.
    //***************************************************************************
    ETL_NODISCARD virtual ireference_counter& get_reference_counter() ETL_OVERRIDE
    {
      return reference_counter;
    }

    //***************************************************************************
    /// Get a const reference to the reference counter.
    //***************************************************************************
    ETL_NODISCARD virtual const ireference_counter& get_reference_counter() const ETL_OVERRIDE
    {
      return reference_counter;
    }

  private:

    // This class must not be default contructed, copy constructed or assigned.
    persistent_object() ETL_DELETE;
    persistent_object(const persistent_object&) ETL_DELETE;
    persistent_object& operator =(const persistent_object&) ETL_DELETE;

    TObject object;                                ///< The object being reference counted.
    etl::null_reference_counter reference_counter; ///< The reference counter.
  };

#if ETL_CPP11_SUPPORTED && ETL_HAS_ATOMIC
  //***************************************************************************
  /// Class for creating reference counted objects using an atomic counter.
  /// \tparam TObject  The type to be reference counted.
  //***************************************************************************
  template <typename TObject>
  using atomic_counted_object = etl::reference_counted_object<TObject, etl::atomic_int32_t>;
#endif
}

#endif