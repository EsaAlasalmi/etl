/**
  @file median.h
  @brief simple median calculation  
  @author R2/April2024

  annexation for the Embedded Template Library
*/


#ifndef ETL_MEDIAN_INCLUDED
#define ETL_MEDIAN_INCLUDED

#include "../platform.h"
#include "../functional.h"
#include "../vector.h"
#include "../type_traits.h"

#include <stdint.h>

namespace etl
{
	namespace private_median
	{
		/**
			@brief Types for generic median
		*/
		template <typename TInput, typename TCalc>
		struct median_traits
		{
			typedef TCalc calc_t;
		};

		/**
			@brief Types for float median
		*/
		template <typename TCalc>
		struct median_traits<float, TCalc>
		{
			typedef float calc_t;
		};

		/**
			@brief Types for double median
		*/
		template <typename TCalc>
		struct median_traits<double, TCalc>
		{
			typedef double calc_t;
		};
	}

	/**
		@brief template for calculating (numeric) median
	*/
	template <typename TInput, size_t TInputMax, typename TCalc = TInput>
	class median
		: public private_median::median_traits<TInput, TCalc>
		, public etl::binary_function<TInput, TInput, void>
	{
	private:
		typedef typename private_median::median_traits<TInput, TCalc>::calc_t calc_t;

	public:
		/**
			@brief Constructor
		*/
		median()
		{
			clear();
		}

		/**
			@brief Constructor
		*/
		template <typename TIterator>
		median(TIterator first, TIterator last)
		{
			clear();
			insert(first, last);
		}



		/**
			@brief Insert a value
		*/
		void insert(TInput value)
		{
			vectorInput.push_back(value);
			++counter;
			recalculate = true;
		}

		/**
			@brief Insert a range
		*/
		template <typename TIterator>
		void insert(TIterator first, TIterator last)
		{
			while (first != last)
			{
				insert(*first);
				++first;
			}
		}

		/**
			@brief Insert a value
		*/
		void operator ()(TInput value)
		{
			insert(value);
		}

		/**
			@brief Insert a range
		*/
		template <typename TIterator>
		void operator ()(TIterator first, TIterator last)
		{
			insert(first, last);
		}

		/**
			@brief Retrieve the median value
		*/
		double get_median() const
		{
			if (recalculate)
			{
				median_value = 0.0;

				if (counter != 0) /* median remains zero if there are no elements */
				{
					if(vectorInput.size()%2) /* odd number of elements */
					{
						/* median is the middle element */
						median_value = double(vectorInput[vectorInput.size()/2] );
					}
					else /* even number of elements */
					{
						/* median is the average of the two middle elements */
						median_value = double( ( vectorInput[(vectorInput.size()/2)-1] + vectorInput[(vectorInput.size()/2)+0] ) / 2.0 );
					}
				}

				recalculate = false;
			}

			return(median_value);
		}

		/**
			@brief Retrieve the median value
		*/
		operator double() const
		{
			return(get_median());
		}

		operator uint32_t() const
		{
			return((uint32_t)get_median());
		}

		/**
			@brief Get the number of inserted values
		*/		
		size_t count() const
		{
			return(size_t(counter));
		}

		/**
			@brief Clear the calculation
		*/
		void clear()
		{
			vectorInput.clear();
			counter     = 0;
			median_value  = 0.0;
			recalculate = true;
		}

	private:
		etl::vector<TInput,TInputMax> vectorInput;
		uint32_t counter;
		mutable double median_value;
		mutable bool   recalculate;
	};


}

#endif

