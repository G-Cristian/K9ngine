#include <BaseApp.h>

#include <GameObjectComponentsPool.h>
#include <K9Debug.h>
#include <ModelsPool.h>
#include <RenderingManager.h>
#include <TexturesPool.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

namespace K9 {
	namespace App {
		BaseApp* BaseApp::_pointerToThisForUseInWindowResizeCallback = nullptr;

		void windowResizeCallbackWrapper(GLFWwindow* window, int newWidth, int newHeight) {
			BaseApp::_pointerToThisForUseInWindowResizeCallback->windowResizeCallback(window, newWidth, newHeight);
		}

		BaseApp::BaseApp(const std::string& name, int width, int height) :
			_name(name),
			_width(width),
			_height(height),
			_window(nullptr),
			_inited(false),
			_shouldTerminate(false),
			_world(new World()) {
			
			K9_ASSERT(_pointerToThisForUseInWindowResizeCallback == nullptr);
			if (_pointerToThisForUseInWindowResizeCallback == nullptr) {
				_pointerToThisForUseInWindowResizeCallback = this;
			}
		}

		BaseApp::~BaseApp() {
			if (_inited) {
				terminate();
			}

			K9::Graphics::TexturesPool::instance().dispose();
			K9::Graphics::ModelsPool::instance().dispose();
			K9::Components::GameObjectComponentsPool::instance().dispose();
			K9::Graphics::RenderingManager::instance().dispose();

			_pointerToThisForUseInWindowResizeCallback = nullptr;
		}

		void BaseApp::initWindow(int initWidth, int initHeight) {
			if (!glfwInit()) {
				std::cerr << "Error at line " << __LINE__ << " (!glfwInit())" << std::endl;
				return;
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

			_window = glfwCreateWindow(initWidth, initHeight, _name.c_str(), NULL, NULL);
			glfwMakeContextCurrent(_window);

			if (glewInit() != GLEW_OK) {
				std::cerr << "Error at line " << __LINE__ << " (glewInit() != GLEW_OK)" << std::endl;
				return;
			}

			glfwSwapInterval(1);
			//glfwSetWindowSizeCallback(_window, windowResizeCallbackWrapper);
			glfwSetFramebufferSizeCallback(_window, windowResizeCallbackWrapper);

			_inited = true;
		}

		void BaseApp::windowResizeCallback(GLFWwindow* window, int newWidth, int newHeight) {
			//float aspect = (float)(newWidth) / (float)(newHeight);
			glViewport(0, 0, newWidth, newHeight);
			resetingProjection((float)newWidth, (float)newHeight);
		}

		void BaseApp::resetingProjection(float newWidth, float newHeight) {
			_world->resetCamerasProjections(newWidth, newHeight);
		}

		void BaseApp::start() {
			initWindow(_width, _height);
			_shouldTerminate = false;
			init();
			while (!glfwWindowShouldClose(_window) && !_shouldTerminate) {
				update(glfwGetTime());
				draw(glfwGetTime());
				glfwSwapBuffers(_window);
				glfwPollEvents();
			}
			_shouldTerminate = true;
			terminate();
		}

		void BaseApp::terminate() {
			if (_window != nullptr) {
				glfwDestroyWindow(_window);
			}
			glfwTerminate();

			_inited = false;
		}
	}
}
