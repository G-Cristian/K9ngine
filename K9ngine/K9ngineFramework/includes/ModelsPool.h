#ifndef MODELSPOOL_H_
#define MODELSPOOL_H_

#include <Model.h>

#include <map>
#include <memory>
#include <string>

namespace K9 {
	namespace Graphics {
		class ModelsPool {
		public:
			~ModelsPool();
			void dispose();

			static ModelsPool& instance() {
				if (_instance == nullptr) {
					_instance = std::unique_ptr<ModelsPool>(new ModelsPool());
				}

				return *_instance;
			}

			ModelsPool(const ModelsPool&) = delete;
			ModelsPool(ModelsPool&&) noexcept = delete;
			ModelsPool& operator=(const ModelsPool&) = delete;
			ModelsPool& operator=(ModelsPool&&) noexcept = delete;

			std::weak_ptr<const Model> createCube();
			std::weak_ptr<const Model> createPyramid();
			std::weak_ptr<const Model> createSphere(int slicesPrecision, int verticesPrecision);
			std::weak_ptr<const Model> createTorus(float innerRadius = 0.5f, float outerRadius = 0.2f, int precIn = 48);
			//std::weak_ptr<const Model> loadModel(const char* fileName);
			std::weak_ptr<const Model> createModel(const std::string& fileName);

		private:
			ModelsPool() = default;
			float toRadians(float angle)const{
				return (angle * 3.14159f / 180.0f);
			}

			std::weak_ptr<const Model> loadModel(const std::string& fileName);
			std::map<std::string, std::shared_ptr<Model>>::const_iterator findModel(const std::string& modelName)const;

			static const std::string cubeName;
			static const std::string pyramidName;
			static const std::string sphereName;
			static const std::string torusName;

			static std::unique_ptr<ModelsPool> _instance;
			std::map<std::string, std::shared_ptr<Model>> _modelsPool;
		};
	}
}

#endif // !MODELSPOOL_H_
