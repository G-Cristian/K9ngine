#ifndef WORLD_H_
#define WORLD_H_

#include <AmbientLight.h>
#include <Camera.h>
#include <DirectionalLight.h>
#include <GameObject.h>
#include <K9Debug.h>
#include <PositionalLight.h>
#include <Spotlight.h>

#include <map>
#include <memory>
#include <vector>

namespace K9 {
	class World {
	public:
		World() = default;
		World(const World&) = delete;
		World(World&& other)noexcept = delete;

		~World() = default;

		World& operator=(const World&) = delete;
		World& operator=(World&&) noexcept = delete;

		void addGameObject(const std::shared_ptr<GameObject>& gameObject) {
			//TODO: Should assert that the gameObject is not in the container already
			_gameObjects.insert(std::make_pair(gameObject->name(), gameObject));
		}

		std::shared_ptr<GameObject> getGameObject(const std::string& name)const {
			auto it = _gameObjects.find(name);
			K9_ASSERT(it != _gameObjects.end());
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

		void addAmbientLight(const K9::Lighting::AmbientLight& ambientLight) {
			//TODO: Should assert that the light is not in the container already
			_ambientLights.push_back(ambientLight);
		}

		const std::vector<K9::Lighting::AmbientLight>& getAmbientLights()const {
			return _ambientLights;
		}

		void addDirectionalLight(const K9::Lighting::DirectionalLight& directionalLight) {
			//TODO: Should assert that the light is not in the container already
			_directionalLights.push_back(directionalLight);
		}

		const std::vector<K9::Lighting::DirectionalLight>& getDirectionalLights()const {
			return _directionalLights;
		}

		void addPositionalLight(const K9::Lighting::PositionalLight& positionalLight) {
			//TODO: Should assert that the light is not in the container already
			_positionalLights.push_back(positionalLight);
		}

		const std::vector<K9::Lighting::PositionalLight>& getPositionalLights()const {
			return _positionalLights;
		}

		void addSpotlight(const K9::Lighting::Spotlight& spotlight) {
			//TODO: Should assert that the light is not in the container already
			_spotlights.push_back(spotlight);
		}

		const std::vector<K9::Lighting::Spotlight>& getSpotlights()const {
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
	private:
		//TODO: Probably should use Partitioning Trees for one or more of these
		std::map<std::string, std::shared_ptr<GameObject>> _gameObjects;
		std::vector<K9::Lighting::AmbientLight> _ambientLights;
		std::vector<K9::Lighting::DirectionalLight> _directionalLights;
		std::vector<K9::Lighting::PositionalLight> _positionalLights;
		std::vector<K9::Lighting::Spotlight> _spotlights;
		std::map<std::string, std::shared_ptr<Camera>> _cameras;
		std::weak_ptr<Camera> _activeCamera;
	};
}

#endif // !WORLD_H_
