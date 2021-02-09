#ifndef _TEXTUREMATERIALCOMPONENT_H_
#define _TEXTUREMATERIALCOMPONENT_H_

#include <MaterialComponent.h>
#include <Texture.h>

#include <GL/glew.h>
#include <memory>
#include <string>

namespace K9 {
	namespace Graphics {
		class TextureMaterialComponent :public MaterialComponent {
		public:
			TextureMaterialComponent() :
				MaterialComponent(),
				_texture()
			{
			}
			virtual ~TextureMaterialComponent() = default;

			const std::string name() const override {
				return "TextureMaterialComponent";
			}

			const std::weak_ptr<const Texture>& getTexture()const {
				return _texture;
			}

			void setTexture(const std::weak_ptr<const Texture>& texture) {
				_texture = texture;
			}
		private:
			std::weak_ptr<const Texture> _texture;
		};
	}
}

#endif // !_TEXTUREMATERIALCOMPONENT_H_
