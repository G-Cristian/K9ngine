#include <TexturesPool.h>

#include <GL/glew.h>
#include <K9Debug.h>
#include <OpenGLWrapper.h>
#include <SOIL2/SOIL2.h>

namespace K9 {
	namespace Graphics {
		std::unique_ptr<TexturesPool> TexturesPool::_instance = nullptr;
		TexturesPool::~TexturesPool() {
			// Must have called dispose before
			K9_ASSERT(_texturesPool.empty());
			_texturesPool.clear();
		}

		void TexturesPool::dispose() {
			for (auto it = _texturesPool.begin(); it != _texturesPool.end(); ++it) {
				K9_ASSERT(it->second != nullptr);
				if (it->second != nullptr) {
					K9_ASSERT(it->second.unique());
					//TODO: Maybe should free GPU memory (i.e. Texture::_id)
				}
				it->second = nullptr;
			}
			_texturesPool.clear();

			_instance = nullptr;
		}

		std::weak_ptr<Texture> TexturesPool::createTexture(const std::string& textureFile) {
			auto it = _texturesPool.find(textureFile);
			if (it != _texturesPool.end()) {
				return std::weak_ptr<Texture>(it->second);
			}
			else {
				int returnCode = 0;
				GLuint textureID = loadAndCreateTexture(textureFile.c_str(), &returnCode);
				K9_ASSERT(textureID != 0);
				if (textureID == 0) {
					return std::weak_ptr<Texture>();
				}
				else {
					auto ret = _texturesPool.insert(std::make_pair(textureFile, std::make_shared<Texture>(textureFile, textureID)));
					return std::weak_ptr<Texture>(ret.first->second);
				}
			}
		}

		std::weak_ptr<Texture> TexturesPool::createTextureCubeMap(	const std::string& textureName, 
																	const std::string& right, const std::string& left,
																	const std::string& top, const std::string& bottom,
																	const std::string& front, const std::string& back) {
			auto it = _texturesPool.find(textureName);
			if (it != _texturesPool.end()) {
				return std::weak_ptr<Texture>(it->second);
			}
			else {
				int returnCode = 0;
				GLuint textureID = loadAndCreateCubeMapTexture(right.c_str(), left.c_str(), top.c_str(), bottom.c_str(), front.c_str(), back.c_str(), true, &returnCode);
				K9_ASSERT(returnCode == 1);
				if (returnCode != 1) {
					return std::weak_ptr<Texture>();
				}
				else {
					auto ret = _texturesPool.insert({ textureName, std::make_shared<Texture>(textureName, textureID) });
					return std::weak_ptr<Texture>(ret.first->second);
				}
			}
		}
	}
}