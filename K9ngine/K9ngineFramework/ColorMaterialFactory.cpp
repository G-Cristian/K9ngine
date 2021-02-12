#include <ColorMaterialFactory.h>

#include <ColorMaterial.h>
#include <FloatMaterialComponent.h>
#include <VectorMaterialComponent.h>

#include <glm/glm.hpp>
#include <memory>

namespace K9 {
	namespace Graphics {
		std::shared_ptr<ColorMaterial> ColorMaterialFactory::createGoldColorMaterial() {
			auto material = std::make_shared<ColorMaterial>("GoldColorMaterial");
			
			auto ambient = std::make_shared<VectorMaterialComponent>(glm::vec4(0.24725f, 0.1995f, 0.0745f, 1.0f));
			material->setAmbientComponent(ambient);

			auto diffuse = std::make_shared<VectorMaterialComponent>(glm::vec4(0.75164f, 0.60648f, 0.22648f, 1.0f));
			material->setDiffuseComponent(diffuse);

			auto specular = std::make_shared<VectorMaterialComponent>(glm::vec4(0.62828f, 0.5558f, 0.36607f, 1.0f));
			material->setSpecularComponent(specular);

			auto shininess = std::make_shared<FloatMaterialComponent>(51.2f);
			material->setShininessComponent(shininess);
		
			return material;
		}

		std::shared_ptr<ColorMaterial> ColorMaterialFactory::createJadeColorMaterial() {
			auto material = std::make_shared<ColorMaterial>("JadeColorMaterial");

			auto ambient = std::make_shared<VectorMaterialComponent>(glm::vec4(0.135f, 0.2225f, 0.1575f, 0.95f));
			material->setAmbientComponent(ambient);

			auto diffuse = std::make_shared<VectorMaterialComponent>(glm::vec4(0.54f, 0.89f, 0.63f, 0.95f));
			material->setDiffuseComponent(diffuse);

			auto specular = std::make_shared<VectorMaterialComponent>(glm::vec4(0.3162f, 0.3162f, 0.3162f, 0.95f));
			material->setSpecularComponent(specular);

			auto shininess = std::make_shared<FloatMaterialComponent>(12.8f);
			material->setShininessComponent(shininess);

			return material;
		}

		std::shared_ptr<ColorMaterial> ColorMaterialFactory::createPearlColorMaterial() {
			auto material = std::make_shared<ColorMaterial>("PearlColorMaterial");

			auto ambient = std::make_shared<VectorMaterialComponent>(glm::vec4(0.25f, 0.20725f, 0.20725f, 0.922f));
			material->setAmbientComponent(ambient);

			auto diffuse = std::make_shared<VectorMaterialComponent>(glm::vec4(1.00f, 0.829f, 0.829f, 0.922f));
			material->setDiffuseComponent(diffuse);

			auto specular = std::make_shared<VectorMaterialComponent>(glm::vec4(0.2966f, 0.2966f, 0.2966f, 0.922f));
			material->setSpecularComponent(specular);

			auto shininess = std::make_shared<FloatMaterialComponent>(11.264f);
			material->setShininessComponent(shininess);

			return material;
		}

		std::shared_ptr<ColorMaterial> ColorMaterialFactory::createSilverColorMaterial() {
			auto material = std::make_shared<ColorMaterial>("SilverColorMaterial");

			auto ambient = std::make_shared<VectorMaterialComponent>(glm::vec4(0.19225f, 0.19225f, 0.19225f, 1.0f));
			material->setAmbientComponent(ambient);

			auto diffuse = std::make_shared<VectorMaterialComponent>(glm::vec4(0.50754f, 0.50754f, 0.50754f, 1.0f));
			material->setDiffuseComponent(diffuse);

			auto specular = std::make_shared<VectorMaterialComponent>(glm::vec4(0.50827f, 0.50827f, 0.50827f, 1.0f));
			material->setSpecularComponent(specular);

			auto shininess = std::make_shared<FloatMaterialComponent>(51.2f);
			material->setShininessComponent(shininess);

			return material;
		}
	}
}