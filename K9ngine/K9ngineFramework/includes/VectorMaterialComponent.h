#ifndef _VECTORMATERIALCOMPONENT_H_
#define _VECTORMATERIALCOMPONENT_H_

#include <MaterialComponent.h>

#include <glm/glm.hpp>
#include <string>

namespace K9 {
	namespace Graphics {
		class VectorMaterialComponent :public MaterialComponent {
		public:
			VectorMaterialComponent(const glm::vec4& value) :
				MaterialComponent(),
				_value(value) {
			}

			virtual ~VectorMaterialComponent() = default;

			const std::string name()const override{
				return "VectorMaterialComponent";
			}

			const glm::vec4& value() const {
				return _value;
			}
		private:
			glm::vec4 _value;
		};
	}
}

#endif // !_VECTORMATERIALCOMPONENT_H_
