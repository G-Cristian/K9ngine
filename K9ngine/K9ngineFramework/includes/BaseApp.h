#ifndef BASEAPP_H_
#define BASEAPP_H_


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <World.h>

#include <memory>
#include <string>

namespace K9 {
	namespace App {
		void windowResizeCallbackWrapper(GLFWwindow* window, int newWidth, int newHeight);

		class BaseApp {
		public:
			BaseApp(const std::string& name, int width, int height, int framesPerSecond = 60);

			BaseApp(const BaseApp&) = delete;
			BaseApp(BaseApp&&) noexcept = delete;

			virtual ~BaseApp();

			BaseApp& operator=(const BaseApp&) = delete;
			BaseApp& operator=(BaseApp&&) noexcept = delete;

			void start();
		protected:
			friend void windowResizeCallbackWrapper(GLFWwindow*, int, int);
			static BaseApp* _pointerToThisForUseInWindowResizeCallback;

			void windowResizeCallback(GLFWwindow* window, int newWidth, int newHeight);
			virtual void resetingProjection(float newWidth, float newHeight);
			void initWindow(int initWidth, int initHeight);
			virtual void init() = 0;
			virtual void update(double dt) = 0;
			virtual void draw(double dt) = 0;
			void terminate();

			std::string _name;
			int _width;
			int _height;
			GLFWwindow* _window;
			bool _inited;

			bool _shouldTerminate;
			std::unique_ptr<World> _world;
		private:
			double _currentFrameTime;
			double _lastFrameTime;
			double _secondsPerUpdate;
		};
	}
}

#endif // !BASEAPP_H_
