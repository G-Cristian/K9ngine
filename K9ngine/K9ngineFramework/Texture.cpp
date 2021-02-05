#include <Texture.h>

#include <GL/glew.h>

namespace K9{
	namespace Graphics {
		bool Texture::setMaxTextureFilterAnisotropic() {
			bool available = false;
			if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
				glBindTexture(GL_TEXTURE_2D, _id);
				GLfloat anisoSetting = 0.0f;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLint)anisoSetting);

				available = true;
			}

			return available;
		}

		void Texture::setMinificationMipmapMode(GLint mode) {
			glBindTexture(GL_TEXTURE_2D, _id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}
}