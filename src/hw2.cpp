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
 * Issues:
 * 	-Was getting an error for map::iterator when trying to put a method below isString, worked fine after moving it. Find the source, maybe a spacing
 * 	 or brackets/parenthesis issue?
 *
 * Notes:
 * 	-Still need to implement:
 * 		-error display and codes
 * 		-check if(identifier proceeds integer/string)
 * 		-write out (need to figure out how ostream works?)
 * 		-basic UI
 * 		-rework isString and isNumber, should return string "t_int" or "t_string" rather than true/false?
 * 			-or at least implement that in the scanFile method
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
	 * !!! Delete before turning in !!!
	 * This is a test method that prints all of the keys and tokens in the map _tokenMap
	 *
	 * @return null
	 */
	bool isString(string string)
	{
		int length = string.length();
		if(string[0] == '"' && string[length-1] == '"') {return true;}
		return false;
	}
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
		}
		if(temp != "") {inputs.push_back(temp);}
		return inputs;
	}
	string getOutLine(string source)
	{
		string out, token;
		return out;
	}
	string isKey(string s)
	{
		std::map<string, string>::iterator mitr = _tokenMap.begin();
		string temp = "";
		while(mitr != _tokenMap.end() && temp != s)
		{
			temp = mitr->first;
			if(temp == s) {return mitr->second;}
			mitr++;
		}
		return "false";
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
	if(!infile) {exit(-1);} // change, need sentinel loop structure
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
	/*
	 * !!! You're forgetting to check for ints (str len == 1 && isNumber), strings (make a separate method isString, vector<char>, push single quote onto
	 * vector, then pop if encountered again? basically, treat it like a stack) and the keywords/assignments !!!
	 */
	vector<string> linesout;
	string temp;
	getline(infile, temp);
	while(!infile.eof())
	{
		/*
		 * Layout:
		 * 1- vector<string> sline = splitLine(temp)
		 * 2- for(len(sline)) -- MAKE SURE TO CHECK IF IT'S A NUMBER HERE. DON'T KEEP MAKING LOOPS THAT TRAVERSE THE SAME THING
		 * 	if((sline[i])[0] is a number) push a string signifying that it's an error and why it is
		 * 		-- Need to ask about this tomorrow or friday during office hours
		 * 	else linesout.push(sline[i])
		 * 3- compare each of the lines to the token text file
		 * 	-- need to check the string in vector first
		 * 		if(linesout.substr(0, 5) != "Error:") {check(string)}
		 */
		vector<string> sline = splitLine(temp);
		int vecSize = sline.size();
		for(int i = 0; i < vecSize; i++)
		{
			if(isNumber((sline[i])[0])) {linesout.push_back("Error: cannot have integer start variable name");}
			else {linesout.push_back(sline[i]);}
		}
	}
}
bool isNumber(char letter)
{
	string nums = "1234567890";
	for(int i = 0; i < 10; i++)
	{
		if(letter == nums[i]) {return false;}
	}
	return true;
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

	/*filebuf fb;
	fb.open("tokens.txt", std::ios::in);
	istream is(&fb);
	if(!is.eof()) {cout << "not eof";} // should be this
	else {cout << "is eof";}
	cout << endl;
	LexAnalyzer la(is);
	la.test();*/

	// this is for testing how to stack the chars and work with them. c++ doesn't have a split method so you'll have to get creative
	/*string test = "this is a  test    to 2see if  it works";
	int length = test.length(), count = 0;
	string line = "";
	vector<string> inputs;
	while(count < length)
	{
		if(test[count] != ' ') {line += test[count];}
		else
		{
			if(test[count] == ' ' && line != "")
			{
				inputs.push_back(line);
				line = "";
			}
		}
		count++;
	}
	if(line != "") {inputs.push_back(line);}
	int incount = inputs.size();
	for(int i = 0; i < incount; i++)
	{
		if(isNumber((inputs[i])[0]))
		{
			cout << inputs[i] << endl;
		}
		else {cout << "Error: invalid variable name -- cannot start with a number" << endl;}
	}*/

	// The code above splits it and the isNumber method works properly


	return 0;
}
