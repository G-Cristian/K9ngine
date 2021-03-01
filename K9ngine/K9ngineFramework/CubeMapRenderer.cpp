#include <CubeMapRenderer.h>

#include <ColorMaterial.h>
#include <FloatMaterialComponent.h>
#include <K9Debug.h>
#include <OpenGLWrapper.h>
#include <RenderingComponent.h>
#include <TextureMaterialComponent.h>
#include <VectorMaterialComponent.h>
#include <World.h>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <set>

namespace K9 {
	namespace Graphics {
		void CubeMapRenderer::draw(const World& world, const std::set<std::shared_ptr<const K9::Components::RenderingComponent>>& renderingComponents) const {
			glUseProgram(_renderingProgram);
			enableCullFace();
			disableDepthTest();
			//cube is counter clockwise so inverse the orientation for the cube map
			setFrontFaceClockwise();

			
			GLint v_matrixLoc = getUniformLocation(_renderingProgram, "v_matrix");
			GLint proj_matrixLoc = getUniformLocation(_renderingProgram, "proj_matrix");

			glm::mat4 v_mat = initViewMatrix(world);

			auto proj_mat = initProjMatrix(world);
			setUniformMatrix4fv(proj_matrixLoc, glm::value_ptr(proj_mat));

			for (auto it = renderingComponents.cbegin(); it != renderingComponents.cend(); ++it) {
				auto renderingComponent = *it;
				K9_ASSERT(renderingComponent != nullptr);

				if (renderingComponent != nullptr) {
					setUniformMatrix4fv(v_matrixLoc, glm::value_ptr(v_mat));

					auto model = renderingComponent->getModel().lock();
					K9_ASSERT(model != nullptr);
					if (model != nullptr) {
						activateArrayBuffer3f(0, model->getVertexPositions());

						auto colorMaterial = renderingComponent->getColorMaterial();
						K9_ASSERT(colorMaterial != nullptr);
						if (colorMaterial != nullptr) {
							auto ambientComponent = colorMaterial->getAmbientComponent();
							auto texture = dynamic_cast<const TextureMaterialComponent*>(ambientComponent.get());
							K9_ASSERT(texture != nullptr);
							if (texture != nullptr) {
								glActiveTexture(GL_TEXTURE0);
								glBindTexture(GL_TEXTURE_CUBE_MAP, texture->getTexture().lock()->id());
							}
						}

						if (model->usesIndices()) {
							drawTrianglesUsingIndices(model->getIndices(), model->numberOfElements());
						}
						else {
							drawTrianglesUsingVertices(model->numberOfElements());
						}
					}
				}
			}
		}

		glm::mat4 CubeMapRenderer::initViewMatrix(const World& world) const {
			glm::mat4 v_mat(1.0f);

			auto weakCamera = world.getActiveCamera();
			auto camera = weakCamera.lock();
			if (camera != nullptr) {
				v_mat = glm::translate(glm::mat4(1.0f), -camera->location());
				v_mat *= glm::rotate(glm::mat4(1.0f), -camera->rotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
				v_mat *= glm::rotate(glm::mat4(1.0f), -camera->rotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
				v_mat *= glm::rotate(glm::mat4(1.0f), -camera->rotation().z, glm::vec3(0.0f, 0.0f, 1.0f));
			}

			return v_mat;
		}

		glm::mat4 CubeMapRenderer::initProjMatrix(const World& world) const {
			glm::mat4 proj_mat(1.0f);

			auto weakCamera = world.getActiveCamera();
			auto camera = weakCamera.lock();
			if (camera != nullptr) {
				proj_mat = camera->projMat();
			}

			return proj_mat;
		}
	}
}