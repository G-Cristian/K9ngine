#ifndef SHADOWRENDERER_H_
#define SHADOWRENDERER_H_

#include <RenderingComponent.h>
#include <Renderer.h>
#include <World.h>

#include <set>
#include <string>
#include <memory>

namespace K9 {
	namespace Graphics {
		class ShadowRenderer :public IRenderer {
		public:
			ShadowRenderer(const std::string& name, GLuint renderingProgram, GLuint shadowBuffer, GLuint shadowTexture) :
				IRenderer(name),
				_renderingProgram(renderingProgram),
				_shadowBuffer(shadowBuffer),
				_shadowTexture(shadowTexture){
			}

			virtual ~ShadowRenderer() = default;

			void draw(const World& world, const std::set<std::shared_ptr<const K9::Components::RenderingComponent>>& renderingComponents) const override;
		private:
			glm::mat4 initModelMatrix(const K9::Components::RenderingComponent&) const;
			//glm::mat4 initViewMatrix(const World& world) const;
			//glm::mat4 initProjMatrix(const World& world) const;
			//void initLights(const World& world, const glm::mat4& viewMatrix) const;
			//void initMaterial(GLint specularLoc, GLint shininessLoc, const glm::vec4& specular, float shininess)const;

			GLuint _renderingProgram;
			GLuint _shadowBuffer;
			GLuint _shadowTexture;
		};
	}
}

#endif // !SHADOWRENDERER_H_
