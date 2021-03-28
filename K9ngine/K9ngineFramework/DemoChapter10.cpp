#include <DemoChapter10.h>

#include <BumpMappingRenderer.h>
#include <ColorMaterial.h>
#include <ColorMaterialFactory.h>
#include <FloatMaterialComponent.h>
#include <GameObject.h>
#include <GameObjectComponentsPool.h>
#include <Keyboard.h>
#include <LightMaterialRenderer.h>
#include <Model.h>
#include <ModelsPool.h>
#include <NormalMappingRenderer.h>
#include <NormalMaterial.h>
#include <OpenGLWrapper.h>
#include <RenderersContainer.h>
#include <RenderingManager.h>
#include <ShadowRenderer.h>
#include <TexturesPool.h>
#include <VectorMaterialComponent.h>

#include <GL/glew.h>
#include <memory>
#include <string>
#include <utility>

#include <iostream>

namespace K9 {
	namespace App {
		void DemoChapter10::init() {
			initRenderers();
			loadTextures();
			initSphere1();
			initSphere2();
			initSphere3();
			initLights();
			initCamera();
		}

		void DemoChapter10::update(double dt) {
			float rot = dt * 45.0f * 3.14f / 180.0f;

			auto weakCamera = _world->getActiveCamera();
			auto camera = weakCamera.lock();
			if (camera) {
				if (K9::Input::Keyboard::isPressed(K9::Input::Keys::D)) {
					camera->moveRelative(glm::vec3(0.5f, 0.0f, 0.0f));
				}

				if (K9::Input::Keyboard::isPressed(K9::Input::Keys::A)) {
					camera->moveRelative(glm::vec3(-0.5f, 0.0f, 0.0f));
				}

				if (K9::Input::Keyboard::isPressed(K9::Input::Keys::W)) {
					camera->moveRelative(glm::vec3(0.0f, 0.0f, -0.5f));
				}

				if (K9::Input::Keyboard::isPressed(K9::Input::Keys::S)) {
					camera->moveRelative(glm::vec3(0.0f, 0.0f, 0.5f));
				}

				if (K9::Input::Keyboard::isPressed(K9::Input::Keys::KEY_1)) {
					auto sphere1 = _world->getGameObject("Sphere1");
					if(sphere1 && sphere1->isAlive())
						_world->destroyGameObject(_world->getGameObject("Sphere1"));
				}
			}

			auto Sphere1 = _world->getGameObject("Sphere1");
			if (Sphere1 != nullptr) {
				Sphere1->localRotation().y += rot;
			}

			auto Sphere2 = _world->getGameObject("Sphere2");
			Sphere2->localRotation().y += rot;

			auto Sphere3 = _world->getGameObject("Sphere3");
			Sphere3->localRotation().y += rot;
		}

		void DemoChapter10::draw(double dt) {
			clearColorBuffer();
			clearDepthBuffer();

			K9::Graphics::RenderingManager::instance().draw(*_world);
		}

		void DemoChapter10::initRenderers() {
			createAndBindVAO();
			int returnCode;
			std::string returnMsg;

			//Light Material Renderer
			GLuint lightMaterialRenderingProgram = createRenderingProgram("lightMaterialVertShader.glsl", "lightMaterialFragShader.glsl", returnCode, returnMsg);
			auto lightMaterialRenderer = std::make_shared<Graphics::LightMaterialRenderer>("LightMaterialRenderer", lightMaterialRenderingProgram);
			Graphics::RenderingManager::instance().addRenderer(lightMaterialRenderer);
			std::cout << "returnCode: " << returnCode << std::endl;
			std::cout << "returnMsg: " << returnMsg << std::endl;
			//Bump Mapping Renderer
			GLuint bumpMappingRenderingProgram = createRenderingProgram("bumpMappingVertShader.glsl", "bumpMappingFragShader.glsl", returnCode, returnMsg);
			auto bumpMappingRenderer = std::make_shared<Graphics::BumpMappingRenderer>("BumpMappingRenderer", bumpMappingRenderingProgram);
			Graphics::RenderingManager::instance().addRenderer(bumpMappingRenderer);
			std::cout << "returnCode: " << returnCode << std::endl;
			std::cout << "returnMsg: " << returnMsg << std::endl;
			//Normal Mapping Renderer
			GLuint normalMappingRenderingProgram = createRenderingProgram("normalMappingVertShader.glsl", "normalMappingFragShader.glsl", returnCode, returnMsg);
			auto normalMappingRenderer = std::make_shared<Graphics::NormalMappingRenderer>("NormalMappingRenderer", normalMappingRenderingProgram);
			Graphics::RenderingManager::instance().addRenderer(normalMappingRenderer);
			std::cout << "returnCode: " << returnCode << std::endl;
			std::cout << "returnMsg: " << returnMsg << std::endl;
		}

		void DemoChapter10::loadTextures() {
			auto castleRoofNormal = Graphics::TexturesPool::instance().createTexture("castleroofNORMAL.jpg");
			auto castleRoofNormalTxt = castleRoofNormal.lock();
			if (castleRoofNormalTxt != nullptr) {
				castleRoofNormalTxt->setMinificationMipmapModeLinearMipmapLinear();
				castleRoofNormalTxt->setMaxTextureFilterAnisotropic();
			}
		}

		void DemoChapter10::initSphere1() {
			using namespace K9::Components;
			using namespace K9::Graphics;

			// create game object
			auto gameObject = std::make_shared<GameObject>("Sphere1");
			//cubeObject->scale() = glm::vec3(30.0f, 1.0f, 10.0f);
			gameObject->position() = glm::vec3(-2.0f, 0.0f, 0.0f);
			_world->addGameObject(gameObject);

			// create rendering component
			auto renderingComponent = std::make_shared<RenderingComponent>(gameObject, "Sphere1RenderingComponent");
			GameObjectComponentsPool::instance().addRenderingComponentByGameObjectName(gameObject->name(), renderingComponent);

			// creates the model and attach to rendering component
			auto model = ModelsPool::instance().createSphere(48, 48);
			//auto model = ModelsPool::instance().createTorus();
			renderingComponent->setModel(model);

			// creates the color material and attach to rendering component
			auto colorMaterial = ColorMaterialFactory::createGoldColorMaterial();
			renderingComponent->setColorMaterial(colorMaterial);

			// creates the normal material and attach to rendering component
			auto normalTexture = TexturesPool::instance().createTexture("castleroofNORMAL.jpg");
			auto textureMaterialComponent = std::make_shared<TextureMaterialComponent>();
			textureMaterialComponent->setTexture(normalTexture);
			auto normalMaterial = std::make_shared<NormalMaterial>("castleroofNORMAL");
			normalMaterial->setTexture(textureMaterialComponent);
			renderingComponent->setNormalMaterial(normalMaterial);

			// attach renderer with rendering component
			auto renderer = RenderingManager::instance().getRenderer("NormalMappingRenderer");
			GameObjectComponentsPool::instance().attachRendererAndRenderingComponent(renderer, renderingComponent);
		}

		void DemoChapter10::initSphere2() {
			using namespace K9::Components;
			using namespace K9::Graphics;

			// create game object
			auto gameObject = std::make_shared<GameObject>("Sphere2");
			//cubeObject->scale() = glm::vec3(30.0f, 1.0f, 10.0f);
			gameObject->position() = glm::vec3(0.0f, 0.0f, 0.0f);
			_world->addGameObject(gameObject);

			// create rendering component
			auto renderingComponent = std::make_shared<RenderingComponent>(gameObject, "Sphere2RenderingComponent");
			GameObjectComponentsPool::instance().addRenderingComponentByGameObjectName(gameObject->name(), renderingComponent);

			// creates the model and attach to rendering component
			auto model = ModelsPool::instance().createSphere(48, 48);
			//auto model = ModelsPool::instance().createTorus();
			renderingComponent->setModel(model);

			// creates the color material and attach to rendering component
			auto colorMaterial = ColorMaterialFactory::createGoldColorMaterial();
			renderingComponent->setColorMaterial(colorMaterial);

			// creates the normal material and attach to rendering component
			//auto normalTexture = TexturesPool::instance().createTexture("castleroofNORMAL.jpg");
			//auto normalMaterialComponent = std::make_shared<TextureMaterialComponent>();
			//normalMaterialComponent->setTexture(normalTexture);
			//renderingComponent->setNormalMaterial(normalMaterialComponent);

			// attach renderer with rendering component
			auto renderer = RenderingManager::instance().getRenderer("BumpMappingRenderer");
			GameObjectComponentsPool::instance().attachRendererAndRenderingComponent(renderer, renderingComponent);
		}

		void DemoChapter10::initSphere3() {
			using namespace K9::Components;
			using namespace K9::Graphics;

			// create game object
			auto gameObject = std::make_shared<GameObject>("Sphere3");
			//gameObject->scale() = glm::vec3(30.0f, 1.0f, 10.0f);
			gameObject->position() = glm::vec3(2.0f, 0.0f, 0.0f);
			_world->addGameObject(gameObject);

			// create rendering component
			auto renderingComponent = std::make_shared<RenderingComponent>(gameObject, "Sphere3RenderingComponent");
			GameObjectComponentsPool::instance().addRenderingComponentByGameObjectName(gameObject->name(), renderingComponent);

			// creates the model and attach to rendering component
			auto model = ModelsPool::instance().createSphere(48, 48);
			//auto model = ModelsPool::instance().createTorus();
			renderingComponent->setModel(model);

			// creates the color material and attach to rendering component
			auto colorMaterial = ColorMaterialFactory::createGoldColorMaterial();
			renderingComponent->setColorMaterial(colorMaterial);

			// creates the normal material and attach to rendering component
			//auto normalTexture = TexturesPool::instance().createTexture("castleroofNORMAL.jpg");
			//auto normalMaterialComponent = std::make_shared<TextureMaterialComponent>();
			//normalMaterialComponent->setTexture(normalTexture);
			//renderingComponent->setNormalMaterial(normalMaterialComponent);

			// attach renderer with rendering component
			auto renderer = RenderingManager::instance().getRenderer("LightMaterialRenderer");
			GameObjectComponentsPool::instance().attachRendererAndRenderingComponent(renderer, renderingComponent);
		}

		void DemoChapter10::initLights() {
			auto ambientLight = std::make_shared<Lighting::AmbientLight>(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
			_world->addAmbientLight(ambientLight);

			glm::vec4 posAmbient = { 0.0f, 0.0f, 0.0f, 1.0f };
			glm::vec4 posDiffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
			glm::vec4 posSpecular = { 1.0f, 1.0f, 1.0f, 1.0f };
			glm::vec3 posPosition = { 4.0f, 1.5f, 2.0f };
			auto positionalLight = std::make_shared<Lighting::PositionalLight>(posAmbient, posDiffuse, posSpecular, posPosition);
			_world->addPositionalLight(positionalLight);
		}

		void DemoChapter10::initCamera() {
			int width, height;
			glfwGetFramebufferSize(_window, &width, &height);
			auto camera = std::make_shared<Camera>("MainCamera", (float)width, (float)height);
			camera->moveTo(glm::vec3(0.0f, 1.0f, 4.5f));
			camera->rotateX(-3.14f * 20.0f / 180.0f);

			_world->addCamera(camera);
			_world->setActiveCamera(camera->name());
		}
	}
}
