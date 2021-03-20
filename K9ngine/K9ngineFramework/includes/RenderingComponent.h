#ifndef _RENDERINGCOMPONENT_H_
#define _RENDERINGCOMPONENT_H_

#include <K9Debug.h>

#include <ColorMaterial.h>
#include <Component.h>
#include <Material.h>
#include <Model.h>
#include <NormalMaterial.h>

#include <GL/glew.h>
#include <map>
#include <memory>
#include <string>
#include <utility>

namespace K9 {
	namespace Graphics {
		class IRenderer;
	}
}

namespace K9 {
	class GameObject;

	namespace Components {
		class RenderingComponent :public IComponent {
		public:
			RenderingComponent(std::shared_ptr<GameObject> gameObject, const std::string& name) :
				IComponent(gameObject, name){
			}

			virtual ~RenderingComponent() = default;

			const K9::Graphics::IRenderer& renderer() const {
				K9_ASSERT(_renderer != nullptr);

				return *_renderer;
			}

			void setModel(std::weak_ptr<const K9::Graphics::Model> model) {
				_model = model;
			}

			std::weak_ptr<const K9::Graphics::Model> getModel() const{
				return _model;
			}

			void setColorMaterial(std::shared_ptr<K9::Graphics::ColorMaterial> colorMaterial) {
				setMaterial(colorMaterial);
			}

			std::shared_ptr<const K9::Graphics::ColorMaterial> getColorMaterial() const {
				return getTMaterial<K9::Graphics::ColorMaterial>();
			}

			void setNormalMaterial(std::shared_ptr<K9::Graphics::NormalMaterial> normalMaterial) {
				setMaterial(normalMaterial);
			}

			std::shared_ptr<const K9::Graphics::NormalMaterial> getNormalMaterial() const {
				return getTMaterial<K9::Graphics::NormalMaterial>();
			}

			friend class GameObjectComponentsPool;
		private:
			void setMaterial(std::shared_ptr<K9::Graphics::Material> material) {
				K9_ASSERT(material != nullptr);
				_materials[material->getType()] = material;
			}

			std::shared_ptr<const K9::Graphics::Material> getMaterial(const std::string& materialType)const {
				auto it = _materials.find(materialType);
				return (it == _materials.end() ? nullptr : it->second);
			}

			template<class T>
			std::shared_ptr<const T> getTMaterial() const {
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

			std::shared_ptr<const K9::Graphics::IRenderer> _renderer;
			std::weak_ptr<const K9::Graphics::Model> _model;
			std::map<std::string, std::shared_ptr<K9::Graphics::Material>> _materials;
		};
	}
}

#endif // !_RENDERINGCOMPONENT_H_
