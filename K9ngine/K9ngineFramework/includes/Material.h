#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <string>

namespace K9 {
	namespace Graphics {
		class MaterialComponent;
		class Material {
		public:
			static const std::string materialType;
			Material(const std::string& name) :
				_name(name) {
			}

			Material(const Material& other) :
				_name(other._name) {
			}

			virtual ~Material() = default;

			Material& operator=(const Material& other) {
				_name = other._name;
			}

			const std::string& name()const {
				return _name;
			}

			virtual const std::string getType() const;

		protected:

			std::string _name;
		};
	}
}

#endif // !_MATERIAL_H_
