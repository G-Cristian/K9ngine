#include <K9Debug.h>
#include <Material.h>

namespace K9 {
	namespace Graphics {
		const std::string Material::materialType = "NotImplementedMaterial";

		const std::string Material::getType() const {
			//This must never be called in this class, only inherited should be called
			//therefore, force false
			K9_ASSERT(Material::materialType != Material::materialType);

			return Material::materialType;
		}
	}
}