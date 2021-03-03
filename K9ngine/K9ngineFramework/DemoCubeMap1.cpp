#include <DemoCubeMap1.h>
#include <CubeMapRenderer.h>
#include <TextureLightRenderer.h>

#include <ColorMaterial.h>
#include <ColorMaterialFactory.h>
#include <EnvironmentMappingTextureRenderer.h>
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

#include <iostream>
#include <memory>
#include <string>
namespace K9 {
	namespace App {
		void DemoCubeMap1::init() {
			initRenderers();
			initCube();
			initCube2();
			initTorus();
			initLights();
			initCamera();
		}

		void DemoCubeMap1::update(double dt) {
			auto cameraWeak = _world->getActiveCamera();
			auto camera = cameraWeak.lock();
			if (camera != nullptr) {
				float angle = dt * 2.0f * 3.14f / 180.0f;
				camera->rotateX(dt);
				camera->rotateY(dt);
			}
		}

		void DemoCubeMap1::draw(double dt) {
			clearColorBuffer();
			clearDepthBuffer();

			K9::Graphics::RenderingManager::instance().draw(*_world);
		}

		void DemoCubeMap1::initRenderers() {
			using namespace K9::Graphics;
			createAndBindVAO();
			int returnCode;
			std::string outMsg;

			GLuint textureLightRenderingProgram = createRenderingProgram("blinnPhongTextureVertShader.glsl", "blinnPhongTextureFragShader.glsl", returnCode, outMsg);
			K9_ASSERT(returnCode == 1);

			auto textureLightRenderer = std::make_shared<TextureLightRenderer>("TextureLightRenderer", textureLightRenderingProgram);
			RenderingManager::instance().addRenderer(textureLightRenderer);

			GLuint cubeMapRenderingProgram = createRenderingProgram("cubeMapVertShader.glsl", "cubeMapFragShader.glsl", returnCode, outMsg);
			if (returnCode != 1) {
				std::cout << "Error creating cube map shader: " << outMsg << std::endl;
			}
			K9_ASSERT(returnCode == 1);

			auto cubeMapRenderer = std::make_shared<CubeMapRenderer>("CubeMapRenderer", cubeMapRenderingProgram);
			RenderingManager::instance().addRenderer(cubeMapRenderer);

			glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

			auto cubeMapTexture = TexturesPool::instance().createTextureCubeMap("cubeMap",
				"cubeMap\\xp.jpg", "cubeMap\\xn.jpg",
				"cubeMap\\yp.jpg", "cubeMap\\yn.jpg"
				, "cubeMap\\zp.jpg", "cubeMap\\zn.jpg");

			GLuint environmentMappingTextureRenderingProgram = createRenderingProgram("environmentMappingTextureVertShader.glsl", "environmentMappingTextureFragShader.glsl", returnCode, outMsg);
			if (returnCode != 1) {
				std::cout << "Error creating environmentMappingTexture shader: " << outMsg << std::endl;
			}
			K9_ASSERT(returnCode == 1);

			auto environmentMappingTextureRenderer = std::make_shared<EnvironmentMappingTextureRenderer>("EnvironmentMappingTextureRenderer", environmentMappingTextureRenderingProgram, cubeMapTexture);
			RenderingManager::instance().addRenderer(environmentMappingTextureRenderer);
		}

		void DemoCubeMap1::initCube() {
			using namespace K9::Components;
			using namespace K9::Graphics;

			auto cubeObject = std::make_shared<GameObject>("Cube");
			//cubeObject->scale() = glm::vec3(10.0f, 10.0f, 10.0f);
			cubeObject->position() = glm::vec3(0.0f, 0.0f, 0.0f);
			_world->addGameObject(cubeObject);

			auto cubeRenderingComponent = std::make_shared<RenderingComponent>(cubeObject, "cubeRenderingComponent");
			GameObjectComponentsPool::instance().addRenderingComponentByGameObjectName(cubeObject->name(), cubeRenderingComponent);

			auto cubeModel = ModelsPool::instance().createCube();
			cubeRenderingComponent->setModel(cubeModel);


			auto cubeMapTexture = TexturesPool::instance().createTextureCubeMap("cubeMap",
				"cubeMap\\xp.jpg", "cubeMap\\xn.jpg",
				"cubeMap\\yp.jpg", "cubeMap\\yn.jpg"
				, "cubeMap\\zp.jpg", "cubeMap\\zn.jpg");

			//auto cmTxt = cubeMapTexture.lock();
			//if (cmTxt != nullptr) {
			//	cmTxt->setMinificationMipmapModeLinearMipmapLinear();
			//	cmTxt->setMaxTextureFilterAnisotropic();
			//}
			auto textureMaterianComponent = std::make_shared<TextureMaterialComponent>();
			textureMaterianComponent->setTexture(cubeMapTexture);

			//auto silverMaterial = ColorMaterialFactory::createGoldColorMaterial();
			//auto vectorMaterialComponent = silverMaterial->getSpecularComponent();

			auto cubeMaterial = std::make_shared<ColorMaterial>("CubeColorMaterial");
			cubeMaterial->setAmbientComponent(textureMaterianComponent);
			cubeMaterial->setDiffuseComponent(textureMaterianComponent);
			cubeMaterial->setSpecularComponent(textureMaterianComponent);
			cubeMaterial->setShininessComponent(std::make_shared<FloatMaterialComponent>(1.0f));

			cubeRenderingComponent->setColorMaterial(cubeMaterial);
			auto renderer = RenderingManager::instance().getRenderer("CubeMapRenderer");
			GameObjectComponentsPool::instance().attachRendererAndRenderingComponent(renderer, cubeRenderingComponent);
		}

		void DemoCubeMap1::initCube2() {
			using namespace K9::Components;
			using namespace K9::Graphics;

			auto cubeObject = std::make_shared<GameObject>("Cube2");
			cubeObject->scale() = glm::vec3(1.0f, 1.0f, 1.0f);
			cubeObject->position() = glm::vec3(2.0f, 2.0f, -3.0f);
			cubeObject->localRotation() = glm::vec3(0.0f, 20.0f * 3.14f / 180.0f, 0.0f);
			_world->addGameObject(cubeObject);

			auto cubeRenderingComponent = std::make_shared<RenderingComponent>(cubeObject, "cube2RenderingComponent");
			GameObjectComponentsPool::instance().addRenderingComponentByGameObjectName(cubeObject->name(), cubeRenderingComponent);

			auto cubeModel = ModelsPool::instance().createCube();
			cubeRenderingComponent->setModel(cubeModel);


			auto brickTexture = TexturesPool::instance().createTexture("brick1.jpg");

			//auto cmTxt = cubeMapTexture.lock();
			//if (cmTxt != nullptr) {
			//	cmTxt->setMinificationMipmapModeLinearMipmapLinear();
			//	cmTxt->setMaxTextureFilterAnisotropic();
			//}
			auto textureMaterianComponent = std::make_shared<TextureMaterialComponent>();
			textureMaterianComponent->setTexture(brickTexture);

			auto goldMaterial = ColorMaterialFactory::createGoldColorMaterial();
			auto vectorMaterialComponent = goldMaterial->getSpecularComponent();
			auto floatMaterialComponent = goldMaterial->getShininessComponent();

			auto cubeMaterial = std::make_shared<ColorMaterial>("Cube2ColorMaterial");
			cubeMaterial->setAmbientComponent(textureMaterianComponent);
			cubeMaterial->setDiffuseComponent(textureMaterianComponent);
			cubeMaterial->setSpecularComponent(vectorMaterialComponent);
			cubeMaterial->setShininessComponent(floatMaterialComponent);

			cubeRenderingComponent->setColorMaterial(cubeMaterial);
			auto renderer = RenderingManager::instance().getRenderer("EnvironmentMappingTextureRenderer");
			GameObjectComponentsPool::instance().attachRendererAndRenderingComponent(renderer, cubeRenderingComponent);
		}

		void DemoCubeMap1::initTorus() {
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
			auto renderer = RenderingManager::instance().getRenderer("TextureLightRenderer");
			GameObjectComponentsPool::instance().attachRendererAndRenderingComponent(renderer, torusRenderingComponent);
		}

		void DemoCubeMap1::initLights() {
			auto ambientLight = std::make_shared <Lighting::AmbientLight>(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
			_world->addAmbientLight(ambientLight);

			glm::vec4 posAmbient = { 0.0f, 0.0f, 0.0f, 1.0f };
			glm::vec4 posDiffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
			glm::vec4 posSpecular = { 1.0f, 1.0f, 1.0f, 1.0f };
			glm::vec3 posPosition = { 5.0f, 2.0f, 2.0f };
			auto positionalLight = std::make_shared<Lighting::PositionalLight>(posAmbient, posDiffuse, posSpecular, posPosition);
			_world->addPositionalLight(positionalLight);
		}

		void DemoCubeMap1::initCamera() {
			int width, height;
			glfwGetFramebufferSize(_window, &width, &height);
			auto camera = std::make_shared<Camera>("MainCamera", (float)width, (float)height);
			camera->moveTo(glm::vec3(0.0f, 0.0f, 8.5f));

			_world->addCamera(camera);
			_world->setActiveCamera(camera->name());
		}
	}
}