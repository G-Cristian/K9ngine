#ifndef _RENDERINGCOMPONENT_H_
#define _RENDERINGCOMPONENT_H_

#include <K9Debug.h>

#include <ColorMaterial.h>
#include <Component.h>
#include <Material.h>

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

			void setAttribute(const std::string& attributeName, GLuint attributeValue) {
				_attributes[attributeName] = attributeValue;
			}

			const GLuint getAttribute(const std::string& attributeName) {
				auto attrIt = _attributes.find(attributeName);
				K9_ASSERT(attrIt != _attributes.end());
				return attrIt->second;
			}

			void setColorMaterial(std::shared_ptr<K9::Graphics::ColorMaterial> colorMaterial) {
				setMaterial(colorMaterial);
			}

			std::shared_ptr<const K9::Graphics::ColorMaterial> getColorMaterial() const {
				return getTMaterial<K9::Graphics::ColorMaterial>();
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
			std::shared_ptr<const T> getTMaterial() const;

			std::shared_ptr<const K9::Graphics::IRenderer> _renderer;
			std::map<std::string, GLuint> _attributes;
			std::map<std::string, std::shared_ptr<K9::Graphics::Material>> _materials;
		};
	}
}

#endif // !_RENDERINGCOMPONENT_H_
