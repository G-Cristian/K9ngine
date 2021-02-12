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

		void GameObjectComponentsPool::attachRendererAndRenderingComponent(std::shared_ptr<const K9::Graphics::IRenderer> renderer, std::shared_ptr<RenderingComponent> renderingComponent) {
			K9_ASSERT(renderer != nullptr);
			using namespace Graphics;
			detachRendererAndRenderingComponent(renderingComponent);
			renderingComponent->_renderer = renderer;
			RenderingManager::instance().addRenderingComponentToRenderer(renderingComponent, renderingComponent->_renderer);
		}

		void GameObjectComponentsPool::detachRendererAndRenderingComponent(std::shared_ptr<RenderingComponent> renderingComponent) {
			using namespace Graphics;
			if (renderingComponent->_renderer != nullptr) {
				RenderingManager::instance().removeRenderingComponentFromRenderer(renderingComponent, renderingComponent->_renderer);
				renderingComponent->_renderer = nullptr;
			}
		}
	}
}