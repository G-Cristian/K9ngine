#include <GameObjectComponentsPool.h>
#include <RenderingManager.h>

namespace K9 {
	namespace Components {
		std::unique_ptr<GameObjectComponentsPool> GameObjectComponentsPool::_instance = nullptr;

		GameObjectComponentsPool::~GameObjectComponentsPool() {
			// size must be 0, i.e. call dispose before deleting
			K9_ASSERT(_gameObjectNamesRenderingComponents.empty());
			_gameObjectNamesRenderingComponents.clear();
		}

		void GameObjectComponentsPool::dispose() {
			for (auto it = _gameObjectNamesRenderingComponents.begin(); it != _gameObjectNamesRenderingComponents.end(); ++it) {
				detachRendererAndRenderingComponent(it->second);
				K9_ASSERT(it->second.unique());
				it->second == nullptr;
			}

			_gameObjectNamesRenderingComponents.clear();

			_instance = nullptr;
		}

		void GameObjectComponentsPool::addRenderingComponentByGameObjectName(const std::string& gameObjectName, std::shared_ptr<RenderingComponent> renderingComponent) {
			_gameObjectsNamesRenderingComponentsToBeAdded.push_back({ gameObjectName, renderingComponent });
		}

		void GameObjectComponentsPool::addPendingRenderingComponents() {
			for (auto it = _gameObjectsNamesRenderingComponentsToBeAdded.begin(); it != _gameObjectsNamesRenderingComponentsToBeAdded.end();) {
				addPendingRenderingComponentByGameObjectName(it->first, it->second);
				it = _gameObjectsNamesRenderingComponentsToBeAdded.erase(it);
			}
		}

		void GameObjectComponentsPool::removeRenderingComponentByGameObjectName(const std::string& gameObjectName) {
			auto it = _gameObjectNamesRenderingComponents.find(gameObjectName);
			//K9_ASSERT(it != _gameObjectNamesRenderingComponents.end());
			if (it != _gameObjectNamesRenderingComponents.end()) {
				it->second = nullptr;
				_gameObjectNamesRenderingComponents.erase(it);
			}
		}

		std::shared_ptr<const RenderingComponent> GameObjectComponentsPool::getRenderingComponentByGameObjectName(const std::string& gameObjectName)const {
			auto it = _gameObjectNamesRenderingComponents.find(gameObjectName);
			return (it == _gameObjectNamesRenderingComponents.end() ? nullptr : it->second);
		}

		std::shared_ptr<RenderingComponent> GameObjectComponentsPool::getRenderingComponentByGameObjectName(const std::string& gameObjectName) {
			auto it = _gameObjectNamesRenderingComponents.find(gameObjectName);
			return (it == _gameObjectNamesRenderingComponents.end() ? nullptr : it->second);
		}

		void GameObjectComponentsPool::attachRendererAndRenderingComponent(std::shared_ptr<const K9::Graphics::IRenderer> renderer, std::shared_ptr<RenderingComponent> renderingComponent) {
			_renderersAndRenderingComponentsToBeAttached.push_back({ renderer, renderingComponent });
		}

		void GameObjectComponentsPool::attachPendingRenderersAndRenderingComponents() {
			for (auto it = _renderersAndRenderingComponentsToBeAttached.begin(); it != _renderersAndRenderingComponentsToBeAttached.end();) {
				attachPendingRendererAndRenderingComponent(it->first, it->second);
				it = _renderersAndRenderingComponentsToBeAttached.erase(it);
			}
		}

		void GameObjectComponentsPool::detachRendererAndRenderingComponent(std::shared_ptr<RenderingComponent> renderingComponent) {
			using namespace Graphics;
			//K9_ASSERT(renderingComponent != nullptr);
			if (renderingComponent != nullptr && renderingComponent->_renderer != nullptr) {
				RenderingManager::instance().removeRenderingComponentFromRenderer(renderingComponent, renderingComponent->_renderer);
				renderingComponent->_renderer = nullptr;
			}
		}

		void GameObjectComponentsPool::addPendingRenderingComponentByGameObjectName(const std::string& gameObjectName, std::shared_ptr<RenderingComponent> renderingComponent) {
			auto it = _gameObjectNamesRenderingComponents.find(gameObjectName);
			K9_ASSERT(it == _gameObjectNamesRenderingComponents.end());
			if (it == _gameObjectNamesRenderingComponents.end()) {
				_gameObjectNamesRenderingComponents.insert(it, std::make_pair(gameObjectName, renderingComponent));
			}
		}

		void GameObjectComponentsPool::attachPendingRendererAndRenderingComponent(std::shared_ptr<const K9::Graphics::IRenderer> renderer, std::shared_ptr<RenderingComponent> renderingComponent) {
			K9_ASSERT(renderer != nullptr);
			using namespace Graphics;
			detachRendererAndRenderingComponent(renderingComponent);
			renderingComponent->_renderer = renderer;
			RenderingManager::instance().addRenderingComponentToRenderer(renderingComponent, renderingComponent->_renderer);
		}
	}
}