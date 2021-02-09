#ifndef AMBIENTLIGHT_H_
#define AMBIENTLIGHT_H_

#include <glm/glm.hpp>

namespace K9 {
	namespace Lighting {
		class AmbientLight {
		public:
			AmbientLight(const glm::vec4& ambient) :
				_ambient(ambient) {
			}

			~AmbientLight() = default;

			glm::vec4& ambient() {
				return _ambient;
			}

			const glm::vec4& ambient() const {
				return _ambient;
			}
		private:
			glm::vec4 _ambient;
		};
	}
}

#endif // !AMBIENTLIGHT_H_
