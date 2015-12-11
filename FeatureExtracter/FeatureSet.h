//written by Fabian D.
#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <map>
using namespace std;

typedef unsigned int uint;
#define MAX_WORD_LENGTH 30

class FeatureSet
{
public:
	FeatureSet(string functionWordFile);
	~FeatureSet();
	string text;
	uint textFileNumber;
	uint candidateNumber;

	void extractFeatures();
	void print(fstream& outTextStream);
	static bool isPunctationCharacter(char c);
	static bool isSpaceCharacter(char c);
	static bool cleanString(string& str);

	double blankLineFrequency;
	uint numberOfCharacters, charsInWords, alphCharsInWords, upperCharsInWords, digitCharsInWords,
		whiteSpaceChars, spaceChars, tabSpaceChars, punctuationChars;
	double averageWordLength, averageSentenceLength;
	double vocabularyRichness;
	uint numberOfWords, numberOfDistinctWords, numberOfShortWords, totalNumberOfFunctionWords, hapaxLegomena;
	uint wordLengthFrequencyDistribution[MAX_WORD_LENGTH];
	uint numberOfSentences;
	map<string, uint> functionWords;

private:
	uint lineCounter;
	uint blankLineCounter;
	void clean();
	void getAverageSentenceLength();
	void getAverageWordLength();
	void getNumberOfCharacters();
	void getVocabularyRichness();
	void ReadFunctionWordFile(string functionWordFile);
};

