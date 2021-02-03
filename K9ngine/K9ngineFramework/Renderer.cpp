#include <Renderer.h>

namespace K9 {
	namespace Graphics {
		bool operator<(const IRenderer& left, const IRenderer& right) {
			return left._name < right._name;
		}
	}
}