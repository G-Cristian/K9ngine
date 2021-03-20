#ifndef NORMALMATERIAL_H_
#define NORMALMATERIAL_H_

#include <Material.h>
#include <TextureMaterialComponent.h>

#include <memory>
#include <string>

namespace K9 {
	namespace Graphics {
		class NormalMaterial :public Material {
		public:
			static const std::string materialType;

			NormalMaterial(const std::string& name) :
				Material(name) {
			}

			NormalMaterial(const NormalMaterial& other) :
				Material(other) {
			}

			//TODO: move constructor

			virtual ~NormalMaterial() = default;

			NormalMaterial& operator=(const NormalMaterial& other) {
				if (this != &other) {
					Material::operator=(other);
					//TODO: complete
				}

				return *this;
			}

			//TODO: Move assignment

			const std::string getType() const override {
				return NormalMaterial::materialType;
			}

			void setTexture(std::shared_ptr<const TextureMaterialComponent> texture) {
				_texture = texture;
			}

			std::shared_ptr<const TextureMaterialComponent> getTexture() const {
				return _texture;
			}
		private:
			std::shared_ptr<const TextureMaterialComponent> _texture;
		};
	}
}

#endif // !NORMALMATERIAL_H_
