#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

namespace K9 {
	class Camera {
	public:
		Camera(const std::string& name, float width, float height, float fov = 1.0472f, float zNear = 0.1f, float zFar = 1000.0f) :
			_name(name),
			_fov(fov),
			_zNear(zNear),
			_zFar(zFar),
			_width(width),
			_height(height),
			_location(0.0f,0.0f,0.0f),
			_rotation(0.0f,0.0f,0.0f),
			_projMat(){
			resetProjection(_width, _height, _fov, _zNear, _zFar);
		}

		void resetProjection(float width, float height) {
			_width = width;
			_height = height;
			_projMat = glm::perspective(_fov, aspectRation(), _zNear, _zFar);
		}

		void resetProjection(float width, float height, float fov, float zNear, float zFar) {
			_width = width;
			_height = height;
			_projMat = glm::perspective(fov, aspectRation(), zNear, zFar);
		}

		const std::string& name()const {
			return _name;
		}

		float fov() const {
			return _fov;
		}

		float& fov() {
			return _fov;
		}

		float zNear() const {
			return _zNear;
		}

		float& zNear() {
			return _zNear;
		}

		float zFar() const {
			return _zFar;
		}

		float& zFar() {
			return _zFar;
		}

		float width() const {
			return _width;
		}

		float& width() {
			return _width;
		}

		float height() const {
			return _height;
		}

		float& height() {
			return _height;
		}

		float aspectRation()const {
			return _width / _height;
		}

		const glm::vec3& location() const {
			return _location;
		}

		void moveTo(const glm::vec3& destination) {
			_location = destination;
		}

		void moveRelative(const glm::vec3& units) {
			_location += units;
		}

		const glm::vec3& rotation() const {
			return _rotation;
		}

		void rotateX(float radians) {
			_rotation.x += radians;
		}

		void rotateY(float radians) {
			_rotation.y += radians;
		}

		void rotateZ(float radians) {
			_rotation.z += radians;
		}

		const glm::mat4& projMat()const {
			return _projMat;
		}
	private:
		std::string _name;
		float _fov;
		float _zNear;
		float _zFar;

		float _width;
		float _height;

		glm::vec3 _location;
		glm::vec3 _rotation;
		glm::mat4 _projMat;
	};
}

#endif // !CAMERA_H_
