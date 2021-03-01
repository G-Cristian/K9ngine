#ifndef CUBEMAPRENDERER_H_
#define CUBEMAPRENDERER_H_

#include <Renderer.h>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <memory>
#include <set>
#include <string>

namespace K9 {
	namespace Components {
		class RenderingComponent;
	}
}

namespace K9 {
	class World;

	namespace Graphics {
		class CubeMapRenderer :public IRenderer {
		public:
			CubeMapRenderer(const std::string& name, GLuint renderingProgram) :
				IRenderer(name),
				_renderingProgram(renderingProgram) {
			}

			virtual ~CubeMapRenderer() = default;

			void draw(const World& world, const std::set<std::shared_ptr<const K9::Components::RenderingComponent>>& renderingComponents) const override;
		private:
			glm::mat4 initViewMatrix(const World& world) const;
			glm::mat4 initProjMatrix(const World& world) const;

			GLuint _renderingProgram;
		};
	}
}

#endif // !CUBEMAPRENDERER_H_
