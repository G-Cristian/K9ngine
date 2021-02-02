#include <GameObjectComponentsPool.h>
#include <RenderingManager.h>

namespace K9 {
	namespace Components {
		std::unique_ptr<GameObjectComponentsPool> GameObjectComponentsPool::_instance = nullptr;

		GameObjectComponentsPool::~GameObjectComponentsPool() {
			//TODO
			// Something similar to RenderingManager
			// i.e. remove components and make sure that they are not used/referenced in other places
			// (for example check the count of shared_ptr)
		}

		void GameObjectComponentsPool::setRendererToRenderingComponent(std::shared_ptr<const K9::Graphics::IRenderer> renderer, std::shared_ptr<RenderingComponent> renderingComponent) {
			K9_ASSERT(renderer != nullptr);
			using namespace Graphics;
			clearRendererToRenderingComponent(renderingComponent);
			renderingComponent->_renderer = renderer;
			RenderingManager::instance().addRenderingComponentToRenderer(renderingComponent, renderingComponent->_renderer);
		}

		void GameObjectComponentsPool::clearRendererToRenderingComponent(std::shared_ptr<RenderingComponent> renderingComponent) {
			using namespace Graphics;
			if (renderingComponent->_renderer != nullptr) {
				RenderingManager::instance().removeRenderingComponentFromRenderer(renderingComponent, renderingComponent->_renderer);
				renderingComponent->_renderer = nullptr;
			}
		}
	}
}