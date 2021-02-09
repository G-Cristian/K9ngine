#ifndef SPOTLIGHT_H_
#define SPOTLIGHT_H_

#include <glm/glm.hpp>

namespace K9 {
	namespace Lighting {
		class Spotlight {
		public:
			Spotlight(const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular, const glm::vec3& location, const glm::vec3& direction, float cutoff, float exponent) :
				_ambient(ambient),
				_diffuse(diffuse),
				_specular(specular),
				_location(location),
				_direction(direction),
				_cutoff(cutoff),
				_exponent(exponent) {
			}

			~Spotlight() = default;

			//TODO: add getters and setters
		private:
			glm::vec4 _ambient;
			glm::vec4 _diffuse;
			glm::vec4 _specular;
			glm::vec3 _location;
			glm::vec3 _direction;
			float _cutoff;
			float _exponent;
		};
	}
}

#endif // !SPOTLIGHT_H_
