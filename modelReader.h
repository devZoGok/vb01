#ifndef MODEL_READER
#define MODEL_READER

#include<string>

namespace vb01{
	class ModelReader{
		public:
			ModelReader(std::string);
			~ModelReader();
		private:
			void readSkeletons(int, int);
			void readMeshes(int, int);
			void readLights(int, int);

			std::string path;
	};
}

#endif

