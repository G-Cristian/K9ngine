#include <LightMaterialShadowRenderer.h>

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
		void LightMaterialShadowRenderer::draw(const World& world, const std::set<std::shared_ptr<const K9::Components::RenderingComponent>>& renderingComponents) const {
			glDisable(GL_POLYGON_OFFSET_FILL);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			activateTexture02D(_shadowTexture);
			glDrawBuffer(GL_FRONT);

			glm::mat4 b(0.5f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.5f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.5f, 0.0f,
				0.5f, 0.5f, 0.5f, 1.0f);

			glUseProgram(_renderingProgram);
			glClear(GL_DEPTH_BUFFER_BIT);

			GLint material_ambientLoc = getUniformLocation(_renderingProgram, "material.ambient");
			GLint material_diffuseLoc = getUniformLocation(_renderingProgram, "material.diffuse");
			GLint material_specularLoc = getUniformLocation(_renderingProgram, "material.specular");
			GLint material_shininessLoc = getUniformLocation(_renderingProgram, "material.shininess");

			GLint mv_matrixLoc = getUniformLocation(_renderingProgram, "mv_matrix");
			GLint proj_matrixLoc = getUniformLocation(_renderingProgram, "proj_matrix");
			GLint norm_matrixLoc = getUniformLocation(_renderingProgram, "norm_matrix");
			GLint shadowMVP_matrix_Loc = getUniformLocation(_renderingProgram, "shadowMVP2_matrix");

			glm::mat4 v_mat = initViewMatrix(world);

			auto proj_mat = initProjMatrix(world);
			setUniformMatrix4fv(proj_matrixLoc, glm::value_ptr(proj_mat));

			initLights(world, v_mat);

			auto wCamera = world.getActiveCamera();
			K9_ASSERT(!wCamera.expired());
			auto camera = wCamera.lock();
			K9_ASSERT(camera != nullptr);
			if (camera != nullptr) {
				initShadowAttributes(2.5f, glm::vec2(camera->width(), camera->height()), GL_FALSE);
			}
			else {
				initShadowAttributes(2.5f, glm::vec2(1000.0, 1000.0), GL_FALSE);
			}
			

			const auto& positionalLights = world.getPositionalLights();
			auto lightVMat = glm::mat4(1.0f);
			auto lightPMat = glm::mat4(1.0f);
			if (positionalLights.size() != 0) {
				auto light = positionalLights[0];
				K9_ASSERT(light != nullptr);
				if (light != nullptr) {
					lightVMat = glm::lookAt(light->location(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
					float fov = 90.0f * 3.14f / 180.0f;
					if (camera != nullptr) {
						lightPMat = glm::perspective(fov, camera->aspectRation(), 0.1f, 1000.0f);
					}
				}
			}

			for (auto it = renderingComponents.cbegin(); it != renderingComponents.cend(); ++it) {
				auto renderingComponent = *it;
				K9_ASSERT(renderingComponent != nullptr);

				if (renderingComponent != nullptr) {
					glm::mat4 m_mat = initModelMatrix(*renderingComponent);

					auto shadowMVP = b* lightPMat * lightVMat * m_mat;
					setUniformMatrix4fv(shadowMVP_matrix_Loc, glm::value_ptr(shadowMVP));

					glm::mat4 mv_mat = v_mat * m_mat;
					setUniformMatrix4fv(mv_matrixLoc, glm::value_ptr(mv_mat));

					glm::mat4 invTrMat = glm::transpose(glm::inverse(mv_mat));

					setUniformMatrix4fv(norm_matrixLoc, glm::value_ptr(invTrMat));

					auto model = renderingComponent->getModel().lock();
					K9_ASSERT(model != nullptr);
					if (model != nullptr) {
						activateArrayBuffer3f(0, model->getVertexPositions());
						activateArrayBuffer3f(1, model->getVertexNormals());

						auto colorMaterial = renderingComponent->getColorMaterial();
						K9_ASSERT(colorMaterial != nullptr);
						if (colorMaterial != nullptr) {
							auto ambientComponent = colorMaterial->getAmbientComponent();
							auto ambient = dynamic_cast<const VectorMaterialComponent*>(ambientComponent.get());

							auto diffuseComponent = colorMaterial->getDiffuseComponent();
							auto diffuse = dynamic_cast<const VectorMaterialComponent*>(diffuseComponent.get());

							auto specularComponent = colorMaterial->getSpecularComponent();
							auto specular = dynamic_cast<const VectorMaterialComponent*>(specularComponent.get());

							auto shininessComponent = colorMaterial->getShininessComponent();
							auto shininess = dynamic_cast<const FloatMaterialComponent*>(shininessComponent.get());
							K9_ASSERT(ambient != nullptr && diffuse != nullptr && specular != nullptr && shininess != nullptr);
							if (ambient != nullptr && diffuse != nullptr && specular != nullptr && shininess != nullptr) {
								initMaterial(material_ambientLoc, material_diffuseLoc, material_specularLoc, material_shininessLoc, ambient->value(), diffuse->value(), specular->value(), shininess->value());
							}

						}

						enableCullFace();
						setFrontFaceCounterClockwise();
						enableDepthTest();
						setDepthFunctionLessEqual();
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

		glm::mat4 LightMaterialShadowRenderer::initModelMatrix(const K9::Components::RenderingComponent& renderingComponent) const {
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

		glm::mat4 LightMaterialShadowRenderer::initViewMatrix(const World& world) const {
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

		glm::mat4 LightMaterialShadowRenderer::initProjMatrix(const World& world) const {
			glm::mat4 proj_mat(1.0f);

			auto weakCamera = world.getActiveCamera();
			auto camera = weakCamera.lock();
			if (camera != nullptr) {
				proj_mat = camera->projMat();
			}

			return proj_mat;
		}

		void LightMaterialShadowRenderer::initLights(const World& world, const glm::mat4& viewMatrix) const {
			GLint globalAmbientLoc = getUniformLocation(_renderingProgram, "globalAmbient");
			GLint light0_ambientLoc = getUniformLocation(_renderingProgram, "light0.ambient");
			GLint light0_diffuseLoc = getUniformLocation(_renderingProgram, "light0.diffuse");
			GLint light0_specularLoc = getUniformLocation(_renderingProgram, "light0.specular");
			GLint light0_positionLoc = getUniformLocation(_renderingProgram, "light0.position");

			const auto& ambientLights = world.getAmbientLights();
			if (ambientLights.size() > 0) {
				const auto& ambientLight = ambientLights[0];
				programUniform4fv(_renderingProgram, globalAmbientLoc, glm::value_ptr(ambientLight->ambient()));
			}

			const auto& positionalLights = world.getPositionalLights();
			if (positionalLights.size() > 0) {
				const auto& positionalLight = positionalLights[0];
				programUniform4fv(_renderingProgram, light0_ambientLoc, glm::value_ptr(positionalLight->ambient()));
				programUniform4fv(_renderingProgram, light0_diffuseLoc, glm::value_ptr(positionalLight->diffuse()));
				programUniform4fv(_renderingProgram, light0_specularLoc, glm::value_ptr(positionalLight->specular()));

				glm::vec3 lightPosV = glm::vec3(viewMatrix * glm::vec4(positionalLight->location(), 1.0f));

				programUniform3fv(_renderingProgram, light0_positionLoc, glm::value_ptr(lightPosV));
			}
		}

		void LightMaterialShadowRenderer::initMaterial(GLint ambientLoc, GLint diffuseLoc, GLint specularLoc, GLint shininessLoc, const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular, float shininess)const {
			programUniform4fv(_renderingProgram, ambientLoc, glm::value_ptr(ambient));
			programUniform4fv(_renderingProgram, diffuseLoc, glm::value_ptr(diffuse));
			programUniform4fv(_renderingProgram, specularLoc, glm::value_ptr(specular));
			programUniform1f(_renderingProgram, shininessLoc, shininess);
		}

		void LightMaterialShadowRenderer::initShadowAttributes(GLfloat shadowWidth, glm::vec2 windowSize, GLboolean betterPerformance)const {
			GLint shadowWidthLoc = getUniformLocation(_renderingProgram, "shadowWidth");
			programUniform1f(_renderingProgram, shadowWidthLoc, shadowWidth);

			GLint windowSizeLoc = getUniformLocation(_renderingProgram, "windowSize");
			programUniform2fv(_renderingProgram, windowSizeLoc, glm::value_ptr(windowSize));

			GLint betterPerformanceLoc = getUniformLocation(_renderingProgram, "betterPerformance");
			programUniform1i(_renderingProgram, betterPerformanceLoc, betterPerformance);
		}
	}
}