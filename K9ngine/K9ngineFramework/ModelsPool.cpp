#include <ModelsPool.h>

#include <K9Debug.h>
#include <Model.h>
#include <OpenGLWrapper.h>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <string>
#include <sstream>
#include <vector>

namespace K9 {
	namespace Graphics {

		std::unique_ptr<ModelsPool> ModelsPool::_instance = nullptr;
		const std::string ModelsPool::cubeName = "Cube";
		const std::string ModelsPool::pyramidName = "Pyramid";
		const std::string ModelsPool::sphereName = "Sphere";
		const std::string ModelsPool::torusName = "Torus";

		ModelsPool::~ModelsPool() {
			// Must have called dispose
			K9_ASSERT(_modelsPool.empty());
			_modelsPool.clear();
		}

		void ModelsPool::dispose() {
			for (auto it = _modelsPool.begin(); it != _modelsPool.end(); ++it) {
				K9_ASSERT(it->second != nullptr);
				if (it->second != nullptr) {
					K9_ASSERT(it->second.unique());
				}
				it->second = nullptr;
			}
			_modelsPool.clear();
			_instance = nullptr;
		}

		std::weak_ptr<const Model> ModelsPool::createCube() {
			auto it = findModel(cubeName);
			if (it != _modelsPool.cend()) {
				return std::weak_ptr<const Model>(it->second);
			}
			else {
				float halfSize = 1.0f;
				float vertexPositions[] = {
				halfSize, -halfSize, -halfSize, -halfSize, -halfSize, -halfSize, -halfSize, halfSize, -halfSize,
				-halfSize, halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, -halfSize, -halfSize,
				halfSize, halfSize, -halfSize, halfSize, -halfSize, halfSize, halfSize, -halfSize, -halfSize,
				halfSize, halfSize, -halfSize, halfSize, halfSize, halfSize, halfSize, -halfSize, halfSize,
				halfSize, halfSize, halfSize, -halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize,
				halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize, -halfSize, -halfSize, halfSize,
				-halfSize, halfSize, halfSize, -halfSize, -halfSize, -halfSize, -halfSize, -halfSize, halfSize,
				-halfSize, halfSize, halfSize, -halfSize, halfSize, -halfSize, -halfSize, -halfSize, -halfSize,
				halfSize, -halfSize, -halfSize, halfSize, -halfSize, halfSize, -halfSize, -halfSize, halfSize,
				-halfSize, -halfSize, halfSize, -halfSize, -halfSize, -halfSize, halfSize, -halfSize, -halfSize,
				halfSize, halfSize, halfSize, halfSize, halfSize, -halfSize, -halfSize, halfSize, -halfSize,
				-halfSize, halfSize, -halfSize, -halfSize, halfSize, halfSize, halfSize, halfSize, halfSize
				};

				float vertexNormals[] = {
				0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
				0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
				1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
				-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
				0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
				0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
				};

				float textureCoordinates[] = {
					1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
					1.0f, 0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
					1.0f, 0.0f, 0.0f, 0.0f,	0.0f, 1.0f,
					1.0f, 1.0f, 0.0f, 1.0f,	0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
					1.0f, 0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
					1.0f, 0.0f, 0.0f, 0.0f,	0.0f, 1.0f,
					1.0f, 1.0f, 0.0f, 1.0f,	0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 0.0f,	0.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f,	1.0f, 1.0f
				};

				GLuint vertexPositionsIndex = createArrayBuffer(vertexPositions, 108); //size == 12*9
				GLuint vertexNormalsIndex = createArrayBuffer(vertexNormals, 108); //size == 12*9
				GLuint textureCoordinatesIndex = createArrayBuffer(textureCoordinates, 72); //size == 12*6

				auto model = std::make_shared<Model>();
				model->setVertexPositions(vertexPositionsIndex);
				model->setVertexNormals(vertexNormalsIndex);
				model->setVertexTextureCoordinates(textureCoordinatesIndex);
				model->usesIndices() = false;
				model->numberOfElements() = sizeof(vertexPositions) / sizeof(float);

				auto& modelRef = _modelsPool[cubeName];
				modelRef = model;

				return std::weak_ptr<Model>(modelRef);
			}
		}

		std::weak_ptr<const Model> ModelsPool::createPyramid() {
			auto it = findModel(pyramidName);
			if (it != _modelsPool.cend()) {
				return std::weak_ptr<const Model>(it->second);
			}
			else {
				float halfHeight = 1.0f;
				float halfBase = 1.0f;

				float vertexPositions[] = {
					-halfBase, -halfHeight, halfBase, halfBase, -halfHeight, halfBase, 0.0f, halfHeight, 0.0f,
					halfBase, -halfHeight, halfBase, halfBase, -halfHeight, -halfBase, 0.0f, halfHeight, 0.0f,
					halfBase, -halfHeight, -halfBase, -halfBase, -halfHeight, -halfBase, 0.0f, halfHeight, 0.0f,
					-halfBase, -halfHeight, -halfBase, -halfBase, -halfHeight, halfBase, 0.0f, halfHeight, 0.0f,
					-halfBase, -halfHeight, -halfBase, halfBase, -halfHeight, halfBase, -halfBase, -halfHeight, halfBase,
					halfBase, -halfHeight, halfBase, -halfBase, -halfHeight, -halfBase, halfBase, -halfHeight, -halfBase
				};

				glm::vec3 frontBottomLeft(-halfBase, -halfHeight, halfBase);
				glm::vec3 frontBottomRight(halfBase, -halfHeight, halfBase);
				glm::vec3 top(0.0f, halfHeight, 0.0f);
				glm::vec3 rightBottomBack(halfBase, -halfHeight, -halfBase);
				glm::vec3 leftBottomBack(-halfBase, -halfHeight, -halfBase);
				
				glm::vec3 frontNorm = glm::normalize(glm::cross(frontBottomRight - frontBottomLeft, top - frontBottomLeft));
				glm::vec3 rightNorm = glm::normalize(glm::cross(rightBottomBack - frontBottomRight, top - frontBottomRight));
				glm::vec3 backNorm = glm::normalize(glm::cross(leftBottomBack - rightBottomBack, top - rightBottomBack));
				glm::vec3 leftNorm = glm::normalize(glm::cross(frontBottomLeft - leftBottomBack, top - leftBottomBack));
				glm::vec3 bottomNorm = glm::normalize(glm::cross(leftBottomBack - frontBottomLeft, frontBottomRight - frontBottomLeft));

				float vertexNormals[] = {
					frontNorm.x, frontNorm.y, frontNorm.z, frontNorm.x, frontNorm.y, frontNorm.z, frontNorm.x, frontNorm.y, frontNorm.z,
					rightNorm.x, rightNorm.y, rightNorm.z, rightNorm.x, rightNorm.y, rightNorm.z, rightNorm.x, rightNorm.y, rightNorm.z,
					backNorm.x, backNorm.y, backNorm.z, backNorm.x, backNorm.y, backNorm.z, backNorm.x, backNorm.y, backNorm.z,
					leftNorm.x, leftNorm.y, leftNorm.z, leftNorm.x, leftNorm.y, leftNorm.z, leftNorm.x, leftNorm.y, leftNorm.z,
					bottomNorm.x, bottomNorm.y, bottomNorm.z, bottomNorm.x, bottomNorm.y, bottomNorm.z, bottomNorm.x, bottomNorm.y, bottomNorm.z,
					bottomNorm.x, bottomNorm.y, bottomNorm.z, bottomNorm.x, bottomNorm.y, bottomNorm.z, bottomNorm.x, bottomNorm.y, bottomNorm.z
				};

				float textureCoordinates[] = {
					0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
					0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
					0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
					0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
					0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
					1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
				};

				GLuint vertexPositionsIndex = createArrayBuffer(vertexPositions, 54); //size == 6*9
				GLuint vertexNormalsIndex = createArrayBuffer(vertexNormals, 54); //size == 6*9
				GLuint textureCoordinatesIndex = createArrayBuffer(textureCoordinates, 36); //size == 6*6

				auto model = std::make_shared<Model>();
				model->setVertexPositions(vertexPositionsIndex);
				model->setVertexNormals(vertexNormalsIndex);
				model->setVertexTextureCoordinates(textureCoordinatesIndex);
				model->usesIndices() = false;
				model->numberOfElements() = sizeof(vertexPositions) / sizeof(float);

				auto& modelRef = _modelsPool[pyramidName];
				modelRef = model;

				return std::weak_ptr<Model>(modelRef);
			}
		}

		std::weak_ptr<const Model> ModelsPool::createSphere(int slicesPrecision, int verticesPrecision) {
			std::stringstream ss(sphereName);
			ss << "_" << slicesPrecision << "_" << verticesPrecision;
			std::string modelName = ss.str();

			auto it = findModel(modelName);
			if (it != _modelsPool.cend()) {
				return std::weak_ptr<const Model>(it->second);
			}
			else {
				int numVertices = (slicesPrecision + 1) * (verticesPrecision + 1);
				int numIndices = slicesPrecision * verticesPrecision * 6;

				std::vector<float> vertexPositions = std::vector<float>(numVertices * 3);
				std::vector<float> vertexNormals = std::vector<float>(numVertices * 3);
				std::vector<float> textureCoordinates = std::vector<float>(numVertices * 2);
				std::vector<int> indices = std::vector<int>(numIndices);

				for (int slice = 0; slice <= slicesPrecision; ++slice) {
					for (int vertex = 0; vertex <= verticesPrecision; ++vertex) {
						float y = (float)cos(toRadians(180.0f - (float)slice * 180.0f / (float)slicesPrecision));
						float x = -(float)cos(toRadians((float)vertex * 360.0f / (float)verticesPrecision)) * (float)std::abs(cos(asin(y)));
						float z = (float)sin(toRadians((float)vertex * 360.0f / (float)verticesPrecision)) * (float)std::abs(std::cos(asin(y)));

						int vert = slice * (verticesPrecision + 1) + vertex;

						vertexPositions[vert * 3] = x;
						vertexPositions[vert * 3 + 1] = y;
						vertexPositions[vert * 3 + 2] = z;

						vertexNormals[vert * 3] = x;
						vertexNormals[vert * 3 + 1] = y;
						vertexNormals[vert * 3 + 2] = z;

						textureCoordinates[vert * 2] = (float)vertex / (float)verticesPrecision;
						textureCoordinates[vert * 2 + 1] = (float)slice / (float)slicesPrecision;
					}
				}

				for (int slice = 0; slice < slicesPrecision; ++slice) {
					for (int vertex = 0; vertex < verticesPrecision; ++vertex) {
						int pos = 6 * (slice * verticesPrecision + vertex);
						indices[pos + 0] = slice * (verticesPrecision + 1) + vertex;
						indices[pos + 1] = slice * (verticesPrecision + 1) + vertex + 1;
						indices[pos + 2] = (slice + 1) * (verticesPrecision + 1) + vertex;
						indices[pos + 3] = slice * (verticesPrecision + 1) + vertex + 1;
						indices[pos + 4] = (slice + 1) * (verticesPrecision + 1) + vertex + 1;
						indices[pos + 5] = (slice + 1) * (verticesPrecision + 1) + vertex;
					}
				}

				std::vector<float> modelVertexPositions;
				modelVertexPositions.reserve(numVertices * 3);

				std::vector<float> modelVertexNormals;
				modelVertexNormals.reserve(numVertices * 3);

				std::vector<float> modelTextureCoordinates;
				modelTextureCoordinates.reserve(numVertices * 2);
				//model.indices() = std::vector<int>(numIndices);

				for (auto it = indices.cbegin(); it != indices.cend(); ++it) {
					modelVertexPositions.push_back(vertexPositions[3 * (*it)]);
					modelVertexPositions.push_back(vertexPositions[3 * (*it) + 1]);
					modelVertexPositions.push_back(vertexPositions[3 * (*it) + 2]);

					modelVertexNormals.push_back(vertexNormals[3 * (*it)]);
					modelVertexNormals.push_back(vertexNormals[3 * (*it) + 1]);
					modelVertexNormals.push_back(vertexNormals[3 * (*it) + 2]);

					modelTextureCoordinates.push_back(textureCoordinates[2 * (*it)]);
					modelTextureCoordinates.push_back(textureCoordinates[2 * (*it) + 1]);
				}

				GLuint vertexPositionsIndex = createArrayBuffer(&(modelVertexPositions[0]), modelVertexPositions.size());
				GLuint vertexNormalsIndex = createArrayBuffer(&(modelVertexNormals[0]), modelVertexNormals.size());
				GLuint textureCoordinatesIndex = createArrayBuffer(&(modelTextureCoordinates[0]), modelTextureCoordinates.size());

				auto model = std::make_shared<Model>();
				model->setVertexPositions(vertexPositionsIndex);
				model->setVertexNormals(vertexNormalsIndex);
				model->setVertexTextureCoordinates(textureCoordinatesIndex);

				model->usesIndices() = false;
				model->numberOfElements() = modelVertexPositions.size();

				auto& modelRef = _modelsPool[modelName];
				modelRef = model;

				return std::weak_ptr<Model>(modelRef);
			}
		}

		std::weak_ptr<const Model> ModelsPool::createTorus(float innerRadius /*= 0.5f*/, float outerRadius /*= 0.2f*/, int precIn /*= 48*/) {
			std::stringstream ss(torusName);
			ss << "_" << innerRadius << "_" << outerRadius << "_" << precIn;
			std::string modelName = ss.str();

			auto it = findModel(modelName);
			if (it != _modelsPool.cend()) {
				return std::weak_ptr<const Model>(it->second);
			}
			else {
				int numVertices = (precIn + 1) * (precIn + 1);
				int numIndices = precIn * precIn * 6;

				std::vector<glm::vec3> sTangents = std::vector<glm::vec3>(numVertices);
				std::vector<glm::vec3> tTangents = std::vector<glm::vec3>(numVertices);

				std::vector<float> modelVertexPositions = std::vector<float>(numVertices * 3);
				std::vector<float> modelVertexNormals = std::vector<float>(numVertices * 3);
				std::vector<float> modelTextureCoordinates = std::vector<float>(numVertices * 2);
				std::vector<int> modelIndices = std::vector<int>(numIndices);

				for (int i = 0; i <= precIn; ++i) {
					float amt = toRadians((float)i * 360.0f / (float)precIn);
					glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 0.0f, 1.0f));
					glm::vec3 initPos(rMat * glm::vec4(outerRadius, 0.0f, 0.0f, 1.0f));
					glm::vec3 tmp = initPos + glm::vec3(innerRadius, 0.0f, 0.0f);
					modelVertexPositions[i * 3 + 0] = tmp.x;
					modelVertexPositions[i * 3 + 1] = tmp.y;
					modelVertexPositions[i * 3 + 2] = tmp.z;

					modelTextureCoordinates[i * 2 + 0] = 0.0f;
					modelTextureCoordinates[i * 2 + 1] = (float)i / (float)precIn;

					rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 0.0f, 1.0f));
					tTangents[i] = glm::vec3(rMat * glm::vec4(0.0f, -1.0f, 0.0f, 1.0f));
					sTangents[i] = glm::vec3(0.0f, 0.0f, -1.0f);

					tmp = glm::cross(tTangents[i], sTangents[i]);

					modelVertexNormals[i * 3 + 0] = tmp.x;
					modelVertexNormals[i * 3 + 1] = tmp.y;
					modelVertexNormals[i * 3 + 2] = tmp.z;
				}

				for (int ring = 1; ring <= precIn; ++ring) {
					for (int vert = 0; vert <= precIn; ++vert) {
						float amt = (float)(toRadians((float)ring * 360.0f / (float)precIn));
						glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
						int aux = (ring * (precIn + 1) + vert);
						glm::vec3 tmp = glm::vec3(rMat * glm::vec4(modelVertexPositions[vert * 3 + 0],
							modelVertexPositions[vert * 3 + 1],
							modelVertexPositions[vert * 3 + 2], 1.0f));
						modelVertexPositions[aux * 3 + 0] = tmp.x;
						modelVertexPositions[aux * 3 + 1] = tmp.y;
						modelVertexPositions[aux * 3 + 2] = tmp.z;

						modelTextureCoordinates[aux * 2 + 0] = (float)ring * 2.0f / (float)precIn;
						modelTextureCoordinates[aux * 2 + 1] = modelTextureCoordinates[vert * 2 + 1];
						if (modelTextureCoordinates[aux * 2 + 0] > 1.0f) {
							modelTextureCoordinates[aux * 2 + 0] -= 1.0f;
						}

						rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
						sTangents[aux] = glm::vec3(rMat * glm::vec4(sTangents[vert], 1.0f));
						rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
						tTangents[aux] = glm::vec3(rMat * glm::vec4(tTangents[vert], 1.0f));

						tmp = glm::cross(tTangents[aux], sTangents[aux]);

						modelVertexNormals[aux * 3 + 0] = tmp.x;
						modelVertexNormals[aux * 3 + 1] = tmp.y;
						modelVertexNormals[aux * 3 + 2] = tmp.z;
					}
				}

				for (int ring = 0; ring < precIn; ++ring) {
					for (int vert = 0; vert < precIn; ++vert) {
						int aux = (ring * precIn + vert) * 2;

						modelIndices[aux * 3 + 0] = ring * (precIn + 1) + vert;
						modelIndices[aux * 3 + 1] = (ring + 1) * (precIn + 1) + vert;
						modelIndices[aux * 3 + 2] = ring * (precIn + 1) + vert + 1;
						modelIndices[(aux + 1) * 3 + 0] = ring * (precIn + 1) + vert + 1;
						modelIndices[(aux + 1) * 3 + 1] = (ring + 1) * (precIn + 1) + vert;
						modelIndices[(aux + 1) * 3 + 2] = (ring + 1) * (precIn + 1) + vert + 1;

					}
				}

				GLuint vertexPositionsIndex = createArrayBuffer(&(modelVertexPositions[0]), modelVertexPositions.size());
				GLuint vertexNormalsIndex = createArrayBuffer(&(modelVertexNormals[0]), modelVertexNormals.size());
				GLuint textureCoordinatesIndex = createArrayBuffer(&(modelTextureCoordinates[0]), modelTextureCoordinates.size());
				GLuint indicesIndex = createElementArrayBuffer(&(modelIndices[0]), modelIndices.size());

				auto model = std::make_shared<Model>();
				model->setVertexPositions(vertexPositionsIndex);
				model->setVertexNormals(vertexNormalsIndex);
				model->setVertexTextureCoordinates(textureCoordinatesIndex);
				model->setIndices(indicesIndex);

				model->usesIndices() = true;
				model->numberOfElements() = modelIndices.size();

				auto& modelRef = _modelsPool[modelName];
				modelRef = model;

				return std::weak_ptr<Model>(modelRef);
			}
		}

		std::weak_ptr<const Model> ModelsPool::createModel(const std::string& fileName) {
			//TODO
			return std::weak_ptr<const Model>();
		}

		std::weak_ptr<const Model> ModelsPool::loadModel(const std::string& fileName) {
			//TODO
			return std::weak_ptr<const Model>();
		}

		std::map<std::string, std::shared_ptr<Model>>::const_iterator ModelsPool::findModel(const std::string& modelName)const {
			return _modelsPool.find(modelName);
		}
	}
}