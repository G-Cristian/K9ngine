#ifndef WORLD_H_
#define WORLD_H_

#include <AmbientLight.h>
#include <Camera.h>
#include <DirectionalLight.h>
#include <GameObject.h>
#include <K9Debug.h>
#include <PositionalLight.h>
#include <Spotlight.h>

#include <list>
#include <map>
#include <memory>
#include <vector>

namespace K9 {
	class World {
	public:
		World() = default;
		World(const World&) = delete;
		World(World&& other)noexcept = delete;

		~World();

		World& operator=(const World&) = delete;
		World& operator=(World&&) noexcept = delete;

		void addGameObject(const std::shared_ptr<GameObject>& gameObject) {
			//TODO: Should assert that the gameObject is not in the container already
			_gameObjectsToBeAdded.push_back(gameObject);
		}

		void addGameObjects() {
			for (auto it = _gameObjectsToBeAdded.begin(); it != _gameObjectsToBeAdded.end();) {
				auto& gameObject = *it;
				_gameObjects.emplace(gameObject->name(), gameObject);
				it = _gameObjectsToBeAdded.erase(it);
			}
		}

		std::shared_ptr<GameObject> getGameObject(const std::string& name)const {
			auto it = _gameObjects.find(name);
			//K9_ASSERT(it != _gameObjects.end());
			if (it != _gameObjects.end()) {
				return it->second;
			}
			else {
				return nullptr;
			}
		}

		const std::map<std::string, std::shared_ptr<GameObject>>& getGameObjects()const {
			return _gameObjects;
		}

		void addAmbientLight(const std::shared_ptr <K9::Lighting::AmbientLight>& ambientLight) {
			//TODO: Should assert that the light is not in the container already
			_ambientLights.push_back(ambientLight);
		}

		const std::vector<std::shared_ptr <K9::Lighting::AmbientLight>>& getAmbientLights()const {
			return _ambientLights;
		}

		void addDirectionalLight(const std::shared_ptr < K9::Lighting::DirectionalLight>& directionalLight) {
			//TODO: Should assert that the light is not in the container already
			_directionalLights.push_back(directionalLight);
		}

		const std::vector<std::shared_ptr <K9::Lighting::DirectionalLight>>& getDirectionalLights()const {
			return _directionalLights;
		}

		void addPositionalLight(const std::shared_ptr <K9::Lighting::PositionalLight>& positionalLight) {
			//TODO: Should assert that the light is not in the container already
			_positionalLights.push_back(positionalLight);
		}

		const std::vector<std::shared_ptr<K9::Lighting::PositionalLight>>& getPositionalLights()const {
			return _positionalLights;
		}

		void addSpotlight(const std::shared_ptr<K9::Lighting::Spotlight>& spotlight) {
			//TODO: Should assert that the light is not in the container already
			_spotlights.push_back(spotlight);
		}

		const std::vector<std::shared_ptr<K9::Lighting::Spotlight>>& getSpotlights()const {
			return _spotlights;
		}

		void addCamera(const std::shared_ptr<Camera>& camera) {
			//TODO: Should assert that the camera is not in the container already
			_cameras.insert(std::make_pair(camera->name(), camera));
		}

		const std::map<std::string, std::shared_ptr<Camera>>& getCameras()const {
			return _cameras;
		}

		void resetCamerasProjections(float width, float height);

		void setActiveCamera(const std::string& cameraName);

		std::weak_ptr<const Camera> getActiveCamera()const;
		std::weak_ptr<Camera> getActiveCamera();

		void destroyGameObject(std::shared_ptr<GameObject>);
		void destroyGarbage();

	private:
		void destroyGameObject(const std::string& gameObjectName);
		std::map<std::string, std::shared_ptr<GameObject>>::iterator removeGameObject(const std::string& gameObjectName);
		std::map<std::string, std::shared_ptr<GameObject>>::iterator removeGameObject(std::map<std::string, std::shared_ptr<GameObject>>::iterator gameObjectIt);

		//TODO: Probably should use Partitioning Trees for one or more of these
		std::map<std::string, std::shared_ptr<GameObject>> _gameObjects;
		std::vector<std::shared_ptr<K9::Lighting::AmbientLight>> _ambientLights;
		std::vector<std::shared_ptr<K9::Lighting::DirectionalLight>> _directionalLights;
		std::vector<std::shared_ptr<K9::Lighting::PositionalLight>> _positionalLights;
		std::vector< std::shared_ptr<K9::Lighting::Spotlight>> _spotlights;
		std::map<std::string, std::shared_ptr<Camera>> _cameras;
		std::weak_ptr<Camera> _activeCamera;

		std::list<std::shared_ptr<GameObject>> _gameObjectsToBeAdded;
		std::list<std::string> _gameObjectsToBeDestroyed;
	};
}

#endif // !WORLD_H_
