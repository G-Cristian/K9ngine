#include <OpenGLWrapper.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

GLFWwindow* window;

GLuint vao;
GLuint cubeVertPos;
GLuint cubeVertNorm;
GLuint cubeTexCoord;

GLuint pyramidVertPos;
GLuint pyramidVertNorm;
GLuint pyramidTexCoord;

GLuint torusVertPos;
GLuint torusVertNorm;
GLuint torusTexCoord;
GLuint torusIndices;

int width = 600, height = 600;
float fov = 1.0472f, zNear = 0.1f, zFar = 1000.0f;

glm::mat4 projMat;

void init(GLFWwindow* window);
void display(GLFWwindow* window, double dt);

void windowResizeCallback(GLFWwindow* window, int newWidth, int newHeight);

float toRadians(float degrees);
void createCube(float size=2.0f);
void createPyramid(float height = 2.0f, float base = 2.0f);
void createTorus(float innerRadius = 0.5f, float outerRadius = 0.2f, int precIn = 48);

int main() {
	/*
	int bufferSize = 3;
	bool moreErrors = true;
	while (moreErrors) {
		int* errorsBuffer = (int*)malloc(bufferSize * sizeof(int));
		int retVal = getOpenGLError(errorsBuffer, bufferSize, &moreErrors);
		std::cout << "retVal: " << retVal << std::endl;
		for (int i = 0; i < retVal; ++i) {
			std::cout << "i: " << i << " errorsBuffer[i]: " << errorsBuffer[i] << std::endl;
		}
		std::cout << "moreErrors: " << moreErrors << std::endl;
		free(errorsBuffer);
	}
	*/

	/*
	int logSize = 50;
	int numberOfBuffers = 3;
	bool moreToRead = true;
	while (moreToRead) {
		char** logBuffers = (char**)malloc(numberOfBuffers * sizeof(char*));
		for (int i = 0; i != numberOfBuffers; ++i) {
			logBuffers[i] = (char*)malloc(logSize * sizeof(char));
		}

		int retVal = getShaderLog(1, logBuffers, logSize, numberOfBuffers, &moreToRead);
		std::cout << "retVal: " << retVal << std::endl;
		for (int i = 0; i < retVal; ++i) {
			std::cout << "i: " << i << " logBuffers[i]: " << logBuffers[i] << std::endl;
		}
		std::cout << "moreToRead: " << moreToRead << std::endl;

		for (int i = 0; i != numberOfBuffers; ++i) {
			free(logBuffers[i]);
		}
		free(logBuffers);
	}
	
	//std::cout << getShaderLog(1) << std::endl;
	//std::cout << getProgramLog(0) << std::endl;
	*/

	if (!glfwInit()) {
		std::cout << "Error at line " << __LINE__ << " (!glfwInit())" << std::endl;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	window = glfwCreateWindow(width, height, "Test shadows", NULL, NULL);

	glfwMakeContextCurrent(window);

	if (initGlew() != GLEW_OK) {
		std::cout << "Error at line " << __LINE__ << " (initGlew() != GLEW_OK)" << std::endl;
	}

	glfwSwapInterval(1);
	glfwSetWindowSizeCallback(window, windowResizeCallback);

	init(window);

	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}

void init(GLFWwindow* window) {
	int code(0);
	std::string msg;

	vao = createAndBindVAO();
	createCube();

	GLuint renderingPRogram = createRenderingProgram("shadowPassOneVertShader.glsl", "shadowPassOneFragShader.glsl", code, msg);
}

void display(GLFWwindow* window, double dt) {

}

void windowResizeCallback(GLFWwindow* window, int newWidth, int newHeight) {
	float aspect = (float)(newWidth) / (float)(newHeight);
	width = newWidth;
	height = newHeight;
	glViewport(0, 0, newWidth, newHeight);
	projMat = glm::perspective(fov, aspect, zNear, zFar);
}

float toRadians(float degrees) {
	return (degrees * 3.14159f / 180.0f);
}

void createCube(float size/*=2.0f*/) {
	float halfSize = size / 2.0f;
	std::vector<float> vpos = {
	-halfSize, halfSize, -halfSize, -halfSize, -halfSize, -halfSize, halfSize, -halfSize, -halfSize,
	halfSize, -halfSize, -halfSize, halfSize, halfSize, -halfSize, -halfSize, halfSize, -halfSize,
	halfSize, -halfSize, -halfSize, halfSize, -halfSize, halfSize, halfSize, halfSize, -halfSize,
	halfSize, -halfSize, halfSize, halfSize, halfSize, halfSize, halfSize, halfSize, -halfSize,
	halfSize, -halfSize, halfSize, -halfSize, -halfSize, halfSize, halfSize, halfSize, halfSize,
	-halfSize, -halfSize, halfSize, -halfSize, halfSize, halfSize, halfSize, halfSize, halfSize,
	-halfSize, -halfSize, halfSize, -halfSize, -halfSize, -halfSize, -halfSize, halfSize, halfSize,
	-halfSize, -halfSize, -halfSize, -halfSize, halfSize, -halfSize, -halfSize, halfSize, halfSize,
	-halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize, -halfSize, -halfSize,
	halfSize, -halfSize, -halfSize, -halfSize, -halfSize, -halfSize, -halfSize, -halfSize, halfSize,
	-halfSize, halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize, halfSize,
	halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, -halfSize
	};

	std::vector<float> vnorm = {
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

	std::vector<float> texCoord = {
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f
	};

	cubeVertPos = createArrayBuffer(&(vpos[0]), vpos.size());
	cubeVertNorm = createArrayBuffer(&(vnorm[0]), vnorm.size());
	cubeTexCoord = createArrayBuffer(&(texCoord[0]), texCoord.size());
}

void createPyramid(float height/*=2.0f*/, float base/*=2.0f*/) {
	float halfHeight = height / 2.0f;
	float halfBase = base / 2.0f;

	std::vector<float> vpos = {
		-halfBase, -halfHeight, halfBase, halfBase, -halfHeight, halfBase, 0.0f, halfHeight, 0.0f,
		halfBase, -halfHeight, halfBase, halfBase, -halfHeight, -halfBase, 0.0f, halfHeight, 0.0f,
		halfBase, -halfHeight, -halfBase, -halfBase, -halfHeight, -halfBase, 0.0f, halfHeight, 0.0f,
		-halfBase, -halfHeight, -halfBase, -halfBase, -halfHeight, halfBase, 0.0f, halfHeight, 0.0f,
		-halfBase, -halfHeight, -halfBase, halfBase, -halfHeight, halfBase, -halfBase, -halfHeight, halfBase,
		halfBase, -halfHeight, halfBase, -halfBase, -halfHeight, -halfBase, halfBase, -halfHeight, -halfBase
	};
	glm::vec3 front1 = glm::normalize(glm::vec3(base, 0.0f, 0.0f));
	glm::vec3 front2 = glm::normalize(glm::vec3(halfBase, height, -halfBase));
	glm::vec3 frontNorm = glm::normalize(glm::cross(front1, front2));

	glm::vec3 right1 = glm::normalize(glm::vec3(0.0f, 0.0f, -base));
	glm::vec3 right2 = glm::normalize(glm::vec3(-halfBase, height, -halfBase));
	glm::vec3 rightNorm = glm::normalize(glm::cross(right1, right2));

	glm::vec3 back1 = glm::normalize(glm::vec3(-base, 0.0f, 0.0f));
	glm::vec3 back2 = glm::normalize(glm::vec3(-halfBase, height, halfBase));
	glm::vec3 backNorm = glm::normalize(glm::cross(back1, back2));

	glm::vec3 left1 = glm::normalize(glm::vec3(0.0f, 0.0f, base));
	glm::vec3 left2 = glm::normalize(glm::vec3(halfBase, height, halfBase));
	glm::vec3 leftNorm = glm::normalize(glm::cross(left1, left2));

	glm::vec3 bottom1 = glm::normalize(glm::vec3(0.0f, 0.0f, -base));
	glm::vec3 bottom2 = glm::normalize(glm::vec3(base, 0.0f, 0.0f));
	glm::vec3 bottomNorm = glm::normalize(glm::cross(bottom1, bottom2));

	std::vector<float> vnorm = {
		frontNorm.x, frontNorm.y, frontNorm.z, frontNorm.x, frontNorm.y, frontNorm.z, frontNorm.x, frontNorm.y, frontNorm.z,
		rightNorm.x, rightNorm.y, rightNorm.z, rightNorm.x, rightNorm.y, rightNorm.z, rightNorm.x, rightNorm.y, rightNorm.z,
		backNorm.x, backNorm.y, backNorm.z, backNorm.x, backNorm.y, backNorm.z, backNorm.x, backNorm.y, backNorm.z,
		leftNorm.x, leftNorm.y, leftNorm.z, leftNorm.x, leftNorm.y, leftNorm.z, leftNorm.x, leftNorm.y, leftNorm.z,
		bottomNorm.x, bottomNorm.y, bottomNorm.z, bottomNorm.x, bottomNorm.y, bottomNorm.z, bottomNorm.x, bottomNorm.y, bottomNorm.z,
		bottomNorm.x, bottomNorm.y, bottomNorm.z, bottomNorm.x, bottomNorm.y, bottomNorm.z, bottomNorm.x, bottomNorm.y, bottomNorm.z
	};

	std::vector<float> texCoord = {
		0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
	};

	pyramidVertPos = createArrayBuffer(&(vpos[0]), vpos.size());
	pyramidVertNorm = createArrayBuffer(&(vnorm[0]), vnorm.size());
	pyramidTexCoord = createArrayBuffer(&(texCoord[0]), texCoord.size());
}

void createTorus(float innerRadius /*= 0.5f*/, float outerRadius /*= 0.2f*/, int precIn /*= 48*/) {
	int numVertices = (precIn + 1) * (precIn + 1);
	int numIndices = precIn * precIn * 6;

	std::vector<glm::vec3> sTangents = std::vector<glm::vec3>(numVertices);
	std::vector<glm::vec3> tTangents = std::vector<glm::vec3>(numVertices);

	std::vector<float> vpos = std::vector<float>(numVertices * 3);
	std::vector<float> vnorm = std::vector<float>(numVertices * 3);
	std::vector<float> texCoord = std::vector<float>(numVertices * 2);
	std::vector<int> indices = std::vector<int>(numIndices);

	for (int i = 0; i <= precIn; ++i) {
		float amt = toRadians((float)i * 360.0f / (float)precIn);
		glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::vec3 initPos(rMat * glm::vec4(outerRadius, 0.0f, 0.0f, 1.0f));
		glm::vec3 tmp = initPos + glm::vec3(innerRadius, 0.0f, 0.0f);
		vpos[i * 3 + 0] = tmp.x;
		vpos[i * 3 + 1] = tmp.y;
		vpos[i * 3 + 2] = tmp.z;

		texCoord[i * 2 + 0] = 0.0f;
		texCoord[i * 2 + 1] = (float)i / (float)precIn;

		rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 0.0f, 1.0f));
		tTangents[i] = glm::vec3(rMat * glm::vec4(0.0f, -1.0f, 0.0f, 1.0f));
		sTangents[i] = glm::vec3(0.0f, 0.0f, -1.0f);

		tmp = glm::cross(tTangents[i], sTangents[i]);

		vnorm[i * 3 + 0] = tmp.x;
		vnorm[i * 3 + 1] = tmp.y;
		vnorm[i * 3 + 2] = tmp.z;
	}

	for (int ring = 1; ring <= precIn; ++ring) {
		for (int vert = 0; vert <= precIn; ++vert) {
			float amt = (float)(toRadians((float)ring * 360.0f / (float)precIn));
			glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
			int aux = (ring * (precIn + 1) + vert);
			glm::vec3 tmp = glm::vec3(rMat * glm::vec4(vpos[vert * 3 + 0],
				vpos[vert * 3 + 1],
				vpos[vert * 3 + 2], 1.0f));
			vpos[aux * 3 + 0] = tmp.x;
			vpos[aux * 3 + 1] = tmp.y;
			vpos[aux * 3 + 2] = tmp.z;

			texCoord[aux * 2 + 0] = (float)ring * 2.0f / (float)precIn;
			texCoord[aux * 2 + 1] = texCoord[vert * 2 + 1];
			if (texCoord[aux * 2 + 0] > 1.0f) {
				texCoord[aux * 2 + 0] -= 1.0f;
			}

			rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
			sTangents[aux] = glm::vec3(rMat * glm::vec4(sTangents[vert], 1.0f));
			rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
			tTangents[aux] = glm::vec3(rMat * glm::vec4(tTangents[vert], 1.0f));

			tmp = glm::cross(tTangents[aux], sTangents[aux]);

			vnorm[aux * 3 + 0] = tmp.x;
			vnorm[aux * 3 + 1] = tmp.y;
			vnorm[aux * 3 + 2] = tmp.z;
		}
	}

	for (int ring = 0; ring < precIn; ++ring) {
		for (int vert = 0; vert < precIn; ++vert) {
			int aux = (ring * precIn + vert) * 2;

			indices[aux * 3 + 0] = ring * (precIn + 1) + vert;
			indices[aux * 3 + 1] = (ring + 1) * (precIn + 1) + vert;
			indices[aux * 3 + 2] = ring * (precIn + 1) + vert + 1;
			indices[(aux + 1) * 3 + 0] = ring * (precIn + 1) + vert + 1;
			indices[(aux + 1) * 3 + 1] = (ring + 1) * (precIn + 1) + vert;
			indices[(aux + 1) * 3 + 2] = (ring + 1) * (precIn + 1) + vert + 1;

		}
	}

	torusVertPos = createArrayBuffer(&(vpos[0]), vpos.size());
	torusVertNorm = createArrayBuffer(&(vnorm[0]), vnorm.size());
	torusTexCoord = createArrayBuffer(&(texCoord[0]), texCoord.size());
}