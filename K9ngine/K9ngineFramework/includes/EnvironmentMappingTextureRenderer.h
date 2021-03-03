#ifndef ENVIRONMENTMAPPINGTEXTURERENDERER_H_
#define ENVIRONMENTMAPPINGTEXTURERENDERER_H_

#include <Renderer.h>
#include <Texture.h>

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
		class EnvironmentMappingTextureRenderer :public IRenderer {
		public:
			EnvironmentMappingTextureRenderer(const std::string& name, GLuint renderingProgram, const std::weak_ptr<const Texture>& cubeMapTexture) :
				IRenderer(name),
				_renderingProgram(renderingProgram),
				_cubeMapTexture(cubeMapTexture){
			}

			virtual ~EnvironmentMappingTextureRenderer() = default;

			void draw(const World & world, const std::set<std::shared_ptr<const K9::Components::RenderingComponent>>&renderingComponents) const override;
		private:
			glm::mat4 initModelMatrix(const K9::Components::RenderingComponent&) const;
			glm::mat4 initViewMatrix(const World & world) const;
			glm::mat4 initProjMatrix(const World & world) const;
			void initLights(const World & world, const glm::mat4 & viewMatrix) const;
			void initMaterial(GLint specularLoc, GLint shininessLoc, const glm::vec4 & specular, float shininess)const;

			GLuint _renderingProgram;
			std::weak_ptr<const Texture> _cubeMapTexture;
		};
	}
}

#endif // !ENVIRONMENTMAPPINGTEXTURERENDERER_H_
