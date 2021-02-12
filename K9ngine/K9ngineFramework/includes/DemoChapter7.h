#ifndef DEMOCHAPTER7_H_
#define DEMOCHAPTER7_H_

#include <BaseApp.h>

#include <string>

namespace K9 {
	namespace App {
		class DemoChapter7 :public BaseApp {
		public:
			DemoChapter7(const std::string& name, int width, int height) :
				BaseApp(name, width, height) {
			}

			virtual ~DemoChapter7() = default;

			void init() override;
			void update(double dt) override;
			void draw(double dt) override;

		private:
			void initRenderers();
			void initCube();
			void initTorus();
			void initLights();
			void initCamera();
		};
	}
}

#endif // !DEMOCHAPTER7_H_
