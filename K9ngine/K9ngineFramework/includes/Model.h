#ifndef MODEL_H_
#define MODEL_H_

#define SET_ATTRIBUTE(attributeName) setAttribute(#attributeName, attributeName)

#include <K9Debug.h>

#include <GL/glew.h>
#include <map>
#include <string>

namespace K9 {
	namespace Graphics {
		class Model {
		public:
			Model() :
				_usesIndices(false),
				_numberOfElements(0){
			}

			~Model() = default;

			const bool& usesIndices() const {
				return _usesIndices;
			}

			bool& usesIndices() {
				return _usesIndices;
			}

			const size_t& numberOfElements() const {
				return _numberOfElements;
			}

			size_t& numberOfElements() {
				return _numberOfElements;
			}

			void setVertexPositions(GLuint vertexPositionsIndex) {
				SET_ATTRIBUTE(vertexPositionsIndex);
			}

			GLuint getVertexPositions() const{
				K9_ASSERT(_numberOfElements > 0);

				return getAttribute("vertexPositionsIndex");
			}

			void setVertexNormals(GLuint vertexNormalsIndex) {
				SET_ATTRIBUTE(vertexNormalsIndex);
			}

			GLuint getVertexNormals() const{
				K9_ASSERT(_numberOfElements > 0);

				return getAttribute("vertexNormalsIndex");
			}

			void setVertexTextureCoordinates(GLuint vertexTextureCoordinates) {
				SET_ATTRIBUTE(vertexTextureCoordinates);
			}

			GLuint getVertexTextureCoordinates() const{
				K9_ASSERT(_numberOfElements > 0);

				return getAttribute("vertexTextureCoordinates");
			}

			void setIndices(GLuint indices) {
				SET_ATTRIBUTE(indices);
			}

			GLuint getIndices() const {
				K9_ASSERT(_usesIndices);
				K9_ASSERT(_numberOfElements > 0);

				return getAttribute("indices");
			}

		private:
			void setAttribute(const std::string& attributeName, GLuint attributeValue) {
				_attributes[attributeName] = attributeValue;
			}

			GLuint getAttribute(const std::string& attributeName) const{
				auto attrIt = _attributes.find(attributeName);
				K9_ASSERT(attrIt != _attributes.end());
				if (attrIt == _attributes.end()) {
					return 0;
				}
				else {
					return attrIt->second;
				}
			}

			bool _usesIndices;
			size_t _numberOfElements;
			std::map<std::string, GLuint> _attributes;
		};
	}
}

#endif // !MODEL_H_
