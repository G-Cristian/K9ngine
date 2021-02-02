#ifndef _FLOATMATERIALCOMPONENT_H_
#define _FLOATMATERIALCOMPONENT_H_

#include <MaterialComponent.h>

#include <string>

namespace K9 {
	namespace Graphics {
		class FloatMaterialComponent :public MaterialComponent {
		public:
			FloatMaterialComponent(float value) :
				MaterialComponent(),
				_value(value) {
			}
			virtual ~FloatMaterialComponent() = default;

			const std::string name() const override{
				return "FloatMaterialComponent";
			}

			float value() const {
				return _value;
			}
		private:
			float _value;
		};
	}
}

#endif // !_FLOATMATERIALCOMPONENT_H_
