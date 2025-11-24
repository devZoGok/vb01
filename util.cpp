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

	bool rectInter(Vector2 rp1, Vector2 rd1, Vector2 rps1, Vector2 p2[]){
		for(int i = 0; i < 4; i++){
			Vector2 pv = p2[i] - rp1;
			float pd = pv.getLength(), angle = rd1.getAngleBetween(pv.norm());
			angle = (angle > PI / 2 ? PI - angle : angle);

			bool b1 = (pd * sin(angle) <= .5 * rps1.x);
			bool b2 = (pd * cos(angle) <= .5 * rps1.y);

			if(b1 && b2) return true;
		}

		return false;
	}

	bool segmInter(Vector2 p1[], Vector2 p2[]){
		pair<Vector2, Vector2> e1[]{
			make_pair(p1[0], p1[1]),
			make_pair(p1[1], p1[2]),
			make_pair(p1[2], p1[3]),
			make_pair(p1[3], p1[0])
		};
		pair<Vector2, Vector2> e2[]{
			make_pair(p2[0], p2[1]),
			make_pair(p2[1], p2[2]),
			make_pair(p2[2], p2[3]),
			make_pair(p2[3], p2[0])
		};

		for(int i = 0; i < 4; i++){
			//float a = hypVec.z / hypVec.x;
			//float b = pos.z - a * pos.x;

			Vector2 e1Vec = e1[i].second - e1[i].first;
			float a1, b1, x1;
			bool vert1 = true;

			if(e1Vec.x != 0){
				vert1 = false;
				a1 = e1Vec.y / e1Vec.x;
				b1 = e1[i].second.y - a1 * e1[i].second.x;
			}
			else x1 = e1[i].first.x;

			for(int j = 0; j < 4; j++){
				Vector2 e2Vec = e2[j].second - e2[j].first;
				float a2, b2, x2;
				bool vert2 = true;

				if(e2Vec.x != 0){
					vert2 = false;
					a2 = e2Vec.y / e2Vec.x;
					b2 = e2[j].second.y - a2 * e2[j].second.x;
				}
				else x2 = e2[j].first.x;

				if(!vert1 && !vert2){
					if(a1 - a2 == 0) continue;

					float x = (b1 - b2) / (a1 - a2);
					float y = a1 * x + b1;

					if(
							(e1[i].first.x < x && x < e1[i].second.x) &&
							(e1[i].first.y < y && y < e1[i].second.y)
					){
					   	return true;
					}
				}
				else if(!vert1 && vert2){
					float minX = e1[i].first.x, maxX = e1[i].second.x;
					float minY = e2[i].first.y, maxY = e2[i].second.y;

					if(minX > maxX) swap(minY, maxY);
					if(minY > maxY) swap(minY, maxY);

					float y = a1 * x2 + b1;

					if((minX < x2 && x2 < maxX) && (minY < y && y < maxY)){
						return true;
					}
				}
				else if(vert1 && !vert2){
					float minX = e2[j].first.x, maxX = e2[j].second.x;
					float minY = e1[j].first.y, maxY = e1[j].second.y;

					if(minX > maxX) swap(minY, maxY);
					if(minY > maxY) swap(minY, maxY);

					float y = a2 * x1 + b2;

					if((minX < x1 && x1 < maxX) && (minY < y && y < maxY)){
						return true;
					}
				}
			}
		}

		return false;
	}

	bool rectanglesIntersect(Vector2 rp1, Vector2 rd1, Vector2 rdl1, Vector2 rps1, Vector2 rp2, Vector2 rd2, Vector2 rdl2, Vector2 rps2){
		Vector2 p1[]{
			rp1 + .5 * (rdl1 * rps1.x + rd1 * rps1.y),
			rp1 + .5 * (-rdl1 * rps1.x + rd1 * rps1.y),
			rp1 + .5 * (-rdl1 * rps1.x - rd1 * rps1.y),
			rp1 + .5 * (rdl1 * rps1.x - rd1 * rps1.y)
		};
		Vector2 p2[]{
			rp2 + .5 * (rdl2 * rps2.x + rd2 * rps2.y),
			rp2 + .5 * (-rdl2 * rps2.x + rd2 * rps2.y),
			rp2 + .5 * (-rdl2 * rps2.x - rd2 * rps2.y),
			rp2 + .5 * (rdl2 * rps2.x - rd2 * rps2.y)
		};

		if(rectInter(rp1, rd1, rps1, p2)) return true;
		if(rectInter(rp2, rd2, rps2, p1)) return true;

		if(segmInter(p1, p2)) return true;

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
