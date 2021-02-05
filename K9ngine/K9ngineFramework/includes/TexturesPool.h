#ifndef _TEXTURESPOOL_H_
#define _TEXTURESPOOL_H_

#include <Texture.h>

#include <map>
#include <memory>
#include <GL/glew.h>
#include <string>

namespace K9 {
	namespace Graphics {
		class TexturesPool {
		public:
			~TexturesPool();

			static TexturesPool& instance() {
				if (_instance == nullptr) {
					_instance = std::unique_ptr<TexturesPool>(new TexturesPool());
				}

				return *_instance;
			}

			TexturesPool(const TexturesPool&) = delete;
			TexturesPool(TexturesPool&&) noexcept = delete;
			TexturesPool& operator=(const TexturesPool&) = delete;
			TexturesPool& operator=(TexturesPool&&) noexcept = delete;

			std::weak_ptr<Texture> createTexture(const std::string& textureFile);

		private:
			TexturesPool() = default;
			static std::unique_ptr<TexturesPool> _instance;

			std::map<std::string, std::shared_ptr<Texture>> _texturesPool;
		};
	}
}

#endif // !_TEXTURESPOOL_H_
