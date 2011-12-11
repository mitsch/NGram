
#ifndef __NGRAM_GOOGLEBOOKSNGRAMPARSER_HPP__
#define __NGRAM_GOOGLEBOOKSNGRAMPARSER_HPP__

#include <cctype>
#include <string>

namespace ngram
{
	
	template <class InputIterator>
	class GoogleBooksNGramParser
	{

		protected:

			virtual void DetectCorruptLine(const unsigned int line, const std::string & description)
			{}

			virtual void DetectEntry(const std::string & gram, const unsigned long int year, const unsigned long int matchCount,
				const unsigned long int pageCount, const unsigned long int volumeCount)
			{}
	
		public:

			int test;

			virtual ~GoogleBooksNGramParser() {}
		
			void Parse(InputIterator & begin, InputIterator & end)
			{
				enum {
					BEGIN_OF_NEWLINE,
					READ_GRAM,
					TAB_AFTER_GRAM,
					READ_YEAR,
					TAB_AFTER_YEAR,
					READ_MATCH_COUNT,
					TAB_AFTER_MATCH_COUNT,
					READ_PAGE_COUNT,
					TAB_AFTER_PAGE_COUNT,
					READ_VOLUME_COUNT,
					NEWLINE,
					GO_TO_NEWLINE
				} state = BEGIN_OF_NEWLINE;
				
				unsigned int line = 0;
				std::string gram;
				unsigned long int year = 0;
				unsigned long int matchCount = 0;
				unsigned long int pageCount = 0;
				unsigned long int volumeCount = 0;

				for (InputIterator current = begin; current != end; ++current)
				{
					switch (state)
					{						
						case BEGIN_OF_NEWLINE:
							if (*current != '\t' && *current != '\n') {gram += *current; state = READ_GRAM;}
							else {DetectCorruptLine(line, "1st field (gram) contains some invalid character"); state = GO_TO_NEWLINE;}
							break;
						case READ_GRAM:
							if (*current != '\t' && *current != '\n') gram += *current;
							else if (*current == '\t') state = TAB_AFTER_GRAM;
							else {DetectCorruptLine(line, "1st field (gram) contains some invalid character"); state = GO_TO_NEWLINE;}
							break;
						case TAB_AFTER_GRAM:
							if (std::isdigit(*current)) {year = (year*10)+(*current-0x30); state = READ_YEAR;}
							else {DetectCorruptLine(line, "2nd field (year) contains some invalid character"); state = GO_TO_NEWLINE;}
							break;
						case READ_YEAR:
							if (std::isdigit(*current)) year = (year*10)+(*current-0x30);
							else if (*current == '\t') state = TAB_AFTER_YEAR;
							else {DetectCorruptLine(line, "2nd field (year) contains some invalid character"); state = GO_TO_NEWLINE;}
							break;
						case TAB_AFTER_YEAR:
							if (std::isdigit(*current)) {matchCount = (matchCount*10)+(*current-0x30);state = READ_MATCH_COUNT;}
							else {DetectCorruptLine(line, "3rd field (match count) contains some invalid character"); state = GO_TO_NEWLINE;}
							break;
						case READ_MATCH_COUNT:
							if (std::isdigit(*current)) matchCount = (matchCount*10)+(*current-0x30);
							else if (*current == '\t') {state = TAB_AFTER_MATCH_COUNT;}
							else {DetectCorruptLine(line, "3rd field (match count) contains some invalid character"); state = GO_TO_NEWLINE;}
							break;
						case TAB_AFTER_MATCH_COUNT:
							if (std::isdigit(*current)) {pageCount = (pageCount*10)+(*current-0x30); state = READ_PAGE_COUNT;}
							else {DetectCorruptLine(line, "4th field (page count) contains some invalid character"); state = GO_TO_NEWLINE;}
							break;
						case READ_PAGE_COUNT:
							if (std::isdigit(*current)) pageCount = (pageCount*10)+(*current-0x30);
							else if (*current == '\t') {state = TAB_AFTER_PAGE_COUNT;}
							else {DetectCorruptLine(line, "4th field (page count) contains some invalid character"); state = GO_TO_NEWLINE;}
							break;
						case TAB_AFTER_PAGE_COUNT:
							if (std::isdigit(*current)) {volumeCount = (volumeCount*10)+(*current-0x30); state = READ_VOLUME_COUNT;}
							else {DetectCorruptLine(line, "5th field (volume count) contains some invalid character"); state = GO_TO_NEWLINE;}
							break;
						case READ_VOLUME_COUNT:
							if (std::isdigit(*current)) volumeCount = (volumeCount*10)+(*current-0x30);
							else if (*current == '\n')
							{
								DetectEntry(gram, year, matchCount, pageCount, volumeCount);
								gram.clear();
								year = 0;
								matchCount = 0;
								pageCount = 0;
								volumeCount = 0;
								state = BEGIN_OF_NEWLINE;
							}
							else {DetectCorruptLine(line, "5th field (volume count) contains some invalid character"); state = GO_TO_NEWLINE;}
							break;
						case GO_TO_NEWLINE:
							if (*current == '\n') state = BEGIN_OF_NEWLINE;
							break;
						default: throw "this branch should never be reached!!!";
					}
				}
				if (state != BEGIN_OF_NEWLINE)
					DetectCorruptLine(line, "end of line unexpected");
			}	
		
	};

}

#endif

