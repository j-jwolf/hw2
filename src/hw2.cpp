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
 * Main issue so far:
 * 	- The last line in tokens.txt is being read, pushed onto the vector correctly and even being put into its associated string. However, it's not
 * 		being printed out from the map iterator properly. It is in the map though, it can print via _tokenMap["!"]
 * 		-- Fixed, it's sorted so it isn't in the last index, idiot
 *
 * Notes:
 * 	-Ask if we should/can implement a default constructor for LexAnalyzer. It's a bit pointless but it really wouldn't hurt
 * 		--Just  have to remember that you'd need to have a set map method that has the istream reference to read from
 *  -//
 *
 *
 */

class LexAnalyzer
{
private:
	vector<string> _lexemes;
	vector<string> _tokens;
	map<string, string> _tokenMap;
	//other private methods go here
	/*
	 * pre: Called to check if character is a number
	 * post: returns boolean confirming if char is a number
	 *
	 * Determines if a character is a number
	 *
	 * @return true if is a number 0-9, false if not
	 */
	bool isNumber(char letter)
	{
		string nums = "1234567890";
		for(int i = 0; i < 10; i++)
		{
			if(letter == nums[i]) {return true;}
		}
		return false;
	}
	/*
	 * !!! Delete before turning in !!!
	 * This is a test method that prints all of the keys and tokens in the map _tokenMap
	 *
	 * @return null
	 */
	void testPrint()
	{
		cout << "keys" << endl << endl;
		for(std::map<string, string>::iterator mitr = _tokenMap.begin(); mitr != _tokenMap.end(); mitr++)
		{
			/*cout << mitr->first << ", " << mitr->second << endl;
			string line = mitr->first + " " + mitr->second;
			outfile << line << endl;*/
			cout << mitr->first << endl;
		}
		cout << endl << "tokens" << endl << endl;
		for(std::map<string, string>::iterator mitr = _tokenMap.begin(); mitr != _tokenMap.end(); mitr++)
		{
			cout << mitr->second << endl;
		}
	}
	/*
	 * !!! Delete before turning in !!!
	 * This is a test method to write all of the keys and tokens from map to confirm if they were read correctly from file
	 *
	 * @return null
	 */
	void testOut()
	{
		string fn = "outfile_.txt", line;
		ofstream outfile;
		outfile.open(fn);
		outfile << "Keys:" << endl;
		for(std::map<string, string>::iterator mitr = _tokenMap.begin(); mitr != _tokenMap.end(); mitr++)
		{
			line = mitr->first;
			outfile << line << endl;
		}
		line = "";
		outfile << "Tokens:" << endl;
		for(std::map<string, string>::iterator mitr = _tokenMap.begin(); mitr != _tokenMap.end(); mitr++)
		{
			line = _tokenMap[mitr->first];
			outfile << line << endl;
		}
		outfile.close();
	}
public:
	LexAnalyzer(istream&);
	void scanFile(istream&, ostream&);
	void test() {testOut();} // calls for test out, redundant. remove this for easier testing
};
//============================================================ Class Methods =============================================================================
/*
 * pre:
 * post:
 *
 * n-argument constructor for LexAnalyzer class object
 *
 * @param istream& - reference to an open input stream
 *
 * @return null
 */
LexAnalyzer::LexAnalyzer(istream& infile)
{
	string temp;
	if(!infile) {exit(-1);}
	while(getline(infile, temp))
	{
		string token = "", key = "";
		vector<char> tokenvec;
		vector<char> keyvec;
		char index = '0';
		int count = 0, length = temp.length();
		while(count < length && index != ' ')
		{
			index = temp[count];
			if(index != ' ')
			{
				count++;
				tokenvec.push_back(index);
				//cout << index << endl;
			}
		}
		count++;
		index = '0';
		while(count < length && index != ' ')
		{
			index = temp[count];
			count++;
			keyvec.push_back(index);
			//cout << index << endl;
		}
		int tokenLength = tokenvec.size(), keyLength = keyvec.size();
		for(int i = 0; i < tokenLength; i++) {token.push_back(tokenvec[i]);}
		//cout << "===========================" << endl;
		for(int i = 0; i < keyLength; i++) {key.push_back(keyvec[i]);}

		_tokenMap[key] = token;
		tokenvec.clear();
		keyvec.clear();
	}
	//testPrint();
}
/*
 * pre:
 * post:
 *
 * method that scans the file that is opened in instream reference, scans the code that it reads, and writes it to the open outsteam reference
 *
 * @param instream& - reference to open input stream that code is read from
 * @param ostream& - reference to open output stream that tokens are written to
 *
 * @return null
 */
void LexAnalyzer::scanFile(istream& infile, ostream& outfile)
{
	vector<string> linesout;
	while(!infile.eof())
	{
		string temp;
		getline(infile, temp);
		vector<char> symbols;
		char letter = '0';
		int length = temp.length(), count = 0;
		while(count < length && letter != ' ')
		{
			if(count == 0 && isNumber(temp[0])) // if starts with number and does not only contain numbers, throw error
			{
				//
			}
		}
	}
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
	fb.open("tokens.txt", std::ios::in);
	istream is(&fb);
	if(!is.eof()) {cout << "not eof";} // should be this
	else {cout << "is eof";}
	cout << endl;
	LexAnalyzer la(is);
	la.test();
	return 0;
}
