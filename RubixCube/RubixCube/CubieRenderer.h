#pragma once
#include <glm/mat4x4.hpp>
#include <GL/glew.h>
#include <vector>

class CubieRenderer
{
public:
	void Initialize();
	void Render(const glm::mat4& transformationMatrix); // when rendered, the center point is used
	void ClearResources();

	float GetCubieExtension() const { return 2.0f * m_offset; }

private:
	const float m_offset = 0.5f; // half the size of the cube

	// sideType: 0 perpendicular to the x-axis, 1 to y, 2 to z; direction 1 or -1
	void AddSidePosition(int sideType, int direction, std::vector<glm::vec3>& positionArray); // store vertex positions in positionArray
	void AddSideColor(int sideType, int direction, std::vector<glm::vec3>& colorArray);
	void TranscribeToFloatArray(std::vector<glm::vec3>& vecArray, float* floatArray); // vec to float array


	GLuint m_arrayBufferObject;
	GLuint m_vertexBufferObject[2]; // objects for position and color
	GLuint m_shaderProgram;
	GLint m_transformLocation;
};