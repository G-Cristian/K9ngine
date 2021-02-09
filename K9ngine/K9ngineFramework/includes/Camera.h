#ifndef CAMERA_H_
#define CAMERA_H_

#include <string>

namespace K9 {
	class Camera {
	public:
		const std::string& name()const {
			return _name;
		}
	private:
		std::string _name;
		float _fov;
		float _nearPlane;
		float _farPlane;
	};
}

#endif // !CAMERA_H_
