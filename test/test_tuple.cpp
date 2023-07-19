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

#include "unit_test_framework.h"

#include "Data.h"

#include "etl/tuple.h"

#include <string>
#include <tuple>

namespace
{
  using Data  = TestDataDC<std::string>;
  using DataM = TestDataM<std::string>;

  SUITE(test_tuple)
  {
    //*************************************************************************
    TEST(test_etl_tuple_size)
    {
      using Tuple = etl::tuple<int, double, int, Data>;

      Tuple theTuple;

      CHECK_EQUAL(4U, etl::tuple_size<Tuple>::value);
      CHECK_EQUAL(4U, theTuple.size());

#if (ETL_USING_CPP17)
      CHECK_EQUAL(4U, etl::tuple_size_v<Tuple>);
#endif
    }

    //*************************************************************************
    TEST(test_std_tuple_size)
    {
      using Tuple = std::tuple<int, double, int, Data>;

      Tuple theTuple;

      CHECK_EQUAL(4U, etl::tuple_size<Tuple>::value);

#if (ETL_USING_CPP17)
      CHECK_EQUAL(4U, etl::tuple_size_v<Tuple>);
#endif
    }

    //*************************************************************************
    TEST(test_default_constructor)
    {
      etl::tuple<int, double, int, Data> theTuple;

      int i0          = etl::get<0>(theTuple);
      double d1       = etl::get<1>(theTuple);
      int i2          = etl::get<2>(theTuple);
      std::string& s3 = etl::get<3>(theTuple).value;

      CHECK_EQUAL(0, i0);
      CHECK_EQUAL(0, d1);
      CHECK_EQUAL(0, i2);
      CHECK_TRUE(s3.empty());
    }

    //*************************************************************************
    TEST(test_construct_from_parameters)
    {
      etl::tuple<int, double, int, Data> theTuple(1, 2.2, 3, Data("4"));

      int i0          = etl::get<0>(theTuple);
      double d1       = etl::get<1>(theTuple);
      int i2          = etl::get<2>(theTuple);
      std::string& s3 = etl::get<3>(theTuple).value;

      CHECK_EQUAL(1, i0);
      CHECK_EQUAL(2.2, d1);
      CHECK_EQUAL(3, i2);
      CHECK_EQUAL(std::string("4"), s3);
    }

    //*************************************************************************
    TEST(test_construct_from_parameters_including_move_only)
    {
      etl::tuple<int, double, int, DataM> theTuple(1, 2.2, 3, etl::move(DataM("4")));

      int i0          = etl::get<0>(theTuple);
      double d1       = etl::get<1>(theTuple);
      int i2          = etl::get<2>(theTuple);
      std::string& s3 = etl::get<3>(theTuple).value;

      CHECK_EQUAL(1, i0);
      CHECK_EQUAL(2.2, d1);
      CHECK_EQUAL(3, i2);
      CHECK_EQUAL(std::string("4"), s3);
    }

#if ETL_USING_CPP17
    //*************************************************************************
    TEST(test_constructor_from_parameters_using_template_deduction_guideline)
    {
      etl::tuple theTuple(1, 2.2, 3, Data("4"));

      int i0          = etl::get<0>(theTuple);
      double d1       = etl::get<1>(theTuple);
      int i2          = etl::get<2>(theTuple);
      std::string& s3 = etl::get<3>(theTuple).value;

      CHECK_EQUAL(1, i0);
      CHECK_EQUAL(2.2, d1);
      CHECK_EQUAL(3, i2);
      CHECK_EQUAL(std::string("4"), s3);
    }
#endif

    //*************************************************************************
    TEST(test_construct_from_std_tuple)
    {
      std::tuple<int, double, int, Data> stdTuple(1, 2.2, 3, Data("4"));
      etl::tuple<int, double, int, Data> etlTuple(stdTuple);

      //int i0 = etl::get<0>(theTuple);
      //double d1 = etl::get<1>(theTuple);
      //int i2 = etl::get<2>(theTuple);
      //std::string& s3 = etl::get<3>(theTuple).value;

      //CHECK_EQUAL(1, i0);
      //CHECK_EQUAL(2.2, d1);
      //CHECK_EQUAL(3, i2);
      //CHECK_EQUAL(std::string("4"), s3);
    }

    //*************************************************************************
    TEST(test_get)
    {
      etl::tuple<Data, DataM> theTuple(Data("1"), DataM("3"));
      const etl::tuple<Data, DataM> const_data(Data("2"), DataM("4"));

      Data d0  = etl::get<0>(theTuple);
      Data d1  = etl::get<0>(const_data);
      DataM d2 = etl::get<1>(etl::move(theTuple));
      DataM d3 = etl::get<1>(etl::move(const_data));

      CHECK_EQUAL(std::string("1"), d0.value);
      CHECK_EQUAL(std::string("2"), d1.value);
      CHECK_EQUAL(std::string("3"), d2.value);
      CHECK_EQUAL(std::string("4"), d3.value);
    }

    //*************************************************************************
    TEST(test_construct_from_pair)
    {
      ETL_OR_STD::pair<int, Data> p(1, Data("2"));

      etl::tuple<int, Data> theTuple(p);

      int  i = etl::get<0>(theTuple);
      Data d = etl::get<1>(theTuple);

      CHECK_EQUAL(1, i);
      CHECK_EQUAL(std::string("2"), d.value);
    }

    //*************************************************************************
    TEST(test_construct_from_const_pair)
    {
      const ETL_OR_STD::pair<int, Data> p(1, Data("2"));

      etl::tuple<int, Data> theTuple(p);

      int  i = etl::get<0>(theTuple);
      Data d = etl::get<1>(theTuple);

      CHECK_EQUAL(1, i);
      CHECK_EQUAL(std::string("2"), d.value);
    }

    //*************************************************************************
    TEST(test_construct_from_move_only_pair)
    {
      ETL_OR_STD::pair<int, DataM> p(1, etl::move(DataM("2")));

      etl::tuple<int, DataM> theTuple(etl::move(p));

      int  i = etl::get<0>(theTuple);
      DataM d = etl::move(etl::get<1>(theTuple));

      CHECK_EQUAL(1, i);
      CHECK_EQUAL(std::string("2"), d.value);
    }

    //*************************************************************************
    template <typename T> std::string IdentifyType() { return "Unknown type"; }
    template <> std::string IdentifyType<int>()      { return "int"; }
    template <> std::string IdentifyType<double>()   { return "double"; }
    template <> std::string IdentifyType<Data>()     { return "Data"; }

    TEST(test_tuple_element)
    {
      using Tuple = etl::tuple<int, double, int, Data>;

      CHECK_EQUAL((IdentifyType<int>()),    (IdentifyType<etl::tuple_element_t<0U, Tuple>>()));
      CHECK_EQUAL((IdentifyType<double>()), (IdentifyType<etl::tuple_element_t<1U, Tuple>>()));
      CHECK_EQUAL((IdentifyType<int>()),    (IdentifyType<etl::tuple_element_t<2U, Tuple>>()));
      CHECK_EQUAL((IdentifyType<Data>()),   (IdentifyType<etl::tuple_element_t<3U, Tuple>>()));
    }

    //*************************************************************************
    ETL_NODISCARD bool Get()
    {
      return true;
    }

    TEST(test_ignore_t)
    {
      // Should compiler without error.
      etl::ignore = Get();
    }

    ////*************************************************************************
    //TEST(test_static_assert)
    //{
    //  etl::tuple<int, double, int, Data> theTuple;

    //  auto result = etl::get<4>(theTuple);
    //}
  }
}
