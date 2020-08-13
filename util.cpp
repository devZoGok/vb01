#include"util.h"
#include<fstream>

using namespace std;

namespace vb01{
	void readFile(string path, vector<string> &lines, int firstLine, int lastLine){
		string l;
		std::ifstream inFile(path);
		for(int i = 0; i < lastLine && getline(inFile,l); i++){
			if(i >= firstLine)
				lines.push_back(l);
		}
		inFile.close();
	}

	void getLineData(string &line, string data[], int numData, int offset){
		int offsetComma = 0, c1 = 0;
		for(int i = 0; i < line.length() && offsetComma < offset; i++)
			if(line.c_str()[i] == ' '){
				c1 = i + 1;
				offsetComma++;
			}
		int c2 = c1;
		for(int i = 0; i < numData; i++){
			for(int j = c1; j < line.length(); j++)	
				if(line.c_str()[j] == ' '){
					c2 = j;
					break;
				}
			data[i] = line.substr(c1, c2 - c1);
			c2++;
			c1 = c2;
		}
	}

	int getCharId(string line, char ch, bool reverse){
		int charId = -1;
		if(!reverse){
			for(int i = 0; i < line.length(); i++){
				if(line[i] == ch){
					charId = i;
					break;
				}
			}
		}
		else{
			for(int i = line.length() - 1; i >= 0; i--){
				if(line[i] == ch){
					charId = i;
					break;
				}
			}
		}
		return charId;
	}
}
