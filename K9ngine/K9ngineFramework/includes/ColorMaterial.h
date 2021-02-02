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
				_shininessComponent(other._shininessComponent) {
			}

			virtual ~ColorMaterial() = default;

			ColorMaterial& operator=(const ColorMaterial& other) {
				if (this != &other) {
					Material::operator=(other);
					_ambientComponent = other._ambientComponent;
					_diffuseComponent = other._diffuseComponent;
					_specularComponent = other._specularComponent;
					_shininessComponent = other._shininessComponent;
				}

				return *this;
			}

			const std::string getType() const override {
				return ColorMaterial::materialType;
			}

			std::shared_ptr<MaterialComponent> ambientComponent() {
				return _ambientComponent;
			}

			std::shared_ptr<const MaterialComponent> ambientComponent() const {
				return _ambientComponent;
			}

			std::shared_ptr<MaterialComponent> diffuseComponent() {
				return _diffuseComponent;
			}

			std::shared_ptr<const MaterialComponent> diffuseComponent() const {
				return _diffuseComponent;
			}

			std::shared_ptr<MaterialComponent> specularComponent() {
				return _specularComponent;
			}

			std::shared_ptr<const MaterialComponent> specularComponent() const {
				return _specularComponent;
			}

			std::shared_ptr<MaterialComponent> shininessComponent() {
				return _shininessComponent;
			}

			std::shared_ptr<const MaterialComponent> shininessComponent() const {
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
