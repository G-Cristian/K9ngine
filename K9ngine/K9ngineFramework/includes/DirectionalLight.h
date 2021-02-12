#ifndef DIRECTIONALLIGHT_H_
#define DIRECTIONALLIGHT_H_

#include <glm/glm.hpp>

namespace K9 {
	namespace Lighting {
		class DirectionalLight {
		public:
			DirectionalLight(const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular, const glm::vec3& direction):
				_ambient(ambient),
				_diffuse(diffuse),
				_specular(specular),
				_direction(direction) {
			}

			~DirectionalLight() = default;

			glm::vec4& ambient() {
				return _ambient;
			}

			const glm::vec4& ambient() const {
				return _ambient;
			}

			glm::vec4& diffuse() {
				return _diffuse;
			}

			const glm::vec4& diffuse() const {
				return _diffuse;
			}

			glm::vec4& specular() {
				return _specular;
			}

			const glm::vec4& specular() const {
				return _specular;
			}

			glm::vec3& direction() {
				return _direction;
			}

			const glm::vec3& direction() const {
				return _direction;
			}
		private:
			glm::vec4 _ambient;
			glm::vec4 _diffuse;
			glm::vec4 _specular;
			glm::vec3 _direction;
		};
	}
}

#endif // !DIRECTIONALLIGHT_H_
