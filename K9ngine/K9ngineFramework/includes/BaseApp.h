#ifndef BASEAPP_H_
#define BASEAPP_H_

#include <GLFW/glfw3.h>

namespace K9 {
	namespace App {
		class BaseApp {
		public:
			BaseApp() = default;
			virtual ~BaseApp() = default;

			void start();
		protected:
			virtual void init() = 0;
			virtual void update() = 0;
			virtual void draw() = 0;

			GLFWwindow* _window;
		};
	}
}

#endif // !BASEAPP_H_
