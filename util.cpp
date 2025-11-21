#include "util.h"
#include "root.h"

#include <glfw3.h>
#include <fstream>

using namespace std;

namespace vb01{
	double *x = nullptr, *y = nullptr;

	Vector2 getCursorPos() {
		if(!x) x = new double;

		if(!y) y = new double;
			
		glfwGetCursorPos(Root::getSingleton()->getWindow(), x, y);
		return Vector2(*x, *y);
	}

	bool rectanglesIntersect(Vector2 rp1, Vector2 rd1, Vector2 rps1, Vector2 rp2, Vector2 rd2, Vector2 rdl2, Vector2 rps2){
		Vector2 point[]{
			rp2 + .5 * (rdl2 * rps2.x + rd2 * rps2.y),
			rp2 + .5 * (-rdl2 * rps2.x + rd2 * rps2.y),
			rp2 + .5 * (-rdl2 * rps2.x - rd2 * rps2.y),
			rp2 + .5 * (rdl2 * rps2.x - rd2 * rps2.y)
		};

		for(int i = 0; i < 4; i++){
			Vector2 p = point[i], pv = p - rp1;
			float pd = pv.getLength(), angle = rd1.getAngleBetween(pv.norm());
			angle = (angle > PI / 2 ? PI - angle : angle);

			bool b1 = (pd * sin(angle) <= .5 * rps1.x);
			bool b2 = (pd * cos(angle) <= .5 * rps1.y);

			if(b1 && b2) return true;
		}

		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
			}
		}

		return false;
	}

	void readFile(string path, vector<string> &lines, int firstLine, int lastLine){
		string l;
		std::ifstream inFile(path);
		for(int i = 0; i < lastLine && getline(inFile,l); i++){
			if(i >= firstLine)
				lines.push_back(l);
		}
		inFile.close();
	}

	void getLineData(string line, string data[], int numData, int offset){
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

	wstring stringToWstring(string str){
		wstring wstr = L"";

		for(char ch : str)
			wstr += ch;

		return wstr;
	}

	string wstringToString(wstring wstr){
		string str = "";

		for(wchar_t ch : wstr)
			if(ch < 256)
				str += ch;

		return str;
	}

	int findNthOccurence(string &str, string occ, int occId, bool forward){
		int occurenceId = (forward ? str.find(occ) : str.rfind(occ)), iteration = 0;

		while(occurenceId != -1 && iteration != occId){
			occurenceId = (forward ? str.find(occ, occurenceId + 1) : str.rfind(occ, occurenceId - 1)); 
			iteration++;
		}

		return occurenceId;
	}
}
