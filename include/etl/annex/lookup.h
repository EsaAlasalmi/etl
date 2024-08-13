/**
  @file lookup.h
  @brief simple key-value lookup (value is string)
  @author R2/April2024

  annexation to the Embedded Template Library
*/


#ifndef ETL_LOOKUP_INCLUDED
#define ETL_LOOKUP_INCLUDED

#include "../platform.h"
#include "../algorithm.h"
#include "../functional.h"
#include "../vector.h"
#include "../type_traits.h"

#include <stdint.h>

namespace etl
{
	namespace private_lookup
	{
		/**
			@brief proxy types for generic lookup
		*/
		template <typename TKey,typename TValue>
		struct lookup_traits
		{
			typedef TKey key_t;
			typedef TValue value_t;
		};

	}

	/**
		@brief template for a lookup structure
	*/
	template <size_t TCapacity, typename TKey, typename TValue>
	class lookup
		: public private_lookup::lookup_traits<TKey,TValue>
		, public etl::binary_function<TKey,TValue,void>
	{
	private:
		typedef typename private_lookup::lookup_traits<TKey,TValue>::key_t key_t;
		typedef typename private_lookup::lookup_traits<TKey,TValue>::value_t value_t;
		typedef typename etl::vector<TKey,TCapacity>::iterator key_iterator_t;
		typedef typename etl::vector<TValue,TCapacity>::iterator value_iterator_t;

	public:
		/**
			@brief default Constructor
		*/
		lookup()
		{
			clear();
		}

		/**
			@brief pair iterator Constructor

			iterator must refer to a container of pairs
		*/
		template <typename TPairIterator>
		lookup(TPairIterator first, TPairIterator last)
		{
			clear();
			while( first != last )
			{
				insert(first->first,first->second);
				first++;
			}
		}
		/**
			@brief Insert a key and the corresponding value
		*/
		void insert(TKey key,TValue value)
		{
			vectorKey.push_back(key);
			vectorValue.push_back(value);
		}

		/**
			@brief Insert a key-value pair
		*/
		void insert(etl::pair<TKey,TValue> kv )
		{
			vectorKey.push_back(kv.first);
			vectorValue.push_back(kv.second);
		}


		/**
			@brief general get
		*/
		template <typename TKeyIterator,typename TValueIterator>
		TValueIterator get(const TKey key)
		{
			TKeyIterator it = etl::find(vectorKey.begin(),vectorKey.end(),key);
			if(it!=vectorKey.end())
			{
				return(vectorValue.begin()+distance(vectorKey.begin(),it));
			}
			return(vectorValue.end());
		}

		/**
			@brief general get
		*/
		value_iterator_t get(const key_t key)
		{
			key_iterator_t it = etl::find(vectorKey.begin(),vectorKey.end(),key);
			if(it!=vectorKey.end())
			{
				return(vectorValue.begin()+distance(vectorKey.begin(),it));
			}
			return(vectorValue.end());
		}



		/**
			@brief get key index w/ linear search
		*/
		ptrdiff_t get_index(const key_t key)
		{
			key_iterator_t it = etl::find(vectorKey.begin(),vectorKey.end(),key);
			if(it!=vectorKey.end())
			{
				return(static_cast<ptrdiff_t>(distance(vectorKey.begin(),it)));
			}
			return(-1); //should use etl value for npos

		}

		/**
			@brief get key index w/ binary search
		*/
		ptrdiff_t get_index_binary(const key_t key)
		{
			key_iterator_t it = etl::binary_find(vectorKey.begin(),vectorKey.end(),key);
			if(it!=vectorKey.end())
			{
				return(static_cast<ptrdiff_t>(distance(vectorKey.begin(),it)));
			}
			return(-1); //should use etl value for npos

		}


		/**
			@brief get value
		*/
		value_t& get_value(const key_t key)
		{
			key_iterator_t it = etl::find(vectorKey.begin(),vectorKey.end(),key);
			if(it!=vectorKey.end())
			{
				return(vectorValue[distance(vectorKey.begin(),it)]);
			}
			return(vectorValue[distance(vectorKey.begin(),it)]);
		}

		value_t& get_value_binary(const key_t key)
		{
			key_iterator_t it = etl::binary_find(vectorKey.begin(),vectorKey.end(),key);
			if(it!=vectorKey.end())
			{
				return(vectorValue[distance(vectorKey.begin(),it)]);
			}
			return(vectorValue[distance(vectorKey.begin(),it)]);
		}


		const value_t& operator[](const size_t index)
		{
			return(vectorValue[index]);
		}

		const value_t& operator[](const size_t index) const
		{
			return(vectorValue[index]);
		}


		/**
			@brief Get the number of inserted values
		*/		
		size_t count() const
		{
			return(size_t(vectorKey.size()));
		}

		/**
			@brief Get the number of inserted values
		*/		
		size_t size() const
		{
			return(size_t(vectorKey.size()));
		}


		/**
			@brief Clear the calculation
		*/
		void clear()
		{
			vectorKey.clear();
			vectorValue.clear();
		}

		#if 1
		void dump( void )
		{
			for(size_t i=0;i<vectorKey.size();i++)
			{
				printf("@%u : %u %s\n",i,vectorKey[i],vectorValue[i].c_str());
			}
		}
		#endif
		#if 1
		void dump2( void )
		{
			for(size_t i=0;i<vectorKey.size();i++)
			{
				printf("@%u : %s %s\n",i,vectorKey[i].c_str(),vectorValue[i].c_str());
			}
		}
		#endif
	private:
		etl::vector<TKey,TCapacity> vectorKey;
		etl::vector<TValue,TCapacity> vectorValue;
	};
}

#endif

