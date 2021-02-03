#include <RenderingManager.h>

#include <K9Debug.h>
#include <Renderer.h>
#include <RenderingComponent.h>

#include <map>
#include <memory>

namespace K9 {
	namespace Graphics {
		std::unique_ptr<RenderingManager> RenderingManager::_instance = nullptr;

		RenderingManager::~RenderingManager() {
			if (_instance != nullptr) {
				for (auto it = _renderersRenderingComponents.begin(); it != _renderersRenderingComponents.end(); ++it) {
					//The set of components must be empty by this time, i.e. all components must have been removed and erased.
					K9_ASSERT(it->second.empty());
					it->second.clear();
				}
				_renderersRenderingComponents.clear();

				for (auto it = _renderers.begin(); it != _renderers.end(); ++it) {
					it->second = nullptr;
				}
				_renderers.clear();

				_instance = nullptr;
			}
		}

		void RenderingManager::draw(const World& world) const {
			for (auto it = _renderersRenderingComponents.cbegin(); it != _renderersRenderingComponents.cend(); ++it) {
				it->first->draw(world, it->second);
			}
		}

		void RenderingManager::addRenderer(std::shared_ptr<const IRenderer> renderer) {
			K9_ASSERT(renderer != nullptr);
			K9_ASSERT(!isRendererInRenderers(*renderer));
			K9_ASSERT(!isRendererInRenderersRenderingComponents(renderer));

			auto inserted = _renderers.insert(std::make_pair(renderer->name(), renderer));
			_renderersRenderingComponents.insert(std::make_pair(inserted.first->second, std::set<std::shared_ptr<const K9::Components::RenderingComponent>>()));
		}

		void RenderingManager::removeRenderer(const std::string& rendererName) {
			K9_ASSERT(isRendererInRenderers(rendererName));

			auto it = _renderers.find(rendererName);
			if (it != _renderers.end()) {
				K9_ASSERT(it->second != nullptr);
				removeRendererFromRenderersRenderingComponents(it->second);
				
				_renderers.erase(it);
			}
		}

		std::shared_ptr<const IRenderer> RenderingManager::getRenderer(const std::string& rendererName)const {
			K9_ASSERT(isRendererInRenderers(rendererName));

			auto it = _renderers.find(rendererName);
			K9_ASSERT(it->second != nullptr);

			return it->second;
		}

		void RenderingManager::addRenderingComponentToRenderer(std::shared_ptr<const K9::Components::RenderingComponent> renderingComponent, std::shared_ptr<const IRenderer> renderer) {
			K9_ASSERT(isRendererInRenderersRenderingComponents(renderer));
			auto it = _renderersRenderingComponents.find(renderer);
			if (it != _renderersRenderingComponents.end()) {
				auto componentIt = it->second.find(renderingComponent);
				K9_ASSERT(componentIt == it->second.end());
				it->second.insert(componentIt, renderingComponent);
			}
		}
		void RenderingManager::removeRenderingComponentFromRenderer(std::shared_ptr<const K9::Components::RenderingComponent> renderingComponent, std::shared_ptr<const IRenderer> renderer) {
			K9_ASSERT(isRendererInRenderersRenderingComponents(renderer));
			auto it = _renderersRenderingComponents.find(renderer);
			if (it != _renderersRenderingComponents.end()) {
				auto componentIt = it->second.find(renderingComponent);
				K9_ASSERT(componentIt != it->second.end());
				it->second.erase(componentIt);
			}
		}

		bool RenderingManager::isRendererInRenderers(const IRenderer& renderer) const{
			return isRendererInRenderers(renderer.name());
		}

		bool RenderingManager::isRendererInRenderers(const std::string& rendererName)const {
			return _renderers.find(rendererName) != _renderers.end();
		}

		bool RenderingManager::isRendererInRenderersRenderingComponents(std::shared_ptr<const IRenderer> renderer)const {
			return _renderersRenderingComponents.find(renderer) != _renderersRenderingComponents.end();
		}

		void RenderingManager::removeRendererFromRenderersRenderingComponents(std::shared_ptr<const IRenderer> renderer) {
			K9_ASSERT(isRendererInRenderersRenderingComponents(renderer));

			auto it = _renderersRenderingComponents.find(renderer);
			if (it != _renderersRenderingComponents.end()) {
				//The set of components must be empty, i.e. I can't remove a renderer if there is a component using it.
				K9_ASSERT(it->second.empty());
				it->second.clear();
				_renderersRenderingComponents.erase(it);
			}
		}
	}
}