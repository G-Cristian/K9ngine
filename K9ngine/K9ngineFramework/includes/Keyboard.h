#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <GLFW/glfw3.h>

#include <unordered_map>
#include <vector>

namespace K9 {
	namespace Input {
		enum Keys {
			SPACE = 0,
			APOSTROPHE, /* ' */
			COMMA, /* , */
			MINUS, /* - */
			PERIOD, /* . */
			SLASH, /* / */
			KEY_0,
			KEY_1,
			KEY_2,
			KEY_3,
			KEY_4,
			KEY_5,
			KEY_6,
			KEY_7,
			KEY_8,
			KEY_9,
			SEMICOLON, /* ; */
			EQUAL, /* = */
			A,
			B,
			C,
			D,
			E,
			F,
			G,
			H,
			I,
			J,
			K,
			L,
			M,
			N,
			O,
			P,
			Q,
			R,
			S,
			T,
			U,
			V,
			W,
			X,
			Y,
			Z,
			LEFT_BRACKET, /* [ */
			BACKSLASH, /* \ */
			RIGHT_BRACKET, /* ] */
			GRAVE_ACCENT, /* ` */
			WORLD_1, /* non-US #1 */
			WORLD_2, /* non-US #2 */
			ESCAPE,
			ENTER,
			TAB,
			BACKSPACE,
			INSERT,
			DELETE,
			RIGHT,
			LEFT,
			DOWN,
			UP,
			PAGE_UP,
			PAGE_DOWN,
			HOME,
			END,
			CAPS_LOCK,
			SCROLL_LOCK,
			NUM_LOCK,
			PRINT_SCREEN,
			PAUSE,
			F1,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,
			F13,
			F14,
			F15,
			F16,
			F17,
			F18,
			F19,
			F20,
			F21,
			F22,
			F23,
			F24,
			F25,
			KP_0,
			KP_1,
			KP_2,
			KP_3,
			KP_4,
			KP_5,
			KP_6,
			KP_7,
			KP_8,
			KP_9,
			KP_DECIMAL,
			KP_DIVIDE,
			KP_MULTIPLY,
			KP_SUBTRACT,
			KP_ADD,
			KP_ENTER,
			KP_EQUAL,
			LEFT_SHIFT,
			LEFT_CONTROL,
			LEFT_ALT,
			LEFT_SUPER,
			RIGHT_SHIFT,
			RIGHT_CONTROL,
			RIGHT_ALT,
			RIGHT_SUPER,
			MENU,

			SIZE
		};

		void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

		static class Keyboard {
		public:
			static bool isPressed(Keys key);

			friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		private:
			static std::vector<bool> _keys;
			static std::unordered_map<int, Keys> _glfwKeysMapper;
		};
	}
}

#endif // !KEYBOARD_H_
