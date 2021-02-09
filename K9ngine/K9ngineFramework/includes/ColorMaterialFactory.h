#ifndef COLORMATERIALFACTORY_H_
#define COLORMATERIALFACTORY_H_

#include <ColorMaterial.h>
#include <memory>

namespace K9 {
	namespace Graphics {
		class ColorMaterialFactory {
		public:
			static std::shared_ptr<ColorMaterial> createGoldColorMaterial();
			static std::shared_ptr<ColorMaterial> createJadeColorMaterial();
			static std::shared_ptr<ColorMaterial> createPearlColorMaterial();
			static std::shared_ptr<ColorMaterial> createSilverColorMaterial();
		};
	}
}

#endif // !COLORMATERIALFACTORY_H_
