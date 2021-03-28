#ifndef _GAMEOBJECTCOMPONENTSPOOL_H_
#define _GAMEOBJECTCOMPONENTSPOOL_H_

#include <K9Debug.h>

#include <GameObject.h>
#include <Renderer.h>
#include <RenderingComponent.h>

#include <list>
#include <map>
#include <memory>
#include <string>

namespace K9 {
	namespace Components {
		class GameObjectComponentsPool {
		public:
			static GameObjectComponentsPool& instance() {
				if (_instance == nullptr) {
					_instance = std::unique_ptr<GameObjectComponentsPool>(new GameObjectComponentsPool());
				}

				return *_instance;
			}

			~GameObjectComponentsPool();

			void dispose();

			// Delete copy/move constructor/assignment
			GameObjectComponentsPool(const GameObjectComponentsPool&) = delete;
			GameObjectComponentsPool(GameObjectComponentsPool&&) noexcept = delete;
			GameObjectComponentsPool& operator=(GameObjectComponentsPool&&) noexcept = delete;

			void addRenderingComponentByGameObjectName(const std::string& gameObjectName, std::shared_ptr<RenderingComponent> renderingComponent);
			void addPendingRenderingComponents();
			void removeRenderingComponentByGameObjectName(const std::string& gameObjectName);
			std::shared_ptr<const RenderingComponent> getRenderingComponentByGameObjectName(const std::string& gameObjectName)const;
			std::shared_ptr<RenderingComponent> getRenderingComponentByGameObjectName(const std::string& gameObjectName);

			void attachRendererAndRenderingComponent(std::shared_ptr<const K9::Graphics::IRenderer> renderer, std::shared_ptr<RenderingComponent> renderingComponent);
			void attachPendingRenderersAndRenderingComponents();
			void detachRendererAndRenderingComponent(std::shared_ptr<RenderingComponent> renderingComponent);
		private:
			GameObjectComponentsPool() = default;

			void addPendingRenderingComponentByGameObjectName(const std::string& gameObjectName, std::shared_ptr<RenderingComponent> renderingComponent);
			void attachPendingRendererAndRenderingComponent(std::shared_ptr<const K9::Graphics::IRenderer> renderer, std::shared_ptr<RenderingComponent> renderingComponent);

			static std::unique_ptr<GameObjectComponentsPool> _instance;
			//Components
			std::map<std::string, std::shared_ptr<RenderingComponent>> _gameObjectNamesRenderingComponents;
			std::list<std::pair<const std::string, std::shared_ptr<RenderingComponent>>> _gameObjectsNamesRenderingComponentsToBeAdded;
			std::list<std::pair<std::shared_ptr<const K9::Graphics::IRenderer>, std::shared_ptr<RenderingComponent>>> _renderersAndRenderingComponentsToBeAttached;
		};
	}
}

#endif // !_GAMEOBJECTCOMPONENTSPOOL_H_
