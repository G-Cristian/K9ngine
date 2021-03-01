#ifndef _RENDERINGMANAGER_H_
#define _RENDERINGMANAGER_H_

#include <Renderer.h>
#include <RenderingComponent.h>

#include <functional>
#include <map>
#include <memory>
#include <set>
#include <string>

namespace K9 {
	class World;
	namespace Graphics {
		bool rendererComparer(const std::shared_ptr<const IRenderer>& left, const std::shared_ptr<const IRenderer>& right);
		class RenderingManager {
		public:
			~RenderingManager();
			void dispose();

			RenderingManager(const RenderingManager&) = delete;
			RenderingManager(RenderingManager&&) noexcept = delete;
			RenderingManager& operator=(const RenderingManager&) = delete;
			RenderingManager& operator=(RenderingManager&&) noexcept = delete;

			static RenderingManager& instance() {
				if (_instance == nullptr) {
					_instance = std::unique_ptr<RenderingManager>(new RenderingManager());
				}

				return *_instance;
			}

			void draw(const World& world) const;

			void addRenderer(std::shared_ptr<const IRenderer> renderer);
			void removeRenderer(const std::string& rendererName);

			std::shared_ptr<const IRenderer> getRenderer(const std::string& rendererName)const;

			void addRenderingComponentToRenderer(std::shared_ptr<const K9::Components::RenderingComponent> renderingComponent, std::shared_ptr<const IRenderer> renderer);
			void removeRenderingComponentFromRenderer(std::shared_ptr<const K9::Components::RenderingComponent> renderingComponent, std::shared_ptr<const IRenderer> renderer);

		private:
			RenderingManager() :
				_renderersRenderingComponents(rendererComparer) {
			}

			bool isRendererInRenderers(const IRenderer&)const;
			bool isRendererInRenderers(const std::string&)const;
			bool isRendererInRenderersRenderingComponents(std::shared_ptr<const IRenderer>)const;

			void removeRendererFromRenderersRenderingComponents(std::shared_ptr<const IRenderer>renderer);

			static std::unique_ptr<RenderingManager> _instance;
			std::map<std::string, std::shared_ptr<const IRenderer>> _renderers;
			std::map<std::shared_ptr<const IRenderer>, std::set<std::shared_ptr<const K9::Components::RenderingComponent>>, decltype(rendererComparer)*> _renderersRenderingComponents;
		};
	}
}

#endif // !_RENDERINGMANAGER_H_
