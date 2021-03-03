#ifndef DEMOCUBEMAP1_H_
#define DEMOCUBEMAP1_H_

#include <BaseApp.h>

namespace K9 {
	namespace App{
		class DemoCubeMap1 :public BaseApp {
		public:
			DemoCubeMap1(const std::string& name, int width, int height) :
				BaseApp(name, width, height) {
			}

			virtual ~DemoCubeMap1() = default;

			void init() override final;
			void update(double dt) override final;
			void draw(double dt) override final;
		private:
			void initRenderers();
			void initCube();
			void initCube2();
			void initTorus();
			void initLights();
			void initCamera();
		};
	}
}

#endif // !DEMOCUBEMAP1_H_
