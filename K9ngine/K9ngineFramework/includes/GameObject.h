#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <glm/glm.hpp>
#include <string>

namespace K9 {
	class GameObject {
	public:
		GameObject(const std::string& name) :
			_name(name),
			_position(0.0f, 0.0f, 0.0f),
			_localRotation(0.0f, 0.0f, 0.0f),
			_offset(0.0f, 0.0f, 0.0f),
			_rotation(0.0f, 0.0f, 0.0f),
			_scale(1.0f, 1.0f, 1.0f) {
		}

		GameObject(std::string&& name) noexcept :
			_name(std::move(name)),
			_position(0.0f, 0.0f, 0.0f),
			_localRotation(0.0f, 0.0f, 0.0f),
			_offset(0.0f, 0.0f, 0.0f),
			_rotation(0.0f, 0.0f, 0.0f),
			_scale(1.0f, 1.0f, 1.0f) {
		}

		GameObject(const GameObject&) = delete;

		GameObject(GameObject&& other) noexcept :
			_name(std::move(other._name)),
			_position(std::move(other._position)),
			_localRotation(std::move(other._localRotation)),
			_offset(std::move(other._offset)),
			_rotation(std::move(other._rotation)),
			_scale(std::move(other._scale)) {
			other._name = "";
			other._position = glm::vec3();
			other._localRotation = glm::vec3();
			other._offset = glm::vec3();
			other._rotation = glm::vec3();
			other._scale = glm::vec3();
		}

		~GameObject() = default;

		GameObject& operator=(const GameObject&) = delete;

		GameObject& operator=(GameObject&& other)noexcept {
			if (this != &other) {
				_name = std::move(other._name);
				_position = std::move(other._position);
				_localRotation = std::move(other._localRotation);
				_offset = std::move(other._offset);
				_rotation = std::move(other._rotation);
				_scale = std::move(other._scale);

				other._name = "";
				other._position = glm::vec3();
				other._localRotation = glm::vec3();
				other._offset = glm::vec3();
				other._rotation = glm::vec3();
				other._scale = glm::vec3();
			}

			return *this;
		}

		const std::string& name() const {
			return _name;
		}

		const glm::vec3& position() const {
			return _position;
		}

		glm::vec3& position() {
			return _position;
		}

		const glm::vec3& localRotation() const {
			return _localRotation;
		}

		glm::vec3& localRotation() {
			return _localRotation;
		}

		const glm::vec3& offset() const {
			return _offset;
		}

		glm::vec3& offset() {
			return _offset;
		}

		const glm::vec3& rotation() const {
			return _rotation;
		}

		glm::vec3& rotation() {
			return _rotation;
		}

		const glm::vec3& scale() const {
			return _scale;
		}

		glm::vec3& scale() {
			return _scale;
		}
	private:
		std::string _name;
		glm::vec3 _position;
		glm::vec3 _localRotation;
		glm::vec3 _offset;
		glm::vec3 _rotation;
		glm::vec3 _scale;
	};
}

#endif // !_GAMEOBJECT_H_
