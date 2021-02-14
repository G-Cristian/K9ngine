#ifndef DEMOCHAPTER8_H_
#define DEMOCHAPTER8_H_

#include <BaseApp.h>
#include <PositionalLight.h>

#include <memory>
#include <string>
#include <utility>

namespace K9 {
	namespace App {
		class DemoChapter8 :public BaseApp {
		public:
			DemoChapter8(const std::string& name, int width, int height) :
				BaseApp(name, width, height),
				_positionalLight(nullptr) {
			}
			virtual ~DemoChapter8() = default;

			void init() override;
			void update(double dt) override;
			void draw(double dt) override;
		private:
			void initRenderers();
			void initCube();
			void initPyramid();
			void initTorus();
			void initLights();
			void initCamera();
			std::pair<GLuint, GLuint> setupShadowBuffers();

			std::shared_ptr<K9::Lighting::PositionalLight> _positionalLight;
		};
	}
}

#endif // !DEMOCHAPTER8_H_
