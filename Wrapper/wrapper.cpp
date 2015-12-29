#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <Windows.h>
using namespace std;

void exitWithHelp()
{
	cout << "Usage: wrapper.exe input_path" << endl;
	exit(1);
}

void execute(string executable, string path)
{
	string command = path;
	replace(command.begin(), command.end(), '\\', '/');
	command += "/";
	command += executable;
	system(command.c_str());
}

int main(int argc, char* argv[])
{
	cout << "Wrapper: " << endl;

	if (argc != 2)//it should be exact 1
	{
		exitWithHelp();
	}
	
	string inputPath = argv[1];
	replace(inputPath.begin(), inputPath.end(), '\\', '/');
	
	char currentPathCString[256];
	GetCurrentDirectory(sizeof(currentPathCString), currentPathCString);
	string currentPath = currentPathCString;
	
	execute("FeatureExtracter.exe \"" + inputPath + "\" train.dat test.dat", currentPath);
	execute("svm-train.exe -b 1 -c 20 -t 1 -d 3 train.dat model", currentPath);
	execute("svm-predict.exe -b 1 test.dat model predict", currentPath);
	execute("JsonWriter.exe \"" + currentPath + "/predict\" devel01.json", currentPath);
	
	return 0;
}
