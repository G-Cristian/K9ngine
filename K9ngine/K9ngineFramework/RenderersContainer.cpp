#include <RenderersContainer.h>

#include <Renderer.h>
#include <RenderingComponent.h>
#include <World.h>

#include <memory>
#include <set>

namespace K9 {
	
	namespace Graphics {
		void RenderersContainer::draw(const World& world, const std::set<std::shared_ptr<const K9::Components::RenderingComponent>>& renderingComponents) const {
			for (auto it = _renderers.cbegin(); it != _renderers.cend(); ++it) {
				(*it)->draw(world, renderingComponents);
			}
		}

		void RenderersContainer::addRenderer(const std::shared_ptr<IRenderer>& renderer) {
			_renderers.push_back(renderer);
		}
	}
}
