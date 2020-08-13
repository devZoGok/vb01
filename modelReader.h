#ifndef MODEL_READER
#define MODEL_READER

#include<string>
#include"model.h"

namespace vb01{
	class ModelReader{
		public:
		protected:
			ModelReader(Model*, std::string);
			~ModelReader();
			Model *model = nullptr;
			std::string path;
	};
}

#endif

