#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <K9Debug.h>

#include <memory>
#include <string>

namespace K9 {
	class GameObject;
	namespace Components {
		class IComponent {
		public:
			IComponent(std::shared_ptr<GameObject> gameObject, const std::string& name) :
				_gameObject(gameObject),
				_name(name){
			}

			virtual ~IComponent() = default;

			const GameObject& gameObject() const {
				K9_ASSERT(_gameObject != nullptr);

				return *_gameObject;
			}

			const std::string& name() const {
				return _name;
			}
		protected:
			std::shared_ptr<GameObject> _gameObject;
			std::string _name;
		};
	}
}

#endif // !_COMPONENT_H_
