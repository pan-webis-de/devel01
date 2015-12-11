//written by Fabian D.
#include "FeatureSet.h"


FeatureSet::FeatureSet(string functionWordFile)
{
	ReadFunctionWordFile(functionWordFile);
}


FeatureSet::~FeatureSet()
{

}

void FeatureSet::clean()
{
	numberOfWords = 0;
	numberOfDistinctWords = 0;
	numberOfShortWords = 0;
	totalNumberOfFunctionWords = 0;
	hapaxLegomena = 0;
	charsInWords = 0;
	alphCharsInWords = 0;
	upperCharsInWords = 0;
	digitCharsInWords = 0;
	whiteSpaceChars = 0;
	spaceChars = 0;
	tabSpaceChars = 0;
	punctuationChars = 0;
	lineCounter = 1;
	blankLineCounter = 0;
	numberOfSentences = 0;

	//clear mapped values
	for (map<string, uint>::iterator it = functionWords.begin(); it != functionWords.end(); it++)
	{
		it->second = 0;
	}
	//clear array: wordLengthFrequencyDistribution 
	for (uint i = 0; i < MAX_WORD_LENGTH; i++)
	{
		wordLengthFrequencyDistribution[i] = 0;
	}
}

//compute style marker attributes
void FeatureSet::extractFeatures()
{
	clean();
	getNumberOfCharacters();
	getVocabularyRichness();
	getAverageWordLength();
	getAverageSentenceLength();
}

bool FeatureSet::isPunctationCharacter(char c)
{
	if ((c >= '[' && c <= '`')
		|| (c >= '!' && c <= '/')
		|| (c >= '{' && c <= '~')
		|| (c >= ':' && c <= '@'))
	{
		return true;
	}
	return false;
}

bool FeatureSet::isSpaceCharacter(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
	{
		return true;
	}
	return false;
}

//convert upper-case characters to lower-case characters and removes punctations/space in the front and end
//returns false if cleared string is empty
bool FeatureSet::cleanString(string& str)
{
	uint i, j;
	for (i = 0; (i < str.length()) && (isPunctationCharacter(str[i]) || isSpaceCharacter(str[i])); i++);

	if (i == str.length())
	{
		str = "";
		return false;
	}
	for (j = str.length() - 1; (j > i) && (isPunctationCharacter(str[j]) || isSpaceCharacter(str[j])); j--);

	str = str.substr(i, j - i + 1); //removes punctations in the front and end

	//convert upper-case characters to lower-case characters
	for (uint k = 0; k < str.length(); k++)
	{
		if (str[k] >= 'A' && str[k] <= 'Z')
		{
			str[k] = str[k] + ('a' - 'A');
		}
	}
	return true;
}

void FeatureSet::getAverageSentenceLength()
{
	if (numberOfSentences == 0)
	{
		averageSentenceLength = 0;
	}
	else
	{
		averageSentenceLength = numberOfWords / (double)numberOfSentences;
	}
}

void FeatureSet::getAverageWordLength()
{
	if (numberOfWords == 0)
	{
		averageWordLength = 0;
	}
	else
	{
		averageWordLength = charsInWords / (double)numberOfWords;
	}
}

//counts diverse types of characters
void FeatureSet::getNumberOfCharacters()
{
	bool lineFlag = true;
	bool sentenceFlag[4] = { false, false, false, false };

	for (uint i = 0; i < text.length(); i++)
	{
		if (text[i] >= 'a' && text[i] <= 'z')
		{
			alphCharsInWords++;
			lineFlag = false;
			sentenceFlag[i % 4] = true;
		}
		else if (text[i] >= 'A' && text[i] <= 'Z')
		{
			alphCharsInWords++;
			upperCharsInWords++;
			lineFlag = false;
			sentenceFlag[i % 4] = true;
		}
		else if (text[i] >= '0' && text[i] <= '9')
		{
			digitCharsInWords++;
			lineFlag = false;
			sentenceFlag[i % 4] = true;
		}
		else if (text[i] == '.' || text[i] == '!' || text[i] == '?')
		{
			punctuationChars++;
			lineFlag = false;

			if (sentenceFlag[0] && sentenceFlag[1] && sentenceFlag[2] && sentenceFlag[3])
			{
				numberOfSentences++;
			}
			sentenceFlag[i % 4] = false;
		}
		else if (isPunctationCharacter(text[i]))
		{
			punctuationChars++;
			lineFlag = false;
			sentenceFlag[i % 4] = true;
		}
		else if (text[i] == ' ')
		{
			whiteSpaceChars++;
			spaceChars++;
			lineFlag = false;
			sentenceFlag[i % 4] = true;
		}
		else if (text[i] == '\t')
		{
			tabSpaceChars++;
			whiteSpaceChars++;
			lineFlag = false;
			sentenceFlag[i % 4] = true;
		}
		else if (text[i] == '\n' || text[i] == '\r')
		{
			whiteSpaceChars++;
			lineCounter++;
			if (lineFlag == true)
			{
				blankLineCounter++;
			}
			lineFlag = true;
			sentenceFlag[i % 4] = true;
		}

		charsInWords = alphCharsInWords + digitCharsInWords;
	}
	numberOfCharacters = text.length();
	blankLineFrequency = blankLineCounter / (double)lineCounter;
}

//Vocabulary richness (distinct words/totalNumberOfWords) & total Number of short Words
void FeatureSet::getVocabularyRichness()
{
	map<string, uint> uniqueWords;
	uint wordBeginAdress = 0;
	string word;
	bool wordFlag = false; //true, if i is inside a word
	uint i;

	for (i = 0; i < text.length(); i++)
	{
		if (text[i] == '\n' || text[i] == ' ' || text[i] == '\t' || text[i] == '\r' || i == text.length() - 1)
		{
			if (wordFlag)
			{
				//numberOfWords++;
				word = text.substr(wordBeginAdress, i - wordBeginAdress);

				if (cleanString(word))
				{
					if (word.length() < MAX_WORD_LENGTH)
					{
						wordLengthFrequencyDistribution[word.length() - 1]++;

						map<string, uint>::iterator it = functionWords.find(word);
						if (it != functionWords.end())
						{
							it->second++;
							totalNumberOfFunctionWords++;
						}
						it = uniqueWords.find(word);
						if (it != uniqueWords.end())
						{
							it->second++;
						}
						else
						{
							uniqueWords.insert(pair<string, uint>(word, 0));
						}
					}
				}
				wordFlag = false;
			}
		}
		else if (wordFlag == false)
		{
			numberOfWords++;
			wordBeginAdress = i;
			wordFlag = true;
		}
	}
	numberOfShortWords = wordLengthFrequencyDistribution[0] + wordLengthFrequencyDistribution[1] + wordLengthFrequencyDistribution[2];

	//count hapaxLegomena
	for (map<string, uint>::iterator it = uniqueWords.begin(); it != uniqueWords.end(); it++)
	{
		if (it->second == 0)
		{
			hapaxLegomena++;
		}
	}
	numberOfDistinctWords = uniqueWords.size();

	if (numberOfWords)
	{
		vocabularyRichness = numberOfDistinctWords / (double)numberOfWords;
	}
	vocabularyRichness = 0;
}

void FeatureSet::ReadFunctionWordFile(string functionWordFile)
{
	string word;
	fstream functionWordStream;
	functionWordStream.open(functionWordFile.c_str(), ios::in);

	if (!functionWordStream.good())
	{
		cout << "cannot open " << functionWordFile << endl;
		return;
	}

	cout << "read function words: " << functionWordFile << endl;

	while (functionWordStream >> word)
	{
		FeatureSet::cleanString(word);
		functionWords.insert(std::pair<string, uint>(word, 0));
	}
}

//output tyle marker attributes in File
void FeatureSet::print(fstream& outTextStream)
{
	outTextStream << "candidateNumber: " << candidateNumber
		<< ", textFileNumber: " << textFileNumber
		<< ", BlankLines: " << blankLineCounter
		<< ", lineCounter:" << lineCounter
		<< ", Words: " << numberOfWords
		<< ", distinctWords: " << numberOfDistinctWords
		<< ", Chars: " << numberOfCharacters
		<< ", cInWords:" << charsInWords
		<< ", alphChars:" << alphCharsInWords
		<< ", upperChars:" << upperCharsInWords
		<< ", digitChars:" << digitCharsInWords
		<< ", whiteSpace:" << whiteSpaceChars
		<< ", space:" << spaceChars
		<< ", tabSpace:" << tabSpaceChars
		<< ", punctuation:" << punctuationChars
		<< ", sentences: " << numberOfSentences
		<< ", averageSentenceLength: " << averageSentenceLength
		<< ", averageWordLength: " << averageWordLength
		<< ", vocabularyRichness: " << vocabularyRichness
		<< ", numberOfShortWords: " << numberOfShortWords;

	for (map<string, uint>::iterator it = functionWords.begin(); it != functionWords.end(); it++)
	{
		outTextStream << ", " << it->first << ": " << it->second;
	}

	outTextStream << ", wordLengthFrequencyDistribution";
	for (uint i = 0; i < MAX_WORD_LENGTH; i++)
	{
		outTextStream << ", " << i << ": " << wordLengthFrequencyDistribution[i];
	}

	outTextStream << ", totalNumberOfFunctionWords: " << totalNumberOfFunctionWords
		<< ", hapaxLegomena: " << hapaxLegomena
		<< ": " << endl
		<< text << endl << "____________________________________________________" << endl;
}