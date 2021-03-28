#include <World.h>

#include <AmbientLight.h>
#include <Camera.h>
#include <DirectionalLight.h>
#include <GameObject.h>
#include <GameObjectComponentsPool.h>
#include <K9Debug.h>
#include <PositionalLight.h>
#include <Spotlight.h>

#include <map>
#include <memory>
#include <vector>

namespace K9 {
	World::~World() {
		_gameObjectsToBeDestroyed.clear();

		for (auto it = _gameObjects.begin(); it != _gameObjects.end();) {
			it = removeGameObject(it);
		}

		K9_ASSERT(_gameObjects.empty());
		_gameObjects.clear();

		_ambientLights.clear();
		_directionalLights.clear();
		_positionalLights.clear();
		_spotlights.clear();
		_activeCamera.reset();
		_cameras.clear();
	}

	void World::setActiveCamera(const std::string& cameraName) {
		auto it = _cameras.find(cameraName);
		K9_ASSERT(it != _cameras.end());
		if (it != _cameras.end()) {
			_activeCamera = it->second;
		}
	}

	std::weak_ptr<const Camera> World::getActiveCamera()const {
		K9_ASSERT(_activeCamera.use_count() != 0 && !_activeCamera.expired());
		return _activeCamera;
	}

	std::weak_ptr<Camera> World::getActiveCamera() {
		K9_ASSERT(_activeCamera.use_count() != 0 && !_activeCamera.expired());
		return _activeCamera;
	}

	void World::resetCamerasProjections(float width, float height) {
		for (auto it = _cameras.begin(); it != _cameras.end(); ++it) {
			it->second->resetProjection(width, height);
		}
	}

	void World::destroyGameObject(std::shared_ptr<GameObject> gameObject) {
		K9_ASSERT(gameObject != nullptr);
		if (gameObject != nullptr) {
			gameObject->kill();
			destroyGameObject(gameObject->name());
		}
	}

	void World::destroyGarbage() {
		for (auto it = _gameObjectsToBeDestroyed.begin(); it != _gameObjectsToBeDestroyed.end(); ++it) {
			removeGameObject(*it);
		}

		_gameObjectsToBeDestroyed.clear();
	}

	void World::destroyGameObject(const std::string& gameObjectName) {
		_gameObjectsToBeDestroyed.push_back(gameObjectName);
	}

	std::map<std::string, std::shared_ptr<GameObject>>::iterator World::removeGameObject(const std::string& gameObjectName) {
		auto gameObjectIt = _gameObjects.find(gameObjectName);
		// make sure that there is a game object with this name and
		K9_ASSERT(gameObjectIt != _gameObjects.end());

		return removeGameObject(gameObjectIt);
	}

	std::map<std::string, std::shared_ptr<GameObject>>::iterator World::removeGameObject(std::map<std::string, std::shared_ptr<GameObject>>::iterator gameObjectIt) {
		// make sure that the game object iterator is not end
		K9_ASSERT(gameObjectIt != _gameObjects.end());
		if (gameObjectIt != _gameObjects.end()) {
			// remove from game components
			auto& gameObjectComponentsPoolInstance = K9::Components::GameObjectComponentsPool::instance();
			gameObjectComponentsPoolInstance.detachRendererAndRenderingComponent(gameObjectComponentsPoolInstance.getRenderingComponentByGameObjectName(gameObjectIt->first));
			gameObjectComponentsPoolInstance.removeRenderingComponentByGameObjectName(gameObjectIt->first);

			// make sure that this game object pointer is the last one (i.e. unique)
			K9_ASSERT(gameObjectIt->second.unique());
			gameObjectIt->second = nullptr;

			// remove from world
			return _gameObjects.erase(gameObjectIt);
		}
		else {
			return gameObjectIt;
		}
	}
}
