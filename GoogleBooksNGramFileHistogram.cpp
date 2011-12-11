
#include <ngram/GoogleBooksNGramParser.hpp>
#include <map>
#include <fstream>
#include <iterator>
#include <cctype>
#include <iostream>

class MyParser : public ngram::GoogleBooksNGramParser<std::istreambuf_iterator<char> >
{
	protected:
		
		void DetectEntry(const std::string & gram, const unsigned long int year, const unsigned long int matchCount, const unsigned long int pageCount, const unsigned long int volumeCount)
		{
			std::map<unsigned long int, unsigned long int>::iterator iter = counts.find(volumeCount);
			if (iter == counts.end())	counts.insert(std::pair<unsigned long int, unsigned long int>(volumeCount, 1));
			else counts.insert(std::pair<unsigned long int, unsigned long int>(volumeCount, iter->second+1));
		}

	public:

		void ResetCounts()
		{
			counts.clear();
		}

		std::map<unsigned long int, unsigned long int>::const_iterator beginCounts()const
		{
			return counts.begin();
		}

		std::map<unsigned long int, unsigned long int>::const_iterator endCounts() const
		{
			return counts.end();
		}

	private:

		std::map<unsigned long int, unsigned long int> counts;
};

int main(const int argc, const char* argv[])
{
	MyParser parser;
	parser.ResetCounts();
	std::istreambuf_iterator<char> eos;

	for (int currentArgIndex = 0; currentArgIndex < argc; currentArgIndex++)
	{
		std::fstream file(argv[currentArgIndex]);
		std::istreambuf_iterator<char> iter(file.rdbuf());

		parser.Parse(iter, eos);
	}

	for (std::map<unsigned long int, unsigned long int>::const_iterator iter = parser.beginCounts(); iter != parser.endCounts(); ++iter)
	{
		std::cout << iter->first << "\t" << iter->second << std::endl;
	}

	return 0;
}

