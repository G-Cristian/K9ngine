#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <GL/glew.h>
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
		class IRenderer {
		public:
			IRenderer(const std::string& name) :
				_name(name){
			}

			virtual ~IRenderer() = default;

			virtual void draw(const World& world, const std::set<std::shared_ptr<const K9::Components::RenderingComponent>>& renderingComponents) const = 0;

			const std::string& name() const{
				return _name;
			}

			friend bool operator<(const IRenderer&, const IRenderer&);

		protected:
			std::string _name;
		};

		bool operator<(const IRenderer& left, const IRenderer& right);
	}
}

#endif // !_RENDERER_H_
