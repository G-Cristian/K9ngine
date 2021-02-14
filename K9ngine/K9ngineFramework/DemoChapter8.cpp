#include <DemoChapter8.h>

#include <ColorMaterial.h>
#include <ColorMaterialFactory.h>
#include <FloatMaterialComponent.h>
#include <GameObject.h>
#include <GameObjectComponentsPool.h>
#include <LightMaterialShadowRenderer.h>
#include <Model.h>
#include <ModelsPool.h>
#include <OpenGLWrapper.h>
#include <RenderersContainer.h>
#include <RenderingManager.h>
#include <ShadowRenderer.h>
#include <VectorMaterialComponent.h>

#include <GL/glew.h>
#include <memory>
#include <string>
#include <utility>

#include <iostream>

namespace K9 {
	namespace App {
		void DemoChapter8::init() {
			initRenderers();
			initCube();
			initPyramid();
			initTorus();
			initLights();
			initCamera();
		}

		void DemoChapter8::update(double dt) {
			float angle = static_cast<float>(dt) * 10.0f * 3.14f / 180.0f;
			glm::mat4 rot = glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(0.0f, 0.0f, 1.0f));
			_positionalLight->location() = rot * glm::vec4(_positionalLight->location(), 1.0f);
		}

		void DemoChapter8::draw(double dt) {
			clearColorBuffer();
			clearDepthBuffer();

			K9::Graphics::RenderingManager::instance().draw(*_world);
		}

		void DemoChapter8::initRenderers() {
			createAndBindVAO();
			int returnCode;
			std::string returnMsg;
			auto shadowBufferTexture = setupShadowBuffers();

			GLuint shadowRenderingProgram = createRenderingProgram("shadowVertShader.glsl", "shadowFragShader.glsl", returnCode, returnMsg);
			auto shadowRenderer = std::make_shared<K9::Graphics::ShadowRenderer>("ShadowRenderer", shadowRenderingProgram, shadowBufferTexture.first, shadowBufferTexture.second);

			GLuint lightMaterialRenderingProgram = createRenderingProgram("lightMaterialShadowVertShader.glsl", "lightMaterialShadowFragShader.glsl", returnCode, returnMsg);
			std::cout << "returnCode: " << returnCode << " ; returnMsg: " << returnMsg << std::endl;
			auto lightMaterialRenderer = std::make_shared<K9::Graphics::LightMaterialShadowRenderer>("LightMaterialRenderer", lightMaterialRenderingProgram, shadowBufferTexture.first, shadowBufferTexture.second);

			auto lightMaterialShadowRenderer = std::make_shared<K9::Graphics::RenderersContainer>("LightMaterialShadowRenderer");
			lightMaterialShadowRenderer->addRenderer(shadowRenderer);
			lightMaterialShadowRenderer->addRenderer(lightMaterialRenderer);

			K9::Graphics::RenderingManager::instance().addRenderer(lightMaterialShadowRenderer);
			//K9::Graphics::RenderingManager::instance().addRenderer(lightMaterialRenderer);
		}

		void DemoChapter8::initCube() {
			using namespace K9::Components;
			using namespace K9::Graphics;

			auto cubeObject = std::make_shared<GameObject>("Cube");
			cubeObject->scale() = glm::vec3(8.0f, 0.3f, 8.0f);
			cubeObject->position() = glm::vec3(0.0f, -3.0f, 0.0f);
			_world->addGameObject(cubeObject);

			auto cubeRenderingComponent = std::make_shared<RenderingComponent>(cubeObject, "cubeRenderingComponent");
			GameObjectComponentsPool::instance().addRenderingComponentByGameObjectName(cubeObject->name(), cubeRenderingComponent);

			auto cubeModel = ModelsPool::instance().createCube();
			cubeRenderingComponent->setModel(cubeModel);

			auto silverMaterial = ColorMaterialFactory::createSilverColorMaterial();

			auto cubeMaterial = silverMaterial;

			cubeRenderingComponent->setColorMaterial(cubeMaterial);
			//auto renderer = RenderingManager::instance().getRenderer("LightMaterialShadowRenderer");
			auto renderer = RenderingManager::instance().getRenderer("LightMaterialShadowRenderer");

			GameObjectComponentsPool::instance().attachRendererAndRenderingComponent(renderer, cubeRenderingComponent);
		}

		void DemoChapter8::initPyramid() {
			using namespace K9::Components;
			using namespace K9::Graphics;

			auto pyramidObject = std::make_shared<GameObject>("Pyramid");
			//pyramidObject->scale() = glm::vec3(30.0f, 1.0f, 10.0f);
			pyramidObject->position() = glm::vec3(-1.0f, 0.1f, 0.3f);
			//pyramidObject->localRotation() = glm::vec3(0.0f, 3.14f * 25.0f / 180.0f, 0.0f);
			_world->addGameObject(pyramidObject);

			auto pyramidRenderingComponent = std::make_shared<RenderingComponent>(pyramidObject, "pyramidRenderingComponent");
			GameObjectComponentsPool::instance().addRenderingComponentByGameObjectName(pyramidObject->name(), pyramidRenderingComponent);

			auto pyramidModel = ModelsPool::instance().createPyramid();
			pyramidRenderingComponent->setModel(pyramidModel);

			auto goldMaterial = ColorMaterialFactory::createGoldColorMaterial();

			auto pyramidMaterial = goldMaterial;

			pyramidRenderingComponent->setColorMaterial(pyramidMaterial);
			auto renderer = RenderingManager::instance().getRenderer("LightMaterialShadowRenderer");
			GameObjectComponentsPool::instance().attachRendererAndRenderingComponent(renderer, pyramidRenderingComponent);
		}

		void DemoChapter8::initTorus() {
			using namespace K9::Components;
			using namespace K9::Graphics;

			auto torusObject = std::make_shared<GameObject>("Torus");
			//torusObject->scale() = glm::vec3(30.0f, 1.0f, 10.0f);
			torusObject->position() = glm::vec3(1.6f, 0.0f, -0.3f);
			torusObject->localRotation() = glm::vec3(3.14f * 25.0f / 180.0f, 0.0f, 0.0f);
			_world->addGameObject(torusObject);

			auto torusRenderingComponent = std::make_shared<RenderingComponent>(torusObject, "torusRenderingComponent");
			GameObjectComponentsPool::instance().addRenderingComponentByGameObjectName(torusObject->name(), torusRenderingComponent);

			auto torusModel = ModelsPool::instance().createTorus();
			torusRenderingComponent->setModel(torusModel);

			auto pearlMaterial = ColorMaterialFactory::createPearlColorMaterial();

			auto torusMaterial = pearlMaterial;

			torusRenderingComponent->setColorMaterial(torusMaterial);
			auto renderer = RenderingManager::instance().getRenderer("LightMaterialShadowRenderer");
			GameObjectComponentsPool::instance().attachRendererAndRenderingComponent(renderer, torusRenderingComponent);
		}

		void DemoChapter8::initLights() {
			auto ambientLight = std::make_shared <Lighting::AmbientLight>(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
			_world->addAmbientLight(ambientLight);

			glm::vec4 posAmbient = { 0.0f, 0.0f, 0.0f, 1.0f };
			glm::vec4 posDiffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
			glm::vec4 posSpecular = { 1.0f, 1.0f, 1.0f, 1.0f };
			glm::vec3 posPosition = { -3.8f, 2.2f, 1.1f };
			_positionalLight = std::make_shared<Lighting::PositionalLight>(posAmbient, posDiffuse, posSpecular, posPosition);
			_world->addPositionalLight(_positionalLight);
		}

		void DemoChapter8::initCamera() {
			int width, height;
			glfwGetFramebufferSize(_window, &width, &height);
			auto camera = std::make_shared<Camera>("MainCamera", (float)width, (float)height);
			camera->moveTo(glm::vec3(0.0f, 0.2f, 6.0f));

			_world->addCamera(camera);
			_world->setActiveCamera(camera->name());
		}

		std::pair<GLuint, GLuint> DemoChapter8::setupShadowBuffers() {
			GLuint shadowBuffer = 0;
			GLuint shadowTexture = 0;
			int width = 0;
			int height = 0;
			glfwGetFramebufferSize(_window, &width, &height);
			glGenFramebuffers(1, &shadowBuffer);
			glGenTextures(1, &shadowTexture);
			glBindTexture(GL_TEXTURE_2D, shadowTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			return std::make_pair(shadowBuffer, shadowTexture);
		}
	}
}