#include <LightMaterialRenderer.h>

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
		void LightMaterialRenderer::draw(const World& world, const std::set<std::shared_ptr<const K9::Components::RenderingComponent>>& renderingComponents) const {
			std::string error;

			glUseProgram(_renderingProgram);

			enableCullFace();
			setFrontFaceCounterClockwise();
			enableDepthTest();
			setDepthFunctionLessEqual();

			auto viewMatrix = initViewMatrix(world);
			initLights(world, viewMatrix);

			GLint proj_matrixLoc = getUniformLocation(_renderingProgram, "proj_matrix");
			auto proj_mat = initProjMatrix(world);
			setUniformMatrix4fv(proj_matrixLoc, glm::value_ptr(proj_mat));

			GLint material_ambientLoc = getUniformLocation(_renderingProgram, "material.ambient");

			GLint material_diffuseLoc = getUniformLocation(_renderingProgram, "material.diffuse");

			GLint material_specularLoc = getUniformLocation(_renderingProgram, "material.specular");

			GLint material_shininessLoc = getUniformLocation(_renderingProgram, "material.shininess");

			GLint mv_matrixLoc = getUniformLocation(_renderingProgram, "mv_matrix");
			
			GLint norm_matrixLoc = getUniformLocation(_renderingProgram, "norm_matrix");

			for (auto it = renderingComponents.cbegin(); it != renderingComponents.cend(); ++it) {
				auto renderingComponent = *it;
				K9_ASSERT(renderingComponent != nullptr);
				if (renderingComponent != nullptr) {
					glm::mat4 modelMatrix = initModelMatrix(*renderingComponent);

					glm::mat4 mv_mat = viewMatrix * modelMatrix;
					setUniformMatrix4fv(mv_matrixLoc, glm::value_ptr(mv_mat));

					glm::mat4 invTrMat = glm::transpose(glm::inverse(mv_mat));
					setUniformMatrix4fv(norm_matrixLoc, glm::value_ptr(invTrMat));

					auto model = renderingComponent->getModel().lock();
					K9_ASSERT(model != nullptr);
					if (model != nullptr) {
						activateArrayBuffer3f(0, model->getVertexPositions());
						activateArrayBuffer3f(1, model->getVertexNormals());

						initVectorColorMaterial(material_ambientLoc, renderingComponent->getColorMaterial()->getAmbientComponent());
						initVectorColorMaterial(material_diffuseLoc, renderingComponent->getColorMaterial()->getDiffuseComponent());
						initVectorColorMaterial(material_specularLoc, renderingComponent->getColorMaterial()->getSpecularComponent());
						initFloatColorMaterial(material_shininessLoc, renderingComponent->getColorMaterial()->getShininessComponent());

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

		glm::mat4 LightMaterialRenderer::initModelMatrix(const K9::Components::RenderingComponent& renderingComponent) const {
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

		glm::mat4 LightMaterialRenderer::initViewMatrix(const World& world) const {
			glm::mat4 viewMatrix(1.0f);
			auto camera = world.getActiveCamera();
			K9_ASSERT(!camera.expired());
			if (!camera.expired()) {
				auto cameraPtr = camera.lock();

				K9_ASSERT(cameraPtr != nullptr);
				if (cameraPtr != nullptr) {
					viewMatrix = glm::translate(glm::mat4(1.0f), -cameraPtr->location());
					viewMatrix *= glm::rotate(glm::mat4(1.0f), -cameraPtr->rotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
					viewMatrix *= glm::rotate(glm::mat4(1.0f), -cameraPtr->rotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
					viewMatrix *= glm::rotate(glm::mat4(1.0f), -cameraPtr->rotation().z, glm::vec3(0.0f, 0.0f, 1.0f));	
				}
			}

			return viewMatrix;
		}

		glm::mat4 LightMaterialRenderer::initProjMatrix(const World& world) const {
			glm::mat4 projMatrix(1.0f);
			auto camera = world.getActiveCamera();
			K9_ASSERT(!camera.expired());
			if (!camera.expired()) {
				auto cameraPtr = camera.lock();

				K9_ASSERT(cameraPtr != nullptr);
				if (cameraPtr != nullptr) {
					projMatrix = cameraPtr->projMat();
				}
			}
			return projMatrix;
		}

		void LightMaterialRenderer::initLights(const World& world, const glm::mat4& viewMatrix) const {
			auto globalAmbientLoc = getUniformLocation(_renderingProgram, "globalAmbient");
			auto light0AmbientLoc = getUniformLocation(_renderingProgram, "light0.ambient");
			auto light0DiffuseLoc = getUniformLocation(_renderingProgram, "light0.diffuse");
			auto light0SpecularLoc = getUniformLocation(_renderingProgram, "light0.specular");
			auto light0PositionLoc = getUniformLocation(_renderingProgram, "light0.position");

			const auto& ambientLights = world.getAmbientLights();
			if (ambientLights.size() > 0) {
				const auto& ambientLight = ambientLights[0];
				programUniform4fv(_renderingProgram, globalAmbientLoc, glm::value_ptr(ambientLight->ambient()));
			}

			const auto& positionalLights = world.getPositionalLights();
			if (positionalLights.size() > 0) {
				const auto& positionalLight = positionalLights[0];
				programUniform4fv(_renderingProgram, light0AmbientLoc, glm::value_ptr(positionalLight->ambient()));
				programUniform4fv(_renderingProgram, light0DiffuseLoc, glm::value_ptr(positionalLight->diffuse()));
				programUniform4fv(_renderingProgram, light0SpecularLoc, glm::value_ptr(positionalLight->specular()));

				glm::vec3 lightPosV = glm::vec3(viewMatrix * glm::vec4(positionalLight->location(), 1.0f));

				programUniform3fv(_renderingProgram, light0PositionLoc, glm::value_ptr(lightPosV));
			}
		}

		void LightMaterialRenderer::initVectorColorMaterial(GLint loc, std::shared_ptr<const MaterialComponent> materialComponent)const{
			K9_ASSERT(materialComponent != nullptr);
			if (materialComponent != nullptr) {
				auto vectorMaterial = dynamic_cast<const VectorMaterialComponent*>(materialComponent.get());
				K9_ASSERT(vectorMaterial != nullptr);
				if (vectorMaterial != nullptr) {
					programUniform4fv(_renderingProgram, loc, glm::value_ptr(vectorMaterial->value()));
				}
			}
		}

		void LightMaterialRenderer::initFloatColorMaterial(GLint loc, std::shared_ptr<const MaterialComponent> materialComponent)const {
			K9_ASSERT(materialComponent != nullptr);
			if (materialComponent != nullptr) {
				auto floatMaterial = dynamic_cast<const FloatMaterialComponent*>(materialComponent.get());
				K9_ASSERT(floatMaterial != nullptr);
				if (floatMaterial != nullptr) {
					programUniform1f(_renderingProgram, loc, floatMaterial->value());
				}
			}
		}
	}
}
