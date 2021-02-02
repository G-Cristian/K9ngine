#ifndef _TEXTUREMATERIALCOMPONENT_H_
#define _TEXTUREMATERIALCOMPONENT_H_

#include <MaterialComponent.h>

#include <GL/glew.h>
#include <string>

namespace K9 {
	namespace Graphics {
		class TextureMaterialComponent :public MaterialComponent {
		public:
			TextureMaterialComponent(GLuint textureId) :
				MaterialComponent(),
				_textureId(textureId)
			{
			}
			virtual ~TextureMaterialComponent() = default;

			const std::string name() const override {
				return "TextureMaterialComponent";
			}

			GLuint textureId()const {
				return _textureId;
			}
		private:
			GLuint _textureId;
		};
	}
}

#endif // !_TEXTUREMATERIALCOMPONENT_H_
