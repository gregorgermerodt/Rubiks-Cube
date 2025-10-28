#include "CubieRenderer.h"
#include "ShaderUtil.h"
#include <glm/gtc/type_ptr.hpp>

void CubieRenderer::Initialize()
{
	float floatArray[6 * 6 * 3]; // target array; 6 cube faces, 6 vertices, each vertex has 3 floats
	std::vector<glm::vec3> positionField;
	std::vector<glm::vec3> colorField;

	// Build the cube information.
	for (int sideType = 0; sideType < 3; ++sideType)
	{
		for (int direction = -1; direction < 2; direction += 2)
		{
			AddSidePosition(sideType, direction, positionField);      // Add vertex positions for each cube face side
			AddSideColor(sideType, direction, colorField);            // Add colors for each cube face side
		}
	}

	m_shaderProgram = ShaderUtil::CreateShaderProgram("VertexShaderColor.glsl", "FragmentShaderColor.glsl"); // Create shader program from files
	m_transformLocation = glGetUniformLocation(m_shaderProgram, "transformation");                           // Get uniform location for transformation matrix

	glGenVertexArrays(1, &m_arrayBufferObject);               // filled with information of both vertex buffer objects and how their contents map on the input parameters of the vertex shader
	glGenBuffers(2, m_vertexBufferObject);                    // Generate two VBOs (Vertex Buffer Objects)

	glBindVertexArray(m_arrayBufferObject);					  // Bind the VAO

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject[0]);   // Bind first VBO (positions)
	TranscribeToFloatArray(positionField, floatArray);        // Convert vector data to float array
	glBufferData(GL_ARRAY_BUFFER, sizeof(floatArray), floatArray, GL_STATIC_DRAW); // Upload position data to GPU buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0)); // Describe vertex attribute 0 (position)
	glEnableVertexAttribArray(0);                             // Enable vertex attribute 0

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject[1]);   // Bind second VBO (colors)
	TranscribeToFloatArray(colorField, floatArray);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floatArray), floatArray, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0)); // Describe vertex attribute 1 (color)
	glEnableVertexAttribArray(1);                             // Enable vertex attribute 1

	glBindBuffer(GL_ARRAY_BUFFER, 0);                         // Unbind any buffer
	glBindVertexArray(0);                                     // Unbind VAO
}

void CubieRenderer::Render(const glm::mat4& transformationMatrix)
{
	glUseProgram(m_shaderProgram);                            // Use the compiled shader program
	glBindVertexArray(m_arrayBufferObject);                   // Bind VAO for drawing

	glUniformMatrix4fv(m_transformLocation, 1, GL_FALSE, value_ptr(transformationMatrix)); // Upload transformation matrix uniform
	glDrawArrays(GL_TRIANGLES, 0, 6 * 6);                     // draw 36 verticies

	glBindVertexArray(0);                                     // Unbind VAO
	glUseProgram(0);                                          // Unbind shader program
}

void CubieRenderer::ClearResources()
{
	glDeleteBuffers(2, m_vertexBufferObject);                 // Delete the two vertex buffer objects
	glDeleteVertexArrays(1, &m_arrayBufferObject);            // Delete the VAO
	glDeleteProgram(m_shaderProgram);                         // Delete the shader program
}

void CubieRenderer::AddSidePosition(int sideType, int direction, std::vector<glm::vec3>& positionArray)
{
	glm::vec3 cornerPoints[2][2];                             // 4 corner points for a face (2x2 grid)

	int localXDim = (sideType + 1) % 3;                       // Determine local X axis (other than sideType)
	int localYDim = (sideType + 2) % 3;						  // Determine local Y axis

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			glm::vec3 localPoint(direction * m_offset);       // Start with fixed offset along sideType axis
			localPoint[localXDim] = (2 * i - 1) * m_offset;   // Set local X coordinate to -offset or offset
			localPoint[localYDim] = (2 * j - 1) * m_offset;   // Set local Y coordinate to -offset or offset
			cornerPoints[i][j] = localPoint;                  // Store corner point
		}
	}

	// Create two triangles from the 4 corners (6 vertices)
	positionArray.push_back(cornerPoints[0][0]);
	positionArray.push_back(cornerPoints[1][0]);
	positionArray.push_back(cornerPoints[0][1]);
	positionArray.push_back(cornerPoints[1][0]);
	positionArray.push_back(cornerPoints[0][1]);
	positionArray.push_back(cornerPoints[1][1]);
}

void CubieRenderer::AddSideColor(int sideType, int direction, std::vector<glm::vec3>& colorArray)
{
	glm::vec3 color;

	// Classic Rubik's Cube colors
	if (sideType == 0)                                         // X axis
		color = (direction == 1) ? glm::vec3(1.0f, 0.5f, 0.0f) : glm::vec3(1.0f, 0.0f, 0.0f); // Orange & Red
	else if (sideType == 1)                                    // Y axis
		color = (direction == 1) ? glm::vec3(1.0f, 1.0f, 1.0f) : glm::vec3(1.0f, 1.0f, 0.0f); // White & Yellow
	else if (sideType == 2)                                    // Z axis
		color = (direction == 1) ? glm::vec3(0.0f, 0.0f, 1.0f) : glm::vec3(0.0f, 1.0f, 0.0f); // Blue & Green

	for (int i = 0; i < 6; ++i)
		colorArray.push_back(color);                           // Add the same color for all 6 vertices of the face
}

void CubieRenderer::TranscribeToFloatArray(std::vector<glm::vec3>& vecArray, float* floatArray)
{
	int writingCounter = 0;
	for (int i = 0; i < 36; ++i)                               // For all 36 vertices
		for (int coord = 0; coord < 3; ++coord)                // For each coordinate (x,y,z)
			floatArray[writingCounter++] = vecArray[i][coord]; // Copy float values sequentially into the flat array
}
