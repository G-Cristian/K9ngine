#ifndef _GAMEOBJECTCOMPONENTSPOOL_H_
#define _GAMEOBJECTCOMPONENTSPOOL_H_

#include <K9Debug.h>

#include <GameObject.h>
#include <Renderer.h>
#include <RenderingComponent.h>

#include <map>
#include <memory>
#include <string>

namespace K9 {
	namespace Components {
		class GameObjectComponentsPool {
		public:
			static GameObjectComponentsPool& instance() {
				if (_instance == nullptr) {
					_instance = std::unique_ptr<GameObjectComponentsPool>(new GameObjectComponentsPool());
				}

				return *_instance;
			}

			~GameObjectComponentsPool();

			//TODO: delete copy/move construcyor/assignment

			void addRenderingComponentByGameObjectName(const std::string& gameObjectName, std::shared_ptr<RenderingComponent> renderingComponent) {
				auto it = _gameObjectNamesRenderingComponents.find(gameObjectName);
				K9_ASSERT(it == _gameObjectNamesRenderingComponents.end());
				if (it == _gameObjectNamesRenderingComponents.end()) {
					_gameObjectNamesRenderingComponents.insert(it, std::make_pair(gameObjectName, renderingComponent));
				}
			}

			void removeRenderingComponentByGameObjectName(const std::string& gameObjectName) {
				auto it = _gameObjectNamesRenderingComponents.find(gameObjectName);
				K9_ASSERT(it != _gameObjectNamesRenderingComponents.end());
				_gameObjectNamesRenderingComponents.erase(it);
			}

			std::shared_ptr<const RenderingComponent> getRenderingComponentByGameObjectName(const std::string& gameObjectName)const {
				auto it = _gameObjectNamesRenderingComponents.find(gameObjectName);
				return (it == _gameObjectNamesRenderingComponents.end() ? nullptr : it->second);
			}

			void setRendererToRenderingComponent(std::shared_ptr<const K9::Graphics::IRenderer> renderer, std::shared_ptr<RenderingComponent> renderingComponent);
			void clearRendererToRenderingComponent(std::shared_ptr<RenderingComponent> renderingComponent);
		private:
			GameObjectComponentsPool() = default;

			static std::unique_ptr<GameObjectComponentsPool> _instance;
			//Components
			std::map<std::string, std::shared_ptr<RenderingComponent>> _gameObjectNamesRenderingComponents;
		};
	}
}

#endif // !_GAMEOBJECTCOMPONENTSPOOL_H_
