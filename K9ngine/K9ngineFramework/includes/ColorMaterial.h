#ifndef _COLORMATERIAL_H_
#define _COLORMATERIAL_H_

#include <Material.h>
#include <MaterialComponent.h>

#include <memory>
#include <string>

namespace K9 {
	namespace Graphics {
		class ColorMaterial :public Material {
		public:
			static const std::string materialType;
			ColorMaterial(const std::string& name) :
				Material(name) {
			}

			ColorMaterial(const ColorMaterial& other) :
				Material(other),
				_ambientComponent(other._ambientComponent),
				_diffuseComponent(other._diffuseComponent),
				_specularComponent(other._specularComponent),
				_shininessComponent(other._shininessComponent)
				/*_ambientComponent(std::make_shared<MaterialComponent>(*(other._ambientComponent))),
				_diffuseComponent(std::make_shared<MaterialComponent>(*(other._diffuseComponent))),
				_specularComponent(std::make_shared<MaterialComponent>(*(other._specularComponent))),
				_shininessComponent(std::make_shared<MaterialComponent>(*(other._shininessComponent)))*/ {
			}

			//TODO: move constructor

			virtual ~ColorMaterial() = default;

			ColorMaterial& operator=(const ColorMaterial& other) {
				if (this != &other) {
					Material::operator=(other);
					_ambientComponent = other._ambientComponent;
					_diffuseComponent = other._diffuseComponent;
					_specularComponent = other._specularComponent;
					_shininessComponent = other._shininessComponent;
					/*_ambientComponent = std::make_shared<MaterialComponent>(*(other._ambientComponent));
					_diffuseComponent = std::make_shared<MaterialComponent>(*(other._diffuseComponent));
					_specularComponent = std::make_shared<MaterialComponent>(*(other._specularComponent));
					_shininessComponent = std::make_shared<MaterialComponent>(*(other._shininessComponent));*/
				}

				return *this;
			}

			//TODO: Move assignment

			const std::string getType() const override {
				return ColorMaterial::materialType;
			}

			void setAmbientComponent(std::shared_ptr<MaterialComponent> ambientComponent) {
				_ambientComponent = ambientComponent;
			}

			std::shared_ptr<const MaterialComponent> getAmbientComponent() const {
				return _ambientComponent;
			}

			void setDiffuseComponent(std::shared_ptr<MaterialComponent> diffuseComponent) {
				_diffuseComponent = diffuseComponent;
			}

			std::shared_ptr<const MaterialComponent> getDiffuseComponent() const {
				return _diffuseComponent;
			}

			void setSpecularComponent(std::shared_ptr<MaterialComponent> specularComponent) {
				_specularComponent = specularComponent;
			}

			std::shared_ptr<const MaterialComponent> getSpecularComponent() const {
				return _specularComponent;
			}

			void setShininessComponent(std::shared_ptr<MaterialComponent> shininessComponent) {
				_shininessComponent = shininessComponent;
			}

			std::shared_ptr<const MaterialComponent> getSshininessComponent() const {
				return _shininessComponent;
			}

		private:
			std::shared_ptr<MaterialComponent> _ambientComponent;
			std::shared_ptr<MaterialComponent> _diffuseComponent;
			std::shared_ptr<MaterialComponent> _specularComponent;
			std::shared_ptr<MaterialComponent> _shininessComponent;
		};
	}
}

#endif // !_COLORMATERIAL_H_
