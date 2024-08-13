/**
  @file median_element.h
  @brief simple median element picker
  @author R2/April2024

  annexation to the Embedded Template Library
*/


#ifndef ETL_MEDIAN_ELEMENT_INCLUDED
#define ETL_MEDIAN_ELEMENT_INCLUDED

#include "../platform.h"
#include "../functional.h"
#include "../vector.h"
#include "../type_traits.h"

#include <stdint.h>

namespace etl
{
	namespace private_median_element
	{
		/**
			@brief Types for generic median
		*/
		//template <typename TInput, typename TCalc>
		template <typename TCalc>
		struct median_traits
		{
			typedef TCalc calc_t;
		};

	}

	/**
		@brief template for picking the median element
	*/
	template <typename TSampleset, size_t TInputCount, typename TCalc>
	class median_element
		//: public private_median_element::median_traits<TElement, TCalc>
		: public private_median_element::median_traits<TCalc>
	{
	private:
		typedef typename private_median_element::median_traits<TCalc>::calc_t calc_t;

	public:
		/**
			@brief Constructor
		*/
		median_element()
		{
			clear();
		}

		median_element( TSampleset input )
		{
			median_elm=TCalc(input[(TInputCount-1)/2]);
		}

		#if 0
		/**
			@brief Constructor
		*/
		template <typename TIterator>
		median_element(TIterator first, TIterator last)
		{
			clear();
			insert(first, last);
		}
		#endif



		/**
			@brief Retrieve the median value
		*/

		operator uint32_t() const
		{
			return(median_elm);
		}

		/**
			@brief Get the number of inserted values
		*/		
		size_t count() const
		{
			return(TInputCount);
		}

		/**
			@brief Clear the calculation
		*/
		void clear()
		{
			counter     = 0;
			median_elm  = TCalc(0);
			recalculate = true;
		}

	private:
		uint32_t counter;
		mutable TCalc median_elm;
		mutable bool   recalculate;
	};


}

#endif

