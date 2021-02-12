#include <World.h>

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
}
