

#ifndef __NGRAM_NGRAM_HPP__
#define __NGRAM_NGRAM_HPP__

#include <string>
#include <vector>

namespace ngram
{
	
	template <class CharT, class CountT=unsigned long int>
	class NGram
	{
		public:
			virtual ~NGram() {}

			virtual CountT GetCount(const std::basic_string<CharT> & gram)const=0;
			virtual std::vector<CountT> GetCounts(const std::vector<std::basic_string<CharT>> & grams)const=0;
			virtual CountT GetTotal()const=0;
	};

}

#endif

