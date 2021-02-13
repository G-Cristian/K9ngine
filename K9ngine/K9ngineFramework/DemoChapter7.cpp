#include <DemoChapter7.h>
#include <TextureLightRenderer.h>

#include <ColorMaterial.h>
#include <ColorMaterialFactory.h>
#include <FloatMaterialComponent.h>
#include <GameObject.h>
#include <GameObjectComponentsPool.h>
#include <Model.h>
#include <ModelsPool.h>
#include <OpenGLWrapper.h>
#include <RenderingManager.h>
#include <Texture.h>
#include <TextureMaterialComponent.h>
#include <TexturesPool.h>
#include <VectorMaterialComponent.h>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <memory>
#include <string>

namespace K9 {
	namespace App {
		void DemoChapter7::init() {
			initRenderers();
			initCube();
			initTorus();
			initLights();
			initCamera();
		}

		void DemoChapter7::initRenderers() {
			using namespace K9::Graphics;
			createAndBindVAO();
			int returnCode;
			std::string outMsg;
			GLuint textureLightRenderingProgram = createRenderingProgram("blinnPhongTextureVertShader.glsl", "blinnPhongTextureFragShader.glsl", returnCode, outMsg);

			auto textureLightRenderer = std::make_shared<TextureLightRenderer>("BlinnPhongTexture", textureLightRenderingProgram);
			RenderingManager::instance().addRenderer(textureLightRenderer);
		}

		void DemoChapter7::initCube() {
			using namespace K9::Components;
			using namespace K9::Graphics;

			auto cubeObject = std::make_shared<GameObject>("Cube");
			cubeObject->scale() = glm::vec3(30.0f, 1.0f, 10.0f);
			cubeObject->position() = glm::vec3(0.0f, -3.0f, 0.0f);
			_world->addGameObject(cubeObject);

			auto cubeRenderingComponent = std::make_shared<RenderingComponent>(cubeObject, "cubeRenderingComponent");
			GameObjectComponentsPool::instance().addRenderingComponentByGameObjectName(cubeObject->name(), cubeRenderingComponent);

			auto cubeModel = ModelsPool::instance().createCube();
			cubeRenderingComponent->setModel(cubeModel);

			auto brickTexture = TexturesPool::instance().createTexture("brick1.jpg");
			auto brTxt = brickTexture.lock();
			if (brTxt != nullptr) {
				brTxt->setMinificationMipmapModeLinearMipmapLinear();
				brTxt->setMaxTextureFilterAnisotropic();
			}
			auto textureMaterianComponent = std::make_shared<TextureMaterialComponent>();
			textureMaterianComponent->setTexture(brickTexture);

			auto silverMaterial = ColorMaterialFactory::createGoldColorMaterial();
			auto vectorMaterialComponent = silverMaterial->getSpecularComponent();
			auto floatMaterialComponent = silverMaterial->getShininessComponent();

			auto cubeMaterial = std::make_shared<ColorMaterial>("CubeColorMaterial");
			cubeMaterial->setAmbientComponent(textureMaterianComponent);
			cubeMaterial->setDiffuseComponent(textureMaterianComponent);
			cubeMaterial->setSpecularComponent(vectorMaterialComponent);
			cubeMaterial->setShininessComponent(floatMaterialComponent);

			cubeRenderingComponent->setColorMaterial(cubeMaterial);
			auto renderer = RenderingManager::instance().getRenderer("BlinnPhongTexture");
			GameObjectComponentsPool::instance().attachRendererAndRenderingComponent(renderer, cubeRenderingComponent);
		}
		void DemoChapter7::initTorus() {
			using namespace K9::Components;
			using namespace K9::Graphics;

			auto torusObject = std::make_shared<GameObject>("Torus");
			//torusObject->scale() = glm::vec3(30.0f, 1.0f, 10.0f);
			//torusObject->position() = glm::vec3(0.0f, -3.0f, 0.0f);
			torusObject->localRotation() = glm::vec3(3.14f * 35.0f / 180.0f, 0.0f, 0.0f);
			_world->addGameObject(torusObject);

			auto torusRenderingComponent = std::make_shared<RenderingComponent>(torusObject, "torusRenderingComponent");
			GameObjectComponentsPool::instance().addRenderingComponentByGameObjectName(torusObject->name(), torusRenderingComponent);

			auto torusModel = ModelsPool::instance().createTorus();
			torusRenderingComponent->setModel(torusModel);

			auto brickTexture = TexturesPool::instance().createTexture("brick1.jpg");
			auto brTxt = brickTexture.lock();
			if (brTxt != nullptr) {
				brTxt->setMinificationMipmapModeLinearMipmapLinear();
				brTxt->setMaxTextureFilterAnisotropic();
			}
			auto textureMaterianComponent = std::make_shared<TextureMaterialComponent>();
			textureMaterianComponent->setTexture(brickTexture);

			auto goldMaterial = ColorMaterialFactory::createGoldColorMaterial();
			auto vectorMaterialComponent = goldMaterial->getSpecularComponent();
			auto floatMaterialComponent = goldMaterial->getShininessComponent();

			auto torusMaterial = std::make_shared<ColorMaterial>("TorusColorMaterial");
			torusMaterial->setAmbientComponent(textureMaterianComponent);
			torusMaterial->setDiffuseComponent(textureMaterianComponent);
			torusMaterial->setSpecularComponent(vectorMaterialComponent);
			torusMaterial->setShininessComponent(floatMaterialComponent);

			torusRenderingComponent->setColorMaterial(torusMaterial);
			auto renderer = RenderingManager::instance().getRenderer("BlinnPhongTexture");
			GameObjectComponentsPool::instance().attachRendererAndRenderingComponent(renderer, torusRenderingComponent);
		}

		void DemoChapter7::initLights() {
			auto ambientLight = std::make_shared <Lighting::AmbientLight>(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
			_world->addAmbientLight(ambientLight);

			glm::vec4 posAmbient = { 0.0f, 0.0f, 0.0f, 1.0f };
			glm::vec4 posDiffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
			glm::vec4 posSpecular = { 1.0f, 1.0f, 1.0f, 1.0f };
			glm::vec3 posPosition = { 5.0f, 2.0f, 2.0f };
			auto positionalLight = std::make_shared<Lighting::PositionalLight>(posAmbient, posDiffuse, posSpecular, posPosition);
			_world->addPositionalLight(positionalLight);
		}

		void DemoChapter7::initCamera() {
			int width, height;
			glfwGetFramebufferSize(_window, &width, &height);
			auto camera = std::make_shared<Camera>("MainCamera", (float)width, (float)height);
			camera->moveTo(glm::vec3(0.0f, 0.0f, 1.5f));
			
			_world->addCamera(camera);
			_world->setActiveCamera(camera->name());
		}

		void DemoChapter7::update(double dt) {
			auto torus = _world->getGameObject("Torus");
			auto rot = torus->localRotation();
			rot.y = dt * 3.14f * 20.0f / 180.0f;
			torus->localRotation()=glm::vec3(rot.x, rot.y, rot.z);
		}

		void DemoChapter7::draw(double dt) {
			clearColorBuffer();
			clearDepthBuffer();

			K9::Graphics::RenderingManager::instance().draw(*_world);
		}
	}
}
