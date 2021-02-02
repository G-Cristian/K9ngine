#ifndef _MATERIALCOMPONENT_H_
#define _MATERIALCOMPONENT_H_

#include <string>

namespace K9 {
	namespace Graphics {
		class MaterialComponent {
		public:
			MaterialComponent() = default;
			virtual ~MaterialComponent() = default;

			virtual const std::string name() const {
				return "MaterialComponent";
			}
		};
	}
}

#endif // !_MATERIALCOMPONENT_H_
