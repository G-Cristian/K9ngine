#include <RenderingComponent.h>

#include <ColorMaterial.h>
#include <Component.h>
#include <GameObjectComponentsPool.h>
#include <K9Debug.h>
#include <Material.h>
#include <Renderer.h>
#include <RenderingManager.h>

#include <GL/glew.h>
#include <map>
#include <memory>
#include <string>
#include <utility>

namespace K9 {
	namespace Components {

		template<class T>
		std::shared_ptr<const T> RenderingComponent::getTMaterial() const {
			std::shared_ptr<const T> retVal = nullptr;
			auto tmp = getMaterial(T::materialType);
			if (tmp != nullptr) {
				retVal = std::dynamic_pointer_cast<const T>(tmp);
				//if we cast to a T material (ColorMaterial, NormalMaterial, etc)
				// it better be able to cast to type T
				// (i.e. the stored material must be of inherited type T even though we store Material pointers)
				K9_ASSERT(retVal != nullptr);
			}

			return retVal;
		}
	}
}