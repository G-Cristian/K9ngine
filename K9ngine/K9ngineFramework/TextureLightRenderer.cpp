#include <TextureLightRenderer.h>

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
		void TextureLightRenderer::draw(const World& world, const std::set<std::shared_ptr<const K9::Components::RenderingComponent>>& renderingComponents) const {
			glUseProgram(_renderingProgram);
			enableCullFace();
			enableDepthTest();
			setFrontFaceCounterClockwise();
			//setFrontFaceClockwise();
			setDepthFunctionLessEqual();

			GLint material_specularLoc = getUniformLocation(_renderingProgram, "material.specular");
			GLint material_shininessLoc = getUniformLocation(_renderingProgram, "material.shininess");

			GLint mv_matrixLoc = getUniformLocation(_renderingProgram, "mv_matrix");
			GLint proj_matrixLoc = getUniformLocation(_renderingProgram, "proj_matrix");
			GLint norm_matrixLoc = getUniformLocation(_renderingProgram, "norm_matrix");

			glm::mat4 v_mat = initViewMatrix(world);

			auto proj_mat = initProjMatrix(world);
			setUniformMatrix4fv(proj_matrixLoc, glm::value_ptr(proj_mat));

			initLights(world, v_mat);

			for (auto it = renderingComponents.cbegin(); it != renderingComponents.cend(); ++it) {
				auto renderingComponent = *it;
				K9_ASSERT(renderingComponent != nullptr);

				if (renderingComponent != nullptr) {
					glm::mat4 m_mat = initModelMatrix(*renderingComponent);

					glm::mat4 mv_mat = v_mat * m_mat;
					setUniformMatrix4fv(mv_matrixLoc, glm::value_ptr(mv_mat));

					glm::mat4 invTrMat = glm::transpose(glm::inverse(mv_mat));

					setUniformMatrix4fv(norm_matrixLoc, glm::value_ptr(invTrMat));

					auto model = renderingComponent->getModel().lock();
					K9_ASSERT(model != nullptr);
					if (model != nullptr) {
						activateArrayBuffer3f(0, model->getVertexPositions());
						activateArrayBuffer3f(1, model->getVertexNormals());
						activateArrayBuffer2f(2, model->getVertexTextureCoordinates());

						auto colorMaterial = renderingComponent->getColorMaterial();
						K9_ASSERT(colorMaterial != nullptr);
						if (colorMaterial != nullptr) {
							auto ambientComponent = colorMaterial->getAmbientComponent();
							auto texture = dynamic_cast<const TextureMaterialComponent*>(ambientComponent.get());
							K9_ASSERT(texture != nullptr);
							if (texture != nullptr) {
								activateTexture02D(texture->getTexture().lock()->id());
							}

							auto specularComponent = colorMaterial->getSpecularComponent();
							auto specular = dynamic_cast<const VectorMaterialComponent*>(specularComponent.get());

							auto shininessComponent = colorMaterial->getShininessComponent();
							auto shininess = dynamic_cast<const FloatMaterialComponent*>(shininessComponent.get());
							K9_ASSERT(specular != nullptr && shininess != nullptr);
							if (specular != nullptr && shininess != nullptr) {
								initMaterial(material_specularLoc, material_shininessLoc, specular->value(), shininess->value());
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

		glm::mat4 TextureLightRenderer::initModelMatrix(const K9::Components::RenderingComponent& renderingComponent) const{
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

		glm::mat4 TextureLightRenderer::initViewMatrix(const World& world) const {
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

		glm::mat4 TextureLightRenderer::initProjMatrix(const World& world) const {
			glm::mat4 proj_mat(1.0f);

			auto weakCamera = world.getActiveCamera();
			auto camera = weakCamera.lock();
			if (camera != nullptr) {
				proj_mat = camera->projMat();
			}

			return proj_mat;
		}

		void TextureLightRenderer::initLights(const World& world, const glm::mat4& viewMatrix) const {
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

		void TextureLightRenderer::initMaterial(GLint specularLoc, GLint shininessLoc, const glm::vec4& specular, float shininess)const {
			programUniform4fv(_renderingProgram, specularLoc, glm::value_ptr(specular));
			programUniform1f(_renderingProgram, shininessLoc, shininess);
		}
	}
}