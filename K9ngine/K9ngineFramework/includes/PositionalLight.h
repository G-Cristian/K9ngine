#ifndef POSITIONALLIGHT_H_
#define POSITIONALLIGHT_H_

#include <glm/glm.hpp>

namespace K9 {
	namespace Lighting {
		class PositionalLight {
			PositionalLight(const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular, const glm::vec3& location):
				_ambient(ambient),
				_diffuse(diffuse),
				_specular(specular),
				_location(location) {
			}
			~PositionalLight() = default;

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

			glm::vec3& location() {
				return _location;
			}

			const glm::vec3& location() const {
				return _location;
			}
		public:
		private:
			glm::vec4 _ambient;
			glm::vec4 _diffuse;
			glm::vec4 _specular;
			glm::vec3 _location;
		};
	}
}

#endif // !POSITIONALLIGHT_H_
