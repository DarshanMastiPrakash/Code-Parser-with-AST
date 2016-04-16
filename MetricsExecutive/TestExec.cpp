
/////////////////////////////////////////////////////////////////////
// TestExec.cpp - test layer					                   //
// ver 1.0                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Author:      Darshan Masti Prakash, Syracuse University,        //
//				dmastipr@syr.edu								   //
/////////////////////////////////////////////////////////////////////
/*
Test layer to automate the testing of SemiExp and Tokenizer packages
*/
/*
*  Public Interface
*  ----------------
//main entry for the program
// collecting tokens from files, named on the command line
if (argc < 2)
{
std::cout<< "\n Please enter name of file/directory to process on command line\n\n";
return 1;
}
std::string fileSpec = "";
TestExec test;
test.Req1();
fileSpec = FileSystem::Path::getFullFileSpec(argv[1]);
std::string msg = "Processing files from the file/directory : " + fileSpec;
Util::title(msg);
std::vector<std::string> pattColl;
for (int i = 2; i < argc; ++i)
{
pattColl.push_back(argv[i]);
putline();
}
test.ExtractFiles(fileSpec,pattColl);
test.Req2();
std::getchar();
return 0;

* Package Operations:
* -------------------
* This package contains different functionalities to meet the requriements and automate the testing
* so that it can be run in future without any supervision
*
* Build Process:
* --------------
* Required Files: Tokenizer.h, Tokenizer.cpp , SemiExp.cpp, SemiExp.h,MetricsAnalysis.h
* Build Command: devenv Project1.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 17 Mar 2016
* - first release of new design
*
*/

#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <fstream>
#include "../FileManager/FileMgr.h"
#include"../FileManager/DataStore.h"
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../Parser/Parser.h"
#include "../RulesAndActions/RulesAndAction.h"
#include "../Parser/ConfigureParser.h"
#include "TestExec.h"
#include"../MetricsAnalysis/Analysis.h"
#define Util StringHelper

using namespace Utilities;
//Class for test exec
class TestExec
{
public:
	TestExec();
	~TestExec();
	void Req1();
	void Req2();
	bool ExtractFiles(std::string, std::vector<std::string>);
private:
	Results *res;
};
TestExec::TestExec()
{
	res = new Results();

}
TestExec::~TestExec()
{
}

//  Requirement 3,4,5,6 is displayed here  
void TestExec::Req1()
{
	std::cout.width(80); std::cout << "Code Parser with Abstract Syntax Tree (AST)" << std::endl;
	std::cout.width(80); std::cout << "-------------------------------------------\n";
	std::cout << "Demonstrating Requirement #3" << "\n";
	std::cout << "SemiExpression package and TestExec packages have been used from Project 1 as can be seen in Visual Studio" << "\n\n";
	std::cout << "Demonstrating Requirement #4" << "\n";
	std::cout << "Parser packages has been used in solution file with option for IRule and IAction interfaces Line#86 ,#97 in Parser.h " << "\n\n";
	std::cout << "Demonstrating Requirement #5" << "\n";
	std::cout << "Rules packages has been used in solution file with name as RulesAndActions package. Please refer RulesAndAction.h for more details on\n";
	std::cout<<  "the Rules and actions defined for global,static and non-static functions and scopes" << "\n";
	std::cout << "Line #85 - RulesAndActions.h Also supports actions for each rule that support building the Abstract Syntax Tree (AST)\n";
	std::cout << "that represents a single file's scope structure and identifies the type and line number extend of each scope." << "\n\n";
	std::cout << "Demonstrating Requirement #6" << "\n";	
}


//  Requirement 7,8,9,10 is displayed here  
void TestExec::Req2()
{	
	std::cout << "Demonstrating Requirement #7" << "\n";
	std::cout << "File Manager package has been created where we support finding files and/or directories in a directory tree rooted at a specified path.\n\n";
	std::cout << "Demonstrating Requirement #8" << "\n";
	std::cout << "Metric Analysis package details and the results are displayed below.\n";
	res->DisplayFunctionMetrics();
	std::cout << "\n\nDemonstrating Requirement #9" << "\n";
	std::cout << "Metrics Executive package has been created and used where the file paramters such as file name and patterns can been passed from the command line arguments\n\n";
	std::cout << "Demonstrating Requirement #10" << "\n";
	std::cout << "TestStub has been created for all packages\n\n";
}

//Extract files
bool TestExec::ExtractFiles(std::string filespec, std::vector<std::string> pattColl)
{
	try
	{
		DataStore ds;FileMgr fm(filespec, ds);
		for (auto patt : pattColl)
			fm.addPattern("*" + patt + "*");		//add pattern to the file
		fm.search();	//search the files pattern 	
		DataStore::iterator iter = ds.begin();bool isFound = false;
		for (auto fs : ds)
		{
			isFound = true;
			ConfigParseToConsole configure;std::cout << "\n\nFile processed : " << "\n";
			std::cout << "-----------------------\n";std::cout << fs << "\n";
			Parser* pParser = configure.Build();
			try
			{
				if (pParser)
				{
					if (!configure.Attach(fs))
						std::cout << "\n  could not open file " << filespec << std::endl;
				}
				else
					std::cout << "\n\n  Parser not built\n\n";				//return 1;
				while (pParser->next()) 			// now that parser is built, use it
					pParser->parse();
				std::cout << "\n";std::cout << "\n\nTree structure" << "\n";
				std::cout << "-------------------";
				Node* root = configure.DisplayAST(); //Display AST
				res->setresults(root, fs); //add the tree results into metrics analysis
			}
			catch (std::exception& ex)
			{
				std::cout << "\n\n    " << ex.what() << "\n\n";
			}
		}
		if (isFound == false)
		{
			std::cout << "Files with the specified pattern not found: \n";
			for (auto patt : pattColl)
				std::cout << "Pattern : " << patt << std::endl;
		}
		std::cout << "\n\n";
		return isFound;
	}
	catch (std::exception ex)
	{
		std::cout << "Error in TestExec" << std::endl;
	}
}


#ifdef  TEST_METRICSEXEC
int main(int argc, char* argv[])
{
	//main entry for the program
	//collecting tokens from files, named on the command line
	if (argc < 2)
	{
		std::cout<< "\n Please enter name of file/directory to process on command line\n\n";
		return 1;
	}
	std::string fileSpec = "";
	TestExec test;
	test.Req1();
	fileSpec = FileSystem::Path::getFullFileSpec(argv[1]);
	std::string msg = "Processing files from the file/directory : " + fileSpec;
	Util::title(msg);
	std::vector<std::string> pattColl;
	for (int i = 2; i < argc; ++i)
	{
		pattColl.push_back(argv[i]);	    		
		putline();
	}
	bool fileStat = test.ExtractFiles(fileSpec,pattColl);
	if (fileStat)
	{
		test.Req2();
	}
	std::getchar();
	return 0;
}

#endif //  TEST_METRICSEXEC