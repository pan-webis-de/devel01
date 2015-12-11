//Written by Fabian D.
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <map>
#include "Reader.h"
#include "Jzon.h"
#include "FeatureSet.h"
#include "SVMFile.h"
using namespace std;

typedef unsigned int uint;

#define TEXT_FOLDER							"pan11large-v2/"
#define RELATIVE_TEXT_FILE_PATH				"./Texts/NEW CORPORA/"
#define RELATIVE_FUNCTION_WORD_FILE_PATH	"./Texts/"
#define FUNCTION_WORD_FILE					"functionWords184.txt"
//#define FUNCTION_WORD_FILE				"functionWords319.txt"
#define JSON_FILE							"meta-file.json"

struct answerList
{
	string unknown_text;
	string author;
	float score;

	answerList* nextAnswer;
};

void processAnswers(answerList* llist)
{
	Jzon::Object root;
	Jzon::Array answers;

	while (llist)
	{
		Jzon::Object answerOne;
		answerOne.Add("unknown_text", llist->unknown_text);
		answerOne.Add("author", llist->author);
		answerOne.Add("score", llist->score);

		answers.Add(answerOne);
		llist = llist->nextAnswer;
	}

	root.Add("answers", answers);

	Jzon::FileWriter::WriteFile("results.json", root, Jzon::StandardFormat);
}

struct candidateAuthors
{
	string author_name;
	candidateAuthors* next;
};

struct unknownTexts
{
	string unknown_text;
	unknownTexts* next;
};

struct inputMetadata
{
	string unknown_folder;
	string language;
	string encoding;

	candidateAuthors* candidates;
	unknownTexts* unknown;
};

inputMetadata* readMetadata(string path)
{
	Jzon::Object rootNode;
	Jzon::FileReader::ReadFile(path, rootNode);

	inputMetadata* result = new inputMetadata();

	result->unknown_folder = rootNode.Get("folder").ToString();
	result->language = rootNode.Get("language").ToString();
	result->encoding = rootNode.Get("encoding").ToString();
	result->candidates = 0;
	result->unknown = 0;

	candidateAuthors* tempA = 0;
	unknownTexts* tempU = 0;

	const Jzon::Array &candidates = rootNode.Get("candidate-authors").AsArray();
	for (Jzon::Array::const_iterator it = candidates.begin(); it != candidates.end(); ++it)
	{
		candidateAuthors* nextAuthor = new candidateAuthors();

		Jzon::Object obj = (*it).AsObject();

		nextAuthor->author_name = obj.Get("author-name").ToString();
		nextAuthor->next = 0;

		if (tempA)
		{
			tempA->next = nextAuthor;
		}

		tempA = nextAuthor;

		if (!result->candidates)
		{
			result->candidates = tempA;
		}
	}

	const Jzon::Array &unknown = rootNode.Get("unknown-texts").AsArray();
	for (Jzon::Array::const_iterator it = unknown.begin(); it != unknown.end(); ++it)
	{
		unknownTexts* nextText = new unknownTexts();

		Jzon::Object obj = (*it).AsObject();

		nextText->unknown_text = obj.Get("unknown-text").ToString();
		nextText->next = 0;

		if (tempU)
		{
			tempU->next = nextText;
		}

		tempU = nextText;

		if (!result->unknown)
		{
			result->unknown = tempU;
		}
	}

	return result;
}

void exitWithHelp()
{
	cout << "Usage: FeatureExtracter.exe input_path output_train_file output_test_file" << endl;
	exit(1);
}

uint bodyCounter = 0;

int main(int argc, char* argv[])
{
	cout << "FeatureExtracter: " << endl;

	if (argc != 4)//it should be exact 3 
	{
		exitWithHelp();
	}

	string inputPath = argv[1];

	fstream testInputPath;
	testInputPath.open((inputPath + '/' + JSON_FILE).c_str());
	if (!testInputPath.good())
	{
		cout << "cannot open " << JSON_FILE << " in input_path" << endl;
	}
	testInputPath.close();


	Reader reader(inputPath);
	string nextText;

	FeatureSet featureSet(FUNCTION_WORD_FILE);
	
	SVMFile svmTrainFile(argv[2]);
	SVMFile svmTestFile(argv[3]);

	cout << "read candidate texts...";
	while (reader.getNextCandidateText(featureSet.text, featureSet.candidateNumber, featureSet.textFileNumber))
	{
		bodyCounter++;
		if (bodyCounter % 10 == 0)
		{
			cout << bodyCounter << "...";
		}

		featureSet.extractFeatures();
		svmTrainFile.writeFeatureSetToFile(featureSet);
	}
	cout << bodyCounter << endl;
	
	cout << "read unknown texts...";
	bodyCounter = 0;
	featureSet.candidateNumber = 0;
	while (reader.getNextUnknownText(featureSet.text, featureSet.textFileNumber))
	{
		bodyCounter++;
		if (bodyCounter % 10 == 0)
		{
			cout << bodyCounter << "...";
		}

		featureSet.extractFeatures();

		svmTestFile.writeFeatureSetToFile(featureSet);
	}

	return 0;
}
