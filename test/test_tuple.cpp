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

#include "data.h"

#include "etl/tuple.h"

#include <string>

namespace
{
  using Data = TestDataDC<std::string>;

  SUITE(test_tuple)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      etl::tuple<int, double, int, Data> data;

      int i0          = etl::get<0>(data);
      double d1       = etl::get<1>(data);
      int i2          = etl::get<2>(data);
      std::string& s3 = etl::get<3>(data).value;

      CHECK_EQUAL(0, i0);
      CHECK_EQUAL(0, d1);
      CHECK_EQUAL(0, i2);
      CHECK_TRUE(s3.empty());
    }

    //*************************************************************************
    TEST(test_constructor)
    {
      etl::tuple<int, double, int, Data> data(1, 2.2, 3, Data("4"));

      int i0          = etl::get<0>(data);
      double d1       = etl::get<1>(data);
      int i2          = etl::get<2>(data);
      std::string& s3 = etl::get<3>(data).value;

      CHECK_EQUAL(1, i0);
      CHECK_EQUAL(2.2, d1);
      CHECK_EQUAL(3, i2);
      CHECK_EQUAL(std::string("4"), s3);
    }

#if ETL_USING_CPP17
    //*************************************************************************
    TEST(test_constructor_using_template_deduction_guideline)
    {
      etl::tuple data(1, 2.2, 3, Data("4"));

      int i0          = etl::get<0>(data);
      double d1       = etl::get<1>(data);
      int i2          = etl::get<2>(data);
      std::string& s3 = etl::get<3>(data).value;

      CHECK_EQUAL(1, i0);
      CHECK_EQUAL(2.2, d1);
      CHECK_EQUAL(3, i2);
      CHECK_EQUAL(std::string("4"), s3);
    }
#endif

    //*************************************************************************
    TEST(test_static_assert)
    {
      etl::tuple<int, double, int, Data> data;

      //auto result = etl::get<4>(data);
    }
  }
}
