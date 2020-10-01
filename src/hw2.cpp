//============================================================================
// Name        : John Wolf
// Project     : HW2 -- Compiler - part 1
// Professor   : Dr.St.Clair
// Class       : CSCE 306
// Description : Checks syntax of input file and writes source code to output
//============================================================================



#include <iostream>
#include <vector>
#include <map>
#include <iterator>
#include <ios>
#include <fstream>
#include <sstream>
using namespace std;

/*
 *
 *====================================================================== Reference =====================================================================================
 * I wasn't sure how to handle istream and ostream objects. This is a small 'reference' of sorts to the web site that I've been looking at how to handle
 * certain classes and methods in c++: www.cplusplus.com/reference/
 *
 * I also used the reference site to see how to use string stream for converting the chars into strings without having them read as their ascii values
 *
 * I don't know if it's entirely necessary but I would rather have a source cited than not
 *
 * This is mostly irrelevant since going over inheritance in class but I'm still leaving it in the code for the reason above
 *======================================================================================================================================================================
 *
 */

//===================================================================== LexAnalyzer ====================================================================================
class LexAnalyzer
{
private:
	//=============================================================== Member variables =================================================================================
	vector<string> _lexemes;
	vector<string> _tokens;
	map<string, string> _tokenMap;
	//==================================================================================================================================================================
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//=============================================================== Private Methods ==================================================================================
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
	string findKey(string key)
	{
		std::map<string, string>::iterator mitr = _tokenMap.begin();
		string s;
		mitr = _tokenMap.find(key);
		if(mitr != _tokenMap.end()) {s = mitr->second;}
		else {s = "";}
		return s;
	}
	/*
	 * pre: have a line that needs to be checked for syntax
	 * post: vector containing the tokens and potential error found in the string
	 *
	 * @param line: line that is to be checked for syntax
	 *
	 * @return vector containing tokens and potential error; there will only be 1 error at most
	 */
	vector<string> lineCheck(string line)
	{
		vector<string> tokens;
		string read, error = "";
		int length = line.length(), count = 0;
		bool isStr = false;
		while(count < length)
		{
			char c = line[count];
			stringstream sstream; // used string stream because of a few issues with to_string converting chars to their ascii values rather than to a string
			sstream << c; // stores the char into the stream
			string temp;
			sstream >> temp; // retrieves the char as a string
			string key;
			key = findKey(temp);
			string token;
			if(key != "" && !isStr) {token = key+" : "+temp;} // read char was found in token map, sets token to its code
			string copy = "";
			if(token == "") // char was not found in map
			{
				if(c == '"')
				{
					isStr = !isStr; // if closed string, opens or if open, closes
					if(!isStr) {token = "t_str : "+temp;} // gets code for string if one closed
				}
				if(isStr)
				{
					if(c != '"') {read += temp;} // if its a string, any character can be used
				}
				else
				{
					if(c != ' ' && c != '\t') {read += temp;} // ignoring tabs and spaces
					key = findKey(read); // checks if adding newest char makes a keyword
					if(key != "") {token = findKey(read)+" : "+read; read = "";} // if not empty, one was found
					if(token != "")
					{
						if(read != "") // this is so that anything read before the keyword or symbol is pushed first
						{
							copy = token; // copy variable to store the code of the symbol/keyword
							if(isNumber(read[0])) // starts with an integer
							{
								int rlen = read.length(), rcnt = 0;
								while(rcnt < rlen && error == "")
								{
									char tchar = read[rcnt];
									if(!isNumber(tchar)) {error = "Error: cannot start an identifier with an integer";} // non integer was found, error
									rcnt++;
								}
								token = "t_int : "+read;
								read = "";
							}
							else // does not start with an integer
							{
								if(key != copy) // prevents keywords/symbols from being pushed as t_id keyword
								{
									token = "t_id : "+read;
									read = "";
								}
								else {copy = "";} // if duplicate, clears copy
							}
						}
					}
				}
			}
			else // char was found in map
			{
				if(read != "") // if there are more chars that were read, enters block
				{
					copy = token; // copies code for symbol so that what was read before can be pushed
					if(isNumber(read[0])) // starts with an integer
					{
						int rlen = read.length(), rcnt = 0;
						while(rcnt < rlen && error == "")
						{
							char tchar = read[rcnt];
							if(!isNumber(tchar)) {error = "Error: cannot start an identifier with an integer";} // non integer value found, error
							rcnt++;
						}
						token = "t_int : "+read;
						read = "";
					}
					else
					{
						if(key != token) // prevents symbol from being pushed as t_id
						{
							token = "t_id : "+read;
							read = "";
						}
						else {copy = "";} // if duplicate, clears copy so it doesn't get pushed below
					}
				}
			}
			count++;
			if(error != "") {tokens.push_back(error);} // error was found
			else // no errors
			{
				if(token != "") // pushes token if there is one
				{
					tokens.push_back(token);
					if(copy != "") {tokens.push_back(copy);} // pushes the keyword/symbol code if one exists
				}
			}
		}
		if(isStr) {error = "Error: cannot use quotation marks in an identifier"; tokens.push_back(error);} // pushes error if there is an unclosed string
		return tokens;
	}
	//==================================================================================================================================================================
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//====================================================================== Public Methods ============================================================================
public:
	LexAnalyzer(){}
	LexAnalyzer(istream&);
	void setMap(istream&); // used in case the default constructor is called
	void scanFile(istream&, ostream&);
	//==================================================================================================================================================================
};
//======================================================================================================================================================================
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//========================================================================== Class Methods =============================================================================
/*
 * pre: n-argument constructor
 * post: LexAnalyzer class object with tokenMap populated
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
			if(index != ' ')
			{
				tokenvec.push_back(index);
			}
			count++;
		}
		index = '0';
		while(count < length && index != ' ')
		{
			index = temp[count];
			count++;
			keyvec.push_back(index);
		}
		int tokenLength = tokenvec.size(), keyLength = keyvec.size();
		for(int i = 0; i < tokenLength; i++) {token.push_back(tokenvec[i]);}
		for(int i = 0; i < keyLength; i++) {key.push_back(keyvec[i]);}
		_tokenMap[key] = token;
		tokenvec.clear();
		keyvec.clear();
		getline(infile, temp);
	}
}
/*
 * pre: Used after creating a LexAnalyzer object with the default constructor
 * post: Populates the token map member variable
 *
 * Mutator for _tokenMap, reads in from open input stream reference
 *
 * @param: istream& infile: opened input stream
 *
 * return null
 */
void LexAnalyzer::setMap(istream& infile)
{
	string temp;
	getline(infile, temp);
	while(!infile.eof())
	{
		string token = "", key = "";
		vector<char> tokenvec, keyvec;
		char index = '0';
		int count = 0, length = temp.length();
		while(count < length && index != ' ')
		{
			index = temp[count];
			if(index != ' ') {tokenvec.push_back(index);}
			count++;
		}
		index = '0';
		while(count < length && index != ' ')
		{
			index = temp[count];
			count++;
			keyvec.push_back(index);
		}
		int tokenLength = tokenvec.size(), keyLength = keyvec.size();
		for(int i = 0; i < tokenLength; i++) {token.push_back(tokenvec[i]);}
		for(int i = 0; i < keyLength; i++) {key.push_back(keyvec[i]);}
		_tokenMap[key] = token;
		tokenvec.clear();
		keyvec.clear();
		getline(infile, temp);
	}
}
/*
 * pre: Used to read code from input stream and writes corresponding source code to the output stream
 * post: Returns null and writes code and potential error to the output file
 *
 * method that scans the file that is opened in input stream reference, scans the code that it reads, and writes it to the open outsteam reference
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
		vector<string> tokens = lineCheck(temp);
		int vecSize = tokens.size(), count = 0;
		while(count < vecSize)
		{
			string copy = tokens[count];
			toWrite.push_back(copy);
			count++;
		}
		string errorCheck = tokens[vecSize-1];
		if(errorCheck[0] == 'E') {error = errorCheck;}
		getline(infile, temp);
	}
	int linesout = toWrite.size(), lnCount = 0;
	while(lnCount < linesout)
	{
		outfile << toWrite[lnCount] << endl;
		outfile.flush();
		lnCount++;
	}
	if(error != "") {cout << error << endl;}
}
//======================================================================================================================================================================
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//============================================================================ Main ====================================================================================
int main()
{
	//================================================================== Initializations ===============================================================================
	ifstream infile;
	ofstream outfile;
	string tokenfn = "tokens.txt", inputfn, outputfn;
	infile.open(tokenfn);
	LexAnalyzer lex(infile);
	infile.close();
	//==================================================================================================================================================================

	cout << "Compiler - part 1 by John Wolf" << endl << "==============================" << endl << "You can enter 0 at any input to end the program" << endl << endl;
	string input = "", temp;
	cout << "Enter the name of the input file: " << endl;
	cin >> input;
	if(input != "0")
	{
		inputfn = input;
		cout << "Enter the name of the output file: " << endl;
		cin >> input;
		if(input != "0")
		{
			outputfn = input;
			infile.open(inputfn);
			if(!infile) {cout << "error linking file" << endl; exit(-1);}
			outfile.open(outputfn);
			lex.scanFile(infile, outfile);
			cout << "Code finished scanning" << endl << endl << "Results written to " << outputfn << endl;
		}
	}
	else {cout << "Exiting" << endl;}
	return 0;
}
//======================================================================================================================================================================



