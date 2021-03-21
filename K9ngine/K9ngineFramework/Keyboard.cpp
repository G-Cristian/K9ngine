#include <Keyboard.h>

#include <GLFW/glfw3.h>

#include <unordered_map>
#include <vector>

namespace K9 {
	namespace Input {
		std::vector<bool> Keyboard::_keys(Keys::SIZE, false);

		std::unordered_map<int, Keys> Keyboard::_glfwKeysMapper({
				{ GLFW_KEY_SPACE, Keys::SPACE },
				{ GLFW_KEY_APOSTROPHE /* ' */, Keys::APOSTROPHE },
				{ GLFW_KEY_COMMA /* , */, Keys::COMMA },
				{ GLFW_KEY_MINUS /* - */, Keys::MINUS },
				{ GLFW_KEY_PERIOD /* . */, Keys::PERIOD },
				{ GLFW_KEY_SLASH /* / */, Keys::SLASH },
				{ GLFW_KEY_0, Keys::KEY_0 },
				{ GLFW_KEY_1, Keys::KEY_1 },
				{ GLFW_KEY_2, Keys::KEY_2 },
				{ GLFW_KEY_3, Keys::KEY_3 },
				{GLFW_KEY_4, Keys::KEY_4 },
				{GLFW_KEY_5, Keys::KEY_5 },
				{GLFW_KEY_6, Keys::KEY_6 },
				{GLFW_KEY_7, Keys::KEY_7 },
				{GLFW_KEY_8, Keys::KEY_8 },
				{GLFW_KEY_9, Keys::KEY_9 },
				{GLFW_KEY_SEMICOLON /* ; */, Keys::SEMICOLON },
				{GLFW_KEY_EQUAL /* = */, Keys::EQUAL },
				{GLFW_KEY_A, Keys::A },
				{GLFW_KEY_B, Keys::B },
				{GLFW_KEY_C, Keys::C },
				{GLFW_KEY_D, Keys::D },
				{GLFW_KEY_E, Keys::E },
				{GLFW_KEY_F, Keys::F },
				{GLFW_KEY_G, Keys::G },
				{GLFW_KEY_H, Keys::H },
				{GLFW_KEY_I, Keys::I },
				{GLFW_KEY_J, Keys::J },
				{GLFW_KEY_K, Keys::K },
				{GLFW_KEY_L, Keys::L },
				{GLFW_KEY_M, Keys::M },
				{GLFW_KEY_N, Keys::N },
				{GLFW_KEY_O, Keys::O },
				{GLFW_KEY_P, Keys::P },
				{GLFW_KEY_Q, Keys::Q },
				{GLFW_KEY_R, Keys::R },
				{GLFW_KEY_S, Keys::S },
				{GLFW_KEY_T, Keys::T },
				{GLFW_KEY_U, Keys::U },
				{GLFW_KEY_V, Keys::V },
				{GLFW_KEY_W, Keys::W },
				{GLFW_KEY_X, Keys::X },
				{GLFW_KEY_Y, Keys::Y },
				{GLFW_KEY_Z, Keys::Z },
				{GLFW_KEY_LEFT_BRACKET /* [ */, Keys::LEFT_BRACKET },
				{GLFW_KEY_BACKSLASH /* \ */, Keys::BACKSLASH },
				{GLFW_KEY_RIGHT_BRACKET /* ] */, Keys::RIGHT_BRACKET },
				{GLFW_KEY_GRAVE_ACCENT /* ` */, Keys::GRAVE_ACCENT },
				{GLFW_KEY_WORLD_1 /* non-US #1 */, Keys::WORLD_1 },
				{GLFW_KEY_WORLD_2 /* non-US #2 */, Keys::WORLD_2 },
				{GLFW_KEY_ESCAPE, Keys::ESCAPE },
				{GLFW_KEY_ENTER, Keys::ENTER },
				{GLFW_KEY_TAB, Keys::TAB },
				{GLFW_KEY_BACKSPACE, Keys::BACKSPACE },
				{GLFW_KEY_INSERT, Keys::INSERT },
				{GLFW_KEY_DELETE, Keys::DELETE },
				{GLFW_KEY_RIGHT, Keys::RIGHT },
				{GLFW_KEY_LEFT, Keys::LEFT },
				{GLFW_KEY_DOWN, Keys::DOWN },
				{GLFW_KEY_UP, Keys::UP },
				{GLFW_KEY_PAGE_UP, Keys::PAGE_UP },
				{GLFW_KEY_PAGE_DOWN, Keys::PAGE_DOWN },
				{GLFW_KEY_HOME, Keys::HOME },
				{GLFW_KEY_END, Keys::END },
				{GLFW_KEY_CAPS_LOCK, Keys::CAPS_LOCK },
				{GLFW_KEY_SCROLL_LOCK, Keys::SCROLL_LOCK },
				{GLFW_KEY_NUM_LOCK, Keys::NUM_LOCK },
				{GLFW_KEY_PRINT_SCREEN, Keys::PRINT_SCREEN },
				{GLFW_KEY_PAUSE, Keys::PAUSE },
				{GLFW_KEY_F1, Keys::F1 },
				{GLFW_KEY_F2, Keys::F2 },
				{GLFW_KEY_F3, Keys::F3 },
				{GLFW_KEY_F4, Keys::F4 },
				{GLFW_KEY_F5, Keys::F5 },
				{GLFW_KEY_F6, Keys::F6 },
				{GLFW_KEY_F7, Keys::F7 },
				{GLFW_KEY_F8, Keys::F8 },
				{GLFW_KEY_F9, Keys::F9 },
				{GLFW_KEY_F10, Keys::F },
				{GLFW_KEY_F11, Keys::F11 },
				{GLFW_KEY_F12, Keys::F12 },
				{GLFW_KEY_F13, Keys::F13 },
				{GLFW_KEY_F14, Keys::F14 },
				{GLFW_KEY_F15, Keys::F15 },
				{GLFW_KEY_F16, Keys::F16 },
				{GLFW_KEY_F17, Keys::F17 },
				{GLFW_KEY_F18, Keys::F18 },
				{GLFW_KEY_F19, Keys::F19 },
				{GLFW_KEY_F20, Keys::F20 },
				{GLFW_KEY_F21, Keys::F21 },
				{GLFW_KEY_F22, Keys::F22 },
				{GLFW_KEY_F23, Keys::F23 },
				{GLFW_KEY_F24, Keys::F24 },
				{GLFW_KEY_F25, Keys::F25 },
				{GLFW_KEY_KP_0, Keys::KP_0 },
				{GLFW_KEY_KP_1, Keys::KP_1 },
				{GLFW_KEY_KP_2, Keys::KP_2 },
				{GLFW_KEY_KP_3, Keys::KP_3 },
				{GLFW_KEY_KP_4, Keys::KP_4 },
				{GLFW_KEY_KP_5, Keys::KP_5 },
				{GLFW_KEY_KP_6, Keys::KP_6 },
				{GLFW_KEY_KP_7, Keys::KP_7 },
				{GLFW_KEY_KP_8, Keys::KP_8 },
				{GLFW_KEY_KP_9, Keys::KP_9 },
				{ GLFW_KEY_KP_DECIMAL, Keys::KP_DECIMAL },
				{ GLFW_KEY_KP_DIVIDE, Keys::KP_DIVIDE },
				{GLFW_KEY_KP_MULTIPLY, Keys::KP_MULTIPLY },
				{GLFW_KEY_KP_SUBTRACT, Keys::KP_SUBTRACT },
				{ GLFW_KEY_KP_ADD, Keys::KP_ADD },
				{GLFW_KEY_KP_ENTER, Keys::KP_ENTER },
				{GLFW_KEY_KP_EQUAL, Keys::KP_EQUAL },
				{ GLFW_KEY_LEFT_SHIFT, Keys::LEFT_SHIFT },
				{ GLFW_KEY_LEFT_CONTROL, Keys::LEFT_CONTROL },
				{ GLFW_KEY_LEFT_ALT, Keys::LEFT_ALT },
				{ GLFW_KEY_LEFT_SUPER, Keys::LEFT_SUPER },
				{ GLFW_KEY_RIGHT_SHIFT, Keys::RIGHT_SHIFT },
				{ GLFW_KEY_RIGHT_CONTROL, Keys::RIGHT_CONTROL },
				{ GLFW_KEY_RIGHT_ALT, Keys::RIGHT_ALT },
				{ GLFW_KEY_RIGHT_SUPER, Keys::RIGHT_SUPER },
				{ GLFW_KEY_MENU, Keys::MENU }
			},
			Keys::SIZE
		);

		bool Keyboard::isPressed(Keys key) {
			return _keys[key];
		}

		void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
			auto keyboardKeysMapperIt = Keyboard::_glfwKeysMapper.find(key);
			if (keyboardKeysMapperIt != Keyboard::_glfwKeysMapper.end()) {
				if (action == GLFW_PRESS) {
					Keyboard::_keys[keyboardKeysMapperIt->second] = true;
				}
				else if (action == GLFW_RELEASE) {
					Keyboard::_keys[keyboardKeysMapperIt->second] = false;
				}
			}
		}
	}
}