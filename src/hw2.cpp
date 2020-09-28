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
 *
 * Input.txt works in current configuration. The issue was that you were not checking for tabs in the chars being passed in
 * 	- You aren't properly checking strings though. Fix it. Either have a counter that ++ on open and -- on close OR vector<char> push('"') on open, pop() on close
 * 	- try the integers first before you start ruining stuff please
 * 		-- getting there. Isn't recognizing integers nor is it recognizing = sign. It's probably the string read in checkLine that isn't being added to correctly
 * 		all in all, should be easy-ish fixes so get them done please
 *
 * Compiler part 1 -- Lexical analyzer
 *
 * read in a file of source code, determining if the words are a part
 * of the language (language is made up for project)
 *
 * Remember, this is all one file. Do NOT make seperate .h and .cpp
 * files for class LexAnalyzer
 *
 * Issues:
 * 	-Was getting an error for map::iterator when trying to put a method below isString, worked fine after moving it. Find the source, maybe a spacing
 * 	 or brackets/parenthesis issue?
 *
 * Notes:
 * 	-Still need to implement:
 * 		-error display and codes -- Added, needs testing now
 * 		-check if(identifier proceeds integer/string)
 * 		-write out (need to figure out how ostream works?) same as ofstream essentially? or does passing an ofstream work with inheritance?
 * 		-basic UI
 * 		-rework isString and isNumber, should return string "t_int" or "t_string" rather than true/false?
 * 			-or at least implement that in the scanFile method -- opted for this, needs testing
 *
 *
 *================================================================== Reference ===========================================================================
 * I wasn't sure how to handle istream and ostream objects. This is a small 'reference' of sorts to the website that I've been looking at how to handle
 * certain classes and methods in c++: www.cplusplus.com/reference/
 *
 * I don't know if it's entirely necessary but I would rather have a source cited than not
 *========================================================================================================================================================
 *
 *
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
	 * pre: called to check if the given string is syntactically a string containing two quotation marks
	 * post: returns boolean confirming if the string is a string in the source code
	 *
	 * @return true if given string from source code has correct syntax, else false
	 */
	string isString(string string) // needs a complete rewrite
	{
		int length = string.length(), count = 0, quoteCount = 0;
		vector<char> cVec;
		while(count < length && quoteCount < 3)
		{
			char c = string[count];
			if(c == '"') {quoteCount++;}
			count++;
		}
		if(quoteCount == 0) {return "not string";}
		else if(quoteCount > 2) {return "invalid";}
		else {return "valid";}
	}
	/*
	 * !!! either use this or delete it !!!
	 */
	int countQmarks(string source)
	{
		int qmarks = 0, length = source.length();
		for(int i = 0; i < length; i++)
		{
			char c = source[i];
			if(c == '"') {qmarks++;}
		}
		return qmarks;
	}
	/*
	 * !!! Delete before turning in !!!
	 * This is a test method to write all of the keys and tokens from map to confirm if they were read correctly from file as well as checking the
	 * confirming that the strings write properly
	 *
	 * This is mostly obsolete, the method that worked with this was removed. The only reason that it's useful still is being able to reference a txt file
	 * that has the strings saved, rather than having to scroll through a console
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
	/*
	 * !!! delete before turning in !!!
	 *
	 * @return null
	 */
	void testPrint()
	{
		for(std::map<string, string>::iterator mitr = _tokenMap.begin(); mitr != _tokenMap.end(); mitr++)
		{
			cout << "First: " << mitr->first << endl << "Second: " << mitr->second << endl << "------------------------" << endl;
		}
	}
	/*
	 * pre: have a string that needs to be split at char ' '
	 * post: returns a vector containing the strings of the split original
	 *
	 * @param s: string to split
	 *
	 * @return vector containing the split string
	 */
	vector<string> splitLine(string s)
	{
		vector<string> inputs;
		int strLength = s.length(), count = 0;
		string temp = "";
		while(count < strLength)
		{
			if(s[count] != ' ') {temp += s[count];}
			else
			{
				if(temp != "")
				{
					inputs.push_back(temp);
					temp = "";
				}
			}
			count++;
		}
		if(temp != "") {inputs.push_back(temp);}
		return inputs;
	}
	/*
	 * !!! THIS NEEDS TO BE MOVED TO PRIVATE AFTER TESTING !!!
	 *
	 * pre: have a line that needs to be checked for syntax
	 * post: vector containing the tokens and potential error found in the string
	 *
	 * @param line: line that is to be checked for syntax
	 *
	 * @return vector containing tokens and potential error; there will only be 1 error at most
	 */
	vector<string> checkLine(string line)
	{
		vector<string> tokens;
		string read = "", error = "";
		int length = line.length(), count = 0;
		bool test = false;
		while(!test)
		{
			char temp = line[count];
			if(temp == ' ' || temp == '\t')
			{
				count++;
			}
			else {test = true;}
		}
		while(count < length && error == "")
		{
			char c = line[count];
			string temp;
			cout << "C: " << c << endl;
			if(c != ' ' && c != '\t') {read += c;}
			cout << "read after adding c: " << read << endl;
			if(_tokenMap[to_string(c)] != "")
			{
				cout << "c: " << c << " matches token " << _tokenMap[to_string(c)] << endl;
				temp = _tokenMap[to_string(c)]+" : "+c;
				tokens.push_back(temp);
			}
			else
			{
				cout << "Test: " << _tokenMap[read] << endl << "Read: " << read << endl;
				if(_tokenMap[read] != "")
				{
					cout << "read: " << read << " matches token " << _tokenMap[read] << endl;
					temp = _tokenMap[read]+" : "+read;
					tokens.push_back(temp);
					read = "";
				}
				cout << "added to read, read: " << read << " has no match" << endl;
				cout << "force test: read: " << read << endl << "token[read]: " << _tokenMap[read] << endl;
				cout << "token[begin]: " << _tokenMap["begin"] << endl;
			}
			if(temp != "" && read != "")
			{
				if(isNumber(read[0]))
				{
					// integer check
					int length = read.length(), icnt = 0;
					if(length > 1)
					{
						icnt = 1;
						while(icnt < length)
						{
							if(!isNumber(read[icnt])) {error = "Error: cannot have identifier start with an integer";} // error
							else {icnt++;} // is still int
						}
						if(error == "") {tokens.push_back("t_int : "+read); read = "";} // is syntactically an integer
					}
				}
				else
				{
					// string check
					int length = read.length(), strCount = 0, qcnt = 0;
					while(strCount < length && error == "")
					{
						char str_c = read[strCount];
						if(str_c == '"') {qcnt++;}
						if(qcnt > 2) {error = "Error: cannot use a quotation mark in an identifier";} // string/identifier error
					}
					if(error == "" && qcnt == 2) {tokens.push_back("t_str : "+read); read = "";} // is syntactically a string
					else if(error == "" && qcnt != 2) {error = "Error: cannot use quotation mark in an identifier";} // string/identifier error
					else if(error == "" && qcnt == 0)
					{
						strCount = 0;
						while(strCount < length && error == "")
						{
							char str_c = read[strCount];
							strCount++;
						}
						if(error == "") {tokens.push_back("t_id : "+read); read = "";}
					}
				}
			}
			count++;
		}
		return tokens;
	}
public:
	LexAnalyzer(){}
	void setMap(istream&); // document this, this is for if the default constructor is used rather than the n-argument
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
	getline(infile, temp);
	while(!infile.eof())
	{
		string token = "", key = "";
		vector<char> tokenvec;
		vector<char> keyvec;
		char index = '0';
		int count = 0, length = temp.length();
		while(count < length && index != ' ')
		{
			index = temp[count];
			//cout << "here?" << endl;
			if(index != ' ')
			{
				tokenvec.push_back(index);
				//cout << index << endl;
			}
			count++;
		}
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
		getline(infile, temp);
	}
	testPrint();
	cout << endl << "--------" << endl << "Test: tokenMap[begin]: " << _tokenMap["begin"] << endl;
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
void LexAnalyzer::scanFile(istream& infile, ostream& outfile) // this needs a complete rewrite. it's fucking trash
{
	string temp, error = "";
	getline(infile, temp);
	vector<string> toWrite;
	while(!infile.eof() && error == "")
	{
		vector<string> tokens = checkLine(temp);
		// test print
		int testSize = tokens.size();
		for(int i = 0; i < testSize; i++)
		{
			cout << "In scanFile, tokens[" << i << "]: " << tokens[i] << endl; // made it here!
		}
		int vecSize = tokens.size(), count = 0;
		while(count < vecSize)
		{
			string copy = tokens[count];
			cout << "copy string: " << copy << endl;
			toWrite.push_back(copy);
			count++;
		}
		string errorCheck = tokens[vecSize-1];
		if(errorCheck.substr(0, 5) == "Error:") {error = errorCheck;}
		getline(infile, temp);
		cout << "Next line: " << temp << endl;
	}
	int linesout = toWrite.size(), lnCount = 0;
	while(lnCount < linesout)
	{
		outfile << toWrite[lnCount] << endl;
		lnCount++;
	}
	outfile.flush();
	if(error != "") {cout << error << endl;}
}
int main()
{
	//==================================================================================================================================================================
	ifstream infile;
	ofstream outfile;
	string tokenfn = "tokens.txt", inputfn = "input2.txt", outputfn = "outfile2.txt";
	infile.open(tokenfn);
	LexAnalyzer lex(infile);
	infile.close();
	infile.open(inputfn);
	outfile.open(outputfn);
	lex.scanFile(infile, outfile);
	outfile.close();
	return 0;
}
