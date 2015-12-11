//written by Fabian D.
#include "SVMFile.h"


SVMFile::SVMFile(string svmWriteFile)
{
	outSVMStream.open(svmWriteFile.c_str(), ios::out);

	if (!outSVMStream.good())
	{
		cout << "cannot open " << svmWriteFile << endl;
		exit(0);
	}
}


SVMFile::~SVMFile()
{

}

void SVMFile::writeFeatureSetToFile(FeatureSet& featureSet)
{
	outSVMStream << featureSet.candidateNumber;

	//Number of blank lines/total number of lines
	if (featureSet.blankLineFrequency)
	{
		outSVMStream << " 1:" << featureSet.blankLineFrequency;
	}

	//Average sentence length
	if (featureSet.averageSentenceLength)
		outSVMStream << " 2:" << featureSet.averageSentenceLength;// / (double)numberOfWords; //(double)100; 

	//Average word length (number of characters)
	if (featureSet.averageWordLength)
		outSVMStream << " 3:" << featureSet.averageWordLength;// / (double)numberOfCharacters;//(double)100;

	//Vocabulary richness i.e., V / M
	if (featureSet.vocabularyRichness)
		outSVMStream << " 4:" << featureSet.vocabularyRichness;

	//Total number of function words / M
	if (featureSet.totalNumberOfFunctionWords)
	{
		outSVMStream << " 5:" << featureSet.totalNumberOfFunctionWords / (double)featureSet.numberOfWords;
	}

	//Total number of short words / M
	if (featureSet.numberOfShortWords)
		outSVMStream << " 6:" << featureSet.numberOfShortWords / (double)featureSet.numberOfWords;

	//Count of hapax legomena / M
	if (featureSet.hapaxLegomena)
		outSVMStream << " 7:" << featureSet.hapaxLegomena / (double)featureSet.numberOfWords;

	//Count of hapax legomena / V
	if (featureSet.hapaxLegomena)
		outSVMStream << " 8:" << featureSet.hapaxLegomena / (double)featureSet.numberOfDistinctWords;

	//Total number of characters in words / C
	if (featureSet.charsInWords)
		outSVMStream << " 9:" << featureSet.charsInWords / (double)featureSet.numberOfCharacters;

	//Total number of alphabetic characters in words / C
	if (featureSet.alphCharsInWords)
		outSVMStream << " 10:" << featureSet.alphCharsInWords / (double)featureSet.numberOfCharacters;

	//Total number of upper-case characters in words / C
	if (featureSet.upperCharsInWords)
		outSVMStream << " 11:" << featureSet.upperCharsInWords / (double)featureSet.numberOfCharacters;

	//Total number of digit characters in words / C
	if (featureSet.digitCharsInWords)
		outSVMStream << " 12:" << featureSet.digitCharsInWords / (double)featureSet.numberOfCharacters;

	//Total number of white-space characters / C
	if (featureSet.whiteSpaceChars)
		outSVMStream << " 13:" << featureSet.whiteSpaceChars / (double)featureSet.numberOfCharacters;

	//Total number of space characters / C
	if (featureSet.spaceChars)
		outSVMStream << " 14:" << featureSet.spaceChars / (double)featureSet.numberOfCharacters;

	//Total number of space characters / number white-space characters
	if (featureSet.spaceChars)
		outSVMStream << " 15:" << featureSet.spaceChars / (double)featureSet.whiteSpaceChars;

	//Total number of tab spaces / C
	if (featureSet.tabSpaceChars)
		outSVMStream << " 16:" << featureSet.tabSpaceChars / (double)featureSet.numberOfCharacters;

	//Total number of tab spaces / number white-space characters
	if (featureSet.tabSpaceChars)
		outSVMStream << " 17:" << featureSet.tabSpaceChars / (double)featureSet.whiteSpaceChars;

	//Total number of punctuations / C
	if (featureSet.punctuationChars)
		outSVMStream << " 18:" << featureSet.punctuationChars / (double)featureSet.numberOfCharacters;

	//Word length frequency distribution / M(30 features)
	for (uint i = 0; i < MAX_WORD_LENGTH; i++)
	{
		if (featureSet.wordLengthFrequencyDistribution[i])
			outSVMStream << " " << 19 + i << ":" << featureSet.wordLengthFrequencyDistribution[i] / (double)featureSet.numberOfWords;
	}

	//Function word frequency distribution(122 features)
	uint i = 0;
	for (map<string, uint>::iterator it = featureSet.functionWords.begin(); it != featureSet.functionWords.end(); it++, i++)
	{
		if (it->second)
			outSVMStream << " " << 19 + MAX_WORD_LENGTH + i << ":" << it->second;// / (double)numberOfWords;
	}

	//append textFileName and authorID
	outSVMStream << " # candidateNumber = " << featureSet.candidateNumber << ", textFileNumber = " << featureSet.textFileNumber << "\"";

	outSVMStream << endl;
}



