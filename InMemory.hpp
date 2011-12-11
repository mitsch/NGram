

#ifndef __NGRAM_INMEMORY_HPP__
#define __NGRAM_INMEMORY_HPP__

#include <ngram/NGram.hpp>

namespace ngram
{
	
	template <class CharT, class CountT=unsigned long int>
	class InMemory : public NGram
	{
		
		private:

			std::map<std::basic_string<CharT>, CountT> grams;
		
		public:

			InMemory(const std::map<std::basic_string<CharT>, CountT> & grams) :
				grams(grams)
			{}

			CountT GetCount(const std::basic_string<CharT> & gram) const
			{
				std::map<std::basic_string<CharT>, CountT>::iterator iter = gram.find(gram);
				if (iter == gram.end()) return 0;
				else return iter->second;
			}

			std::vector<CountT> GetCounts(const std::vector<std::basic_string<CharT>> & grams) const
			{
				std::map<std::basic_string<CharT>, CountT>::iterator gramIter;
				std::vector<CountT> counts;
				counts.reserve(grams.size());
				for (std::vector<std::basic_string<CharT>>::const_iterator iter = grams.begin(); iter != grams.end(); ++iter)
				{
					gramIter = grams.find(*iter);
					if (iter == grams.end()) counts.push_back(0);
					else counts.push_back(iter->second);
				}
				return counts;
			}

			CountT GetTotal() const
			{
				return grams.size();
			}

	};

}

#endif

