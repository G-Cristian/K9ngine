#include <ShadowRenderer.h>

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

#include <iostream>

namespace K9 {
	namespace Graphics {
		void ShadowRenderer::draw(const World& world, const std::set<std::shared_ptr<const K9::Components::RenderingComponent>>& renderingComponents) const {
			const auto& positionalLights = world.getPositionalLights();
			if (positionalLights.size() != 0) {
				auto light = positionalLights[0];
				K9_ASSERT(light != nullptr);
				if (light != nullptr) {
					auto lightVMat = glm::lookAt(light->location(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
					float fov = 90.0f * 3.14f / 180.0f;
					auto wCamera = world.getActiveCamera();
					K9_ASSERT(!wCamera.expired());
					auto camera = wCamera.lock();
					K9_ASSERT(camera != nullptr);
					auto lightPMat = glm::mat4(1.0f);
					if (camera != nullptr) {
						lightPMat = glm::perspective(fov, camera->aspectRation(), 0.1f, 1000.0f);
					}

					glBindFramebuffer(GL_FRAMEBUFFER, _shadowBuffer);
					glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _shadowTexture, 0);

					glDrawBuffer(GL_NONE);

					glEnable(GL_POLYGON_OFFSET_FILL);
					glPolygonOffset(2.0f, 4.0f);

					glUseProgram(_renderingProgram);
					glClear(GL_DEPTH_BUFFER_BIT);

					GLint shadowMVP_matrix_Loc = getUniformLocation(_renderingProgram, "shadowMVP_matrix");

					for (auto it = renderingComponents.cbegin(); it != renderingComponents.cend(); ++it) {
						auto renderingComponent = *it;
						K9_ASSERT(renderingComponent != nullptr);

						if (renderingComponent != nullptr) {
							glm::mat4 m_mat = initModelMatrix(*renderingComponent);

							auto shadowMVP = lightPMat * lightVMat * m_mat;
							setUniformMatrix4fv(shadowMVP_matrix_Loc, glm::value_ptr(shadowMVP));

							auto model = renderingComponent->getModel().lock();
							K9_ASSERT(model != nullptr);
							if (model != nullptr) {
								enableCullFace();
								setFrontFaceCounterClockwise();
								enableDepthTest();
								setDepthFunctionLessEqual();

								activateArrayBuffer3f(0, model->getVertexPositions());

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
			}
		}

		glm::mat4 ShadowRenderer::initModelMatrix(const K9::Components::RenderingComponent& renderingComponent) const {
			auto& gameObject = renderingComponent.gameObject();

			auto m_mat = glm::translate(glm::mat4(1.0f), gameObject.position());

			m_mat *= glm::rotate(glm::mat4(1.0f), gameObject.rotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
			m_mat *= glm::rotate(glm::mat4(1.0f), gameObject.rotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
			m_mat *= glm::rotate(glm::mat4(1.0f), gameObject.rotation().z, glm::vec3(0.0f, 0.0f, 1.0f));

			m_mat *= glm::translate(glm::mat4(1.0f), gameObject.offset());

			m_mat *= glm::rotate(glm::mat4(1.0f), gameObject.localRotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
			m_mat *= glm::rotate(glm::mat4(1.0f), gameObject.localRotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
			m_mat *= glm::rotate(glm::mat4(1.0f), gameObject.localRotation().z, glm::vec3(0.0f, 0.0f, 1.0f));

			m_mat *= glm::scale(glm::mat4(1.0f), gameObject.scale());

			return m_mat;
		}
	}
}