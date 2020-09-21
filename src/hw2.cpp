//============================================================================
// Name        : hw2.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <map>
#include <iterator>
#include <ios>
#include <fstream>
using namespace std;

/*
 * Compiler part 1 -- Lexical analyzer
 *
 * read in a file of source code, determining if the words are a part
 * of the language (language is made up for project)
 *
 * Remember, this is all one file. Do NOT make seperate .h and .cpp
 * files for class LexAnalyzer
 *
 */

class LexAnalyzer
{
private:
	vector<string> _lexemes;
	vector<string> _tokens;
	map<string, string> _tokenMap;
	//other private methods go here
	bool writeOut(string line);
public:
	LexAnalyzer(istream&);
	void scanFile(istream&, ostream&);
};
// class methods
LexAnalyzer::LexAnalyzer(istream& infile)
{
	//
}
int main()
{
	/*
	 * This works for istream, so this is commented out to keep as a
	 * reference while you work -- make sure to delete before turning
	 * this in
	 */
	// this works for opening istream
	/*filebuf fb;
	fb.open("tokens.txt", std::ios::in);
	istream is(&fb);
	// test out reading
	string temp;
	while(!is.eof())
	{
		getline(is, temp);
		cout << temp << endl;
	}
	fb.close();*/

	filebuf fb;
	fb.open("input.txt", std::ios::in);
	istream is(&fb);
	if(!is.eof()) {cout << "not eof";} // should be this
	else {cout << "is eof";}
	cout << endl;
	return 0;
}
