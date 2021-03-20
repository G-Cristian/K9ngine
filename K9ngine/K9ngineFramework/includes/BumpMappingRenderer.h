#ifndef BUMPMAPPINGRENDERER_H_
#define BUMPMAPPINGRENDERER_H_

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
		class MaterialComponent;
		class BumpMappingRenderer :public IRenderer {
		public:
			BumpMappingRenderer(const std::string& name, GLuint renderingProgram) :
				IRenderer(name),
				_renderingProgram(renderingProgram) {
			}

			virtual ~BumpMappingRenderer() = default;

			void draw(const World & world, const std::set<std::shared_ptr<const K9::Components::RenderingComponent>>&renderingComponents) const override;
		private:
			glm::mat4 initModelMatrix(const K9::Components::RenderingComponent&) const;
			glm::mat4 initViewMatrix(const World& world) const;
			glm::mat4 initProjMatrix(const World& world) const;
			void initLights(const World& world, const glm::mat4& viewMatrix) const;
			void initVectorColorMaterial(GLint loc, std::shared_ptr<const MaterialComponent> materialComponent)const;
			void initFloatColorMaterial(GLint loc, std::shared_ptr<const MaterialComponent> materialComponent)const;

			GLuint _renderingProgram;
		};
	}
}

#endif // !BUMPMAPPINGRENDERER_H_
