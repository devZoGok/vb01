#include"modelReader.h"
#include"model.h"
#include<vector>
#include<fstream>

using namespace std;

namespace vb01{
	ModelReader::ModelReader(Model *model, string path){
		this->model = model;
		this->path = path;
	}

	ModelReader::~ModelReader(){
	}
}
