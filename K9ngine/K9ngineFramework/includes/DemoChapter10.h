#ifndef DEMOCHAPTER10_H_
#define DEMOCHAPTER10_H_

#include <BaseApp.h>

namespace K9 {
	namespace App {
		class DemoChapter10 :public BaseApp {
		public:
			DemoChapter10(const std::string& name, int width, int height) :
				BaseApp(name, width, height) {
			}

			virtual ~DemoChapter10() = default;

			void init() override final;
			void update(double dt) override final;
			void draw(double dt) override final;
		private:
			void initRenderers();
			void loadTextures();
			void initSphere1();
			void initSphere2();
			void initSphere3();
			void initLights();
			void initCamera();
		};
	}
}

#endif // !DEMOCHAPTER10_H_
