#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <GL/glew.h>
#include <string>

namespace K9 {
	namespace Graphics {
		class Texture {
		public:
			Texture(const std::string& name, GLuint id) :
				_name(name),
				_id(id) {
			}
			~Texture() = default;

			const std::string& name()const {
				return _name;
			}

			GLuint id() const {
				return _id;
			}

			void setMinificationMipmapModeLinearMipmapLinear() {
				setMinificationMipmapMode(GL_LINEAR_MIPMAP_LINEAR);
			}

			bool setMaxTextureFilterAnisotropic();

		private:
			void setMinificationMipmapMode(GLint mode);

			std::string _name;
			GLuint _id;
		};
	}
}

#endif // !TEXTURE_H_
