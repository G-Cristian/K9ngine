#ifndef RENDERERSCONTAINER_H_
#define RENDERERSCONTAINER_H_

#include <Renderer.h>

#include <list>
#include <memory>
#include <set>
#include <string>

namespace K9 {
	namespace Components {
		class RenderingComponent;
	}
}

namespace K9 {
	class World;
	namespace Graphics {
		class RenderersContainer :public IRenderer {
		public:
			RenderersContainer(const std::string& name) :
				IRenderer(name) {
			}

			virtual ~RenderersContainer() = default;

			void draw(const World& world, const std::set<std::shared_ptr<const K9::Components::RenderingComponent>>& renderingComponents) const override;

			void addRenderer(const std::shared_ptr<const IRenderer>& renderer);
		private:
			std::list<std::shared_ptr<const IRenderer>> _renderers;
		};
	}
}

#endif // !RENDERERSCONTAINER_H_
