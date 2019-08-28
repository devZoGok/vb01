#ifndef LIGHT_H
#define LIGHT_H

namespace vb01{
	class Light{
		public:
			enum Type{POINT,DIRECTIONAL,SPOT,AMBIENT};

			Light();
			~Light();
			void update();
		private:
			Type type;
	};
}

#endif
