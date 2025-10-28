// Sound
#pragma comment(lib, "winmm.lib")
#include <windows.h>
#include <mmsystem.h>
//
#include "CubeLogic.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <iomanip>

void CubeLogic::Initialize(GLFWwindow* window)
{
	m_cubieRenderer.Initialize();

	m_input.SetWindow(window);

	// debugging keys
	m_input.ObserveKey(GLFW_KEY_SPACE);
	m_input.ObserveKey(GLFW_KEY_R);

	// arrow keys
	m_input.ObserveKey(GLFW_KEY_RIGHT);
	m_input.ObserveKey(GLFW_KEY_LEFT);
	m_input.ObserveKey(GLFW_KEY_UP);
	m_input.ObserveKey(GLFW_KEY_DOWN);

	// numpad keys
	m_input.ObserveKey(GLFW_KEY_KP_0);
	m_input.ObserveKey(GLFW_KEY_KP_1);
	m_input.ObserveKey(GLFW_KEY_KP_2);
	m_input.ObserveKey(GLFW_KEY_KP_3);
	m_input.ObserveKey(GLFW_KEY_KP_4);
	m_input.ObserveKey(GLFW_KEY_KP_6);
	m_input.ObserveKey(GLFW_KEY_KP_7);
	m_input.ObserveKey(GLFW_KEY_KP_8);
	m_input.ObserveKey(GLFW_KEY_KP_9);

	// shift to switch from vertical to horizontal cube layers
	m_input.ObserveKey(GLFW_KEY_LEFT_SHIFT);

	// quaternion for transformation of whole cube
	m_orientationQuaternion = glm::quat(1.0f, glm::vec3(0.0f, 0.0f, 0.0f));

	// fill m_cubies with their rotation matrices
	SetUpCubies();
}

void CubeLogic::SetUpCubies()
{
	float gapBetweenCubies = 0.05f;
	float offset = m_cubieRenderer.GetCubieExtension() + gapBetweenCubies;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				m_cubies[i][j][k] = glm::mat4(1.0f);
				m_cubies[i][j][k] = glm::translate(glm::mat4(1.0f), glm::vec3((i - 1) * offset, (j - 1) * offset, (k - 1) * offset));
				startingPositions[i][j][k] = glm::mat4(1.0f);
				startingPositions[i][j][k] = glm::translate(glm::mat4(1.0f), glm::vec3((i - 1) * offset, (j - 1) * offset, (k - 1) * offset));
			}
		}
	}

	middleCubies[0] = &m_cubies[1][0][1];
	middleCubies[1] = &m_cubies[1][1][0];
	middleCubies[2] = &m_cubies[2][1][1];
	middleCubies[3] = &m_cubies[1][1][2];
	middleCubies[4] = &m_cubies[0][1][1];
	middleCubies[5] = &m_cubies[1][2][1];
}

void CubeLogic::Render(float aspectRatio)
{
	glm::mat4 globalTransformation = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f) // object to screen space coordinates
		* glm::lookAt(glm::vec3(0.0f, 0.0f, 9.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 cubieTransform = glm::mat4(1.0f);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				cubieTransform = globalTransformation * m_cubies[i][j][k];
				m_cubieRenderer.Render(cubieTransform);
			}
		}
	}
}

void CubeLogic::ClearResources()
{
	m_cubieRenderer.ClearResources();
}

void CubeLogic::RotateCube()
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				m_cubies[i][j][k] = glm::mat4_cast(m_orientationQuaternion) * m_cubies[i][j][k];
			}
		}
	}
}

void CubeLogic::FindMiddleCubie(char axis, int layer)
{
	if (layer == 1)
	{
		middleCubie = &m_cubies[1][1][1];
	}
	else
	{
		if (axis == 'x')
		{
			if (layer == 0)
			{
				float xMin = std::min({
					(*middleCubies[0])[3][0],
					(*middleCubies[1])[3][0],
					(*middleCubies[2])[3][0],
					(*middleCubies[3])[3][0],
					(*middleCubies[4])[3][0],
					(*middleCubies[5])[3][0] });

				for (int i = 0; i < 6; ++i)
				{
					if ((*middleCubies[i])[3][0] == xMin)
					{
						middleCubie = middleCubies[i];
						break;
					}
				}
			}
			else if (layer == 2)
			{
				float xMax = std::max({
					(*middleCubies[0])[3][0],
					(*middleCubies[1])[3][0],
					(*middleCubies[2])[3][0],
					(*middleCubies[3])[3][0],
					(*middleCubies[4])[3][0],
					(*middleCubies[5])[3][0] });

				for (int i = 0; i < 6; ++i)
				{
					if ((*middleCubies[i])[3][0] == xMax)
					{
						middleCubie = middleCubies[i];
						break;
					}
				}
			}
		}
		else if (axis == 'y')
		{
			if (layer == 0)
			{
				float yMin = std::min({
					(*middleCubies[0])[3][1],
					(*middleCubies[1])[3][1],
					(*middleCubies[2])[3][1],
					(*middleCubies[3])[3][1],
					(*middleCubies[4])[3][1],
					(*middleCubies[5])[3][1] });

				for (int i = 0; i < 6; ++i)
				{
					if ((*middleCubies[i])[3][1] == yMin)
					{
						middleCubie = middleCubies[i];
						break;
					}
				}
			}
			else if (layer == 2)
			{
				float yMax = std::max({
					(*middleCubies[0])[3][1],
					(*middleCubies[1])[3][1],
					(*middleCubies[2])[3][1],
					(*middleCubies[3])[3][1],
					(*middleCubies[4])[3][1],
					(*middleCubies[5])[3][1] });

				for (int i = 0; i < 6; ++i)
				{
					if ((*middleCubies[i])[3][1] == yMax)
					{
						middleCubie = middleCubies[i];
						break;
					}
				}
			}
		}
	}
}

void CubeLogic::FindLayer(char axis, int layer)
{
	/*
	y\x	 0 1 2
	  2 [][][]
	  1 [][][]
	  0 [][][]
	*/
	if (middleCubie == &m_cubies[1][1][1])
	{
		FindMiddleLayer(axis);
	} // layers are all defined circular and counter clockwise and the last cubie is always the middleCubie
	else if (middleCubie == &m_cubies[1][0][1])
	{
		currentLayer[0] = &m_cubies[0][0][0];
		currentLayer[1] = &m_cubies[0][0][1];
		currentLayer[2] = &m_cubies[0][0][2];
		currentLayer[3] = &m_cubies[1][0][2];
		currentLayer[4] = &m_cubies[2][0][2];
		currentLayer[5] = &m_cubies[2][0][1];
		currentLayer[6] = &m_cubies[2][0][0];
		currentLayer[7] = &m_cubies[1][0][0];
		currentLayer[8] = &m_cubies[1][0][1];
	}
	else if (middleCubie == &m_cubies[1][1][0])
	{
		currentLayer[0] = &m_cubies[0][0][0];
		currentLayer[1] = &m_cubies[1][0][0];
		currentLayer[2] = &m_cubies[2][0][0];
		currentLayer[3] = &m_cubies[2][1][0];
		currentLayer[4] = &m_cubies[2][2][0];
		currentLayer[5] = &m_cubies[1][2][0];
		currentLayer[6] = &m_cubies[0][2][0];
		currentLayer[7] = &m_cubies[0][1][0];
		currentLayer[8] = &m_cubies[1][1][0];
	}
	else if (middleCubie == &m_cubies[0][1][1])
	{
		currentLayer[0] = &m_cubies[0][0][0];
		currentLayer[1] = &m_cubies[0][1][0];
		currentLayer[2] = &m_cubies[0][2][0];
		currentLayer[3] = &m_cubies[0][2][1];
		currentLayer[4] = &m_cubies[0][2][2];
		currentLayer[5] = &m_cubies[0][1][2];
		currentLayer[6] = &m_cubies[0][0][2];
		currentLayer[7] = &m_cubies[0][0][1];
		currentLayer[8] = &m_cubies[0][1][1];
	}
	else if (middleCubie == &m_cubies[2][1][1])
	{
		currentLayer[0] = &m_cubies[2][0][0];
		currentLayer[1] = &m_cubies[2][1][0];
		currentLayer[2] = &m_cubies[2][2][0];
		currentLayer[3] = &m_cubies[2][2][1];
		currentLayer[4] = &m_cubies[2][2][2];
		currentLayer[5] = &m_cubies[2][1][2];
		currentLayer[6] = &m_cubies[2][0][2];
		currentLayer[7] = &m_cubies[2][0][1];
		currentLayer[8] = &m_cubies[2][1][1];
	}
	else if (middleCubie == &m_cubies[1][1][2])
	{
		currentLayer[0] = &m_cubies[0][0][2];
		currentLayer[1] = &m_cubies[1][0][2];
		currentLayer[2] = &m_cubies[2][0][2];
		currentLayer[3] = &m_cubies[2][1][2];
		currentLayer[4] = &m_cubies[2][2][2];
		currentLayer[5] = &m_cubies[1][2][2];
		currentLayer[6] = &m_cubies[0][2][2];
		currentLayer[7] = &m_cubies[0][1][2];
		currentLayer[8] = &m_cubies[1][1][2];
	}
	else if (middleCubie == &m_cubies[1][2][1])
	{
		currentLayer[0] = &m_cubies[0][2][0];
		currentLayer[1] = &m_cubies[0][2][1];
		currentLayer[2] = &m_cubies[0][2][2];
		currentLayer[3] = &m_cubies[1][2][2];
		currentLayer[4] = &m_cubies[2][2][2];
		currentLayer[5] = &m_cubies[2][2][1];
		currentLayer[6] = &m_cubies[2][2][0];
		currentLayer[7] = &m_cubies[1][2][0];
		currentLayer[8] = &m_cubies[1][2][1];
	}
}

void CubeLogic::FindMiddleLayer(char axis)
{
	// layer representatives
	glm::vec3 rep1, rep2, rep3;
	rep1 = m_cubies[2][1][1][3];
	rep2 = m_cubies[1][2][1][3];
	rep3 = m_cubies[1][1][2][3];
	float rep1_rep2 = 0.0f, rep2_rep3 = 0.0f, rep3_rep1 = 0.0f, smallest = 0.0f;
	if (axis == 'x') // look for the two representive cubies which have the lowest combined x position values
	{
		rep1_rep2 = std::abs(rep1.x) + std::abs(rep2.x); //absolute value to determine the rep-combination which is nearest to zero
		rep2_rep3 = std::abs(rep2.x) + std::abs(rep3.x);
		rep3_rep1 = std::abs(rep3.x) + std::abs(rep1.x);
	}
	else if (axis == 'y') // look for the two representive cubies which have the lowest combined y values
	{
		rep1_rep2 = std::abs(rep1.y) + std::abs(rep2.y);
		rep2_rep3 = std::abs(rep2.y) + std::abs(rep3.y);
		rep3_rep1 = std::abs(rep3.y) + std::abs(rep1.y);
	}
	smallest = std::min({ rep1_rep2, rep2_rep3, rep3_rep1 });
	if (smallest == rep1_rep2) // z-layer
	{
		if ((axis == 'x' && rep3.x < 0.0f) || (axis == 'y' && rep3.y < 0.0f)) // direction determination of circular layer defintion
		{
			currentLayer[0] = &m_cubies[0][0][1];
			currentLayer[1] = &m_cubies[0][1][1];
			currentLayer[2] = &m_cubies[0][2][1];
			currentLayer[3] = &m_cubies[1][2][1];
			currentLayer[4] = &m_cubies[2][2][1];
			currentLayer[5] = &m_cubies[2][1][1];
			currentLayer[6] = &m_cubies[2][0][1];
			currentLayer[7] = &m_cubies[1][0][1];
			currentLayer[8] = &m_cubies[1][1][1];
		}
		else
		{
			currentLayer[0] = &m_cubies[0][0][1];
			currentLayer[1] = &m_cubies[1][0][1];
			currentLayer[2] = &m_cubies[2][0][1];
			currentLayer[3] = &m_cubies[2][1][1];
			currentLayer[4] = &m_cubies[2][2][1];
			currentLayer[5] = &m_cubies[1][2][1];
			currentLayer[6] = &m_cubies[0][2][1];
			currentLayer[7] = &m_cubies[0][1][1];
			currentLayer[8] = &m_cubies[1][1][1];
		}

	}
	else if (smallest == rep2_rep3) // x-layer
	{
		if ((axis == 'x' && rep1.x < 0.0f) || (axis == 'y' && rep1.y < 0.0f))
		{
			currentLayer[0] = &m_cubies[1][0][0];
			currentLayer[1] = &m_cubies[1][0][1];
			currentLayer[2] = &m_cubies[1][0][2];
			currentLayer[3] = &m_cubies[1][1][2];
			currentLayer[4] = &m_cubies[1][2][2];
			currentLayer[5] = &m_cubies[1][2][1];
			currentLayer[6] = &m_cubies[1][2][0];
			currentLayer[7] = &m_cubies[1][1][0];
			currentLayer[8] = &m_cubies[1][1][1];
		}
		else
		{
			currentLayer[0] = &m_cubies[1][0][0];
			currentLayer[1] = &m_cubies[1][1][0];
			currentLayer[2] = &m_cubies[1][2][0];
			currentLayer[3] = &m_cubies[1][2][1];
			currentLayer[4] = &m_cubies[1][2][2];
			currentLayer[5] = &m_cubies[1][1][2];
			currentLayer[6] = &m_cubies[1][0][2];
			currentLayer[7] = &m_cubies[1][0][1];
			currentLayer[8] = &m_cubies[1][1][1];
		}
	}
	else if (smallest == rep3_rep1) // y-layer
	{
		if ((axis == 'x' && rep2.x < 0.0f) || (axis == 'y' && rep2.y < 0.0f))
		{
			currentLayer[0] = &m_cubies[0][1][0];
			currentLayer[1] = &m_cubies[1][1][0];
			currentLayer[2] = &m_cubies[2][1][0];
			currentLayer[3] = &m_cubies[2][1][1];
			currentLayer[4] = &m_cubies[2][1][2];
			currentLayer[5] = &m_cubies[1][1][2];
			currentLayer[6] = &m_cubies[0][1][2];
			currentLayer[7] = &m_cubies[0][1][1];
			currentLayer[8] = &m_cubies[1][1][1];
		}
		else
		{
			currentLayer[0] = &m_cubies[0][1][0];
			currentLayer[1] = &m_cubies[0][1][1];
			currentLayer[2] = &m_cubies[0][1][2];
			currentLayer[3] = &m_cubies[1][1][2];
			currentLayer[4] = &m_cubies[2][1][2];
			currentLayer[5] = &m_cubies[2][1][1];
			currentLayer[6] = &m_cubies[2][1][0];
			currentLayer[7] = &m_cubies[1][1][0];
			currentLayer[8] = &m_cubies[1][1][1];
		}
	}
}

glm::vec3 CubeLogic::FindRotationAxis(char axis, int CubieOfLayer)
{
	int ax = (axis == 'x') ? 0 : 1; // axis == y => 1
	int index = CubieOfLayer;
	// find max value of the three vectos which corresponses the most to the axis of the desired rotation
	int prefix = 0; // default = 0, positive = 1, negative = -1
	float max = std::max({ (*currentLayer[index])[0][ax], (*currentLayer[index])[1][ax], (*currentLayer[index])[2][ax] });
	float min = std::min({ (*currentLayer[index])[0][ax], (*currentLayer[index])[1][ax], (*currentLayer[index])[2][ax] });
	if (max >= min * -1) // find prefix of value which determines whether the rotation direction should be flipped or not
	{
		prefix = 1;
	}
	else
	{
		prefix = -1;
	}

	int indexOfHighestAbsoluteValue = 0; // 0 => x-Vec, 1 => y-Vec, 2 => z-Vec with highest absolute value
	for (int i = 0; i < 3; ++i)
	{
		if (prefix == 1)
		{
			if ((*currentLayer[index])[i][ax] == max)
			{
				indexOfHighestAbsoluteValue = i;
				break;
			}
		}
		else if (prefix == -1)
		{
			if ((*currentLayer[index])[i][ax] == min)
			{
				indexOfHighestAbsoluteValue = i;
				break;
			}
		}
	}

	glm::vec3 rotationAxis = glm::vec3(0.0f);
	if (indexOfHighestAbsoluteValue == 0) // x-vec
	{
		rotationAxis = glm::vec3(prefix, 0.0f, 0.0f);
	}
	else if (indexOfHighestAbsoluteValue == 1) // y-vec
	{
		rotationAxis = glm::vec3(0.0f, prefix, 0.0f);
	}
	else if (indexOfHighestAbsoluteValue == 2) // z-vec
	{
		rotationAxis = glm::vec3(0.0f, 0.0f, prefix);
	}
	return rotationAxis;
}

void CubeLogic::UpdateCubiePositions(char axis, int direction)
{
	int dir = FlipCubieTranslationDirectionIfNeeded(axis, direction);

	glm::vec4 newPos[8] = {};
	if (dir == 1)
	{
		int j = 2; // every cubie moves two positions forward
		for (int i = 0; i < 8; ++i)
		{
			if (j == 8) j = 0;
			newPos[i] = (*currentLayer[j])[3];
			++j;
		}
	}
	else if (dir == -1)
	{
		int j = 5;
		for (int i = 7; i >= 0; --i)
		{
			if (j == -1) j = 7;
			newPos[i] = (*currentLayer[j])[3];
			--j;
		}
	}

	// update positions
	for (int i = 0; i < 8; ++i)
	{
		(*currentLayer[i])[3] = newPos[i];
	}
}

void CubeLogic::UpdateCubieIndices(char axis, int direction)
{
	int dir = FlipCubieTranslationDirectionIfNeeded(axis, direction);

	glm::mat4 tempLayer[8] = {};
	if (dir == 1)
	{
		int j = 2;
		for (int i = 0; i < 8; ++i)
		{
			if (j == 8) j = 0;
			tempLayer[j] = *currentLayer[i];
			++j;
		}
	}
	else if (dir == -1)
	{
		int j = 5;
		for (int i = 7; i >= 0; --i)
		{
			if (j == -1) j = 7;
			tempLayer[j] = *currentLayer[i];
			--j;
		}
	}
	for (int i = 0; i < 8; ++i) // upadte index after new position
	{
		*currentLayer[i] = tempLayer[i];
	}
}

int CubeLogic::FlipCubieTranslationDirectionIfNeeded(char axis, int direction)
{
	if (((middleCubie == &m_cubies[2][1][1] || middleCubie == &m_cubies[1][2][1] || middleCubie == &m_cubies[1][1][2])
		&& ((axis == 'x' && (*middleCubie)[3].x < 0.0f) || (axis == 'y' && (*middleCubie)[3].y < 0.0f)))
		||
		((middleCubie == &m_cubies[1][1][0] || middleCubie == &m_cubies[0][1][1] || middleCubie == &m_cubies[1][0][1])
			&& ((axis == 'x' && (*middleCubie)[3].x > 0.0f) || (axis == 'y' && (*middleCubie)[3].y > 0.0f))))
	{
		direction = -direction;
	}
	return direction;
}

void CubeLogic::PlayRotationSound()
{
	int r = std::rand() % 5; // random number
	std::string sound = r == 0 ? "Sound1.wav" : r == 1 ? "Sound2.wav" : r == 2 ? "Sound3.wav" : r == 3 ? "Sound4.wav" : "Sound5.wav";
	std::string soundPath = "..\\Sounds\\" + sound;
	std::wstring relativePath(soundPath.begin(), soundPath.end()); // convert std::string to std::wstring for Unicode support
	PlaySound(relativePath.c_str(), NULL, SND_FILENAME | SND_ASYNC); // play the sound file asynchronously using its filename
}

void CubeLogic::RotateLayer(char axis, int direction, int layer)
{
	FindMiddleCubie(axis, layer);
	FindLayer(axis, layer);

	glm::vec3 rotationAxis = glm::vec3(0.0f);
	for (int i = 0; i < 9; ++i)
	{
		rotationAxis = FindRotationAxis(axis, i);
		*currentLayer[i] = glm::rotate(*currentLayer[i], glm::radians(90.0f * direction), rotationAxis);
	}

	UpdateCubiePositions(axis, direction);
	UpdateCubieIndices(axis, direction);

	PlayRotationSound();
}

void CubeLogic::ResetPosition()
{
	for (int x = 0; x < 3; ++x)
	{
		for (int y = 0; y < 3; ++y)
		{
			for (int z = 0; z < 3; ++z)
			{
				m_cubies[x][y][z] = startingPositions[x][y][z];
			}
		}
	}
}

void CubeLogic::HandleArrowKeys(double deltaTime)
{
	m_input.Update();
	m_orientationQuaternion = glm::quat(1.0f, glm::vec3(0.0f, 0.0f, 0.0f));

	float xVel = 0.0f;
	if (m_input.IsKeyDown(GLFW_KEY_UP))
		xVel = glm::radians(-90.0f);
	if (m_input.IsKeyDown(GLFW_KEY_DOWN))
		xVel = glm::radians(90.0f);

	float yVel = 0.0f;
	if (m_input.IsKeyDown(GLFW_KEY_RIGHT))
		yVel = glm::radians(90.0f);
	if (m_input.IsKeyDown(GLFW_KEY_LEFT))
		yVel = glm::radians(-90.0f);

	glm::quat velQuat = glm::quat(0.0f, glm::vec3(xVel, yVel, 0.0f));
	m_orientationQuaternion += 0.5f * (float(deltaTime)) * velQuat * m_orientationQuaternion;
	m_orientationQuaternion = normalize(m_orientationQuaternion);
	RotateCube();
}

void CubeLogic::HandleNumpadKeys()
{
	if (m_input.IsKeyDown(GLFW_KEY_LEFT_SHIFT))
	{ // vertical rotation
		if (m_input.WasKeyPressed(GLFW_KEY_KP_9))
			RotateLayer('x', -1, 2);// rotate right layer clockwise
		if (m_input.WasKeyPressed(GLFW_KEY_KP_8))
			RotateLayer('x', -1, 1);// rotate mid layer clockwise
		if (m_input.WasKeyPressed(GLFW_KEY_KP_7))
			RotateLayer('x', -1, 0);// rotate left layer clockwise

		if (m_input.WasKeyPressed(GLFW_KEY_KP_3))
			RotateLayer('x', 1, 2);// rotate right layer counter clockwise
		if (m_input.WasKeyPressed(GLFW_KEY_KP_2))
			RotateLayer('x', 1, 1);// rotate mid layer counter clockwise
		if (m_input.WasKeyPressed(GLFW_KEY_KP_1))
			RotateLayer('x', 1, 0);// rotate left layer counter clockwise
	}
	else
	{ // horizontal rotation
		if (m_input.WasKeyPressed(GLFW_KEY_KP_7))
			RotateLayer('y', -1, 2);// rotate top layer clockwise
		if (m_input.WasKeyPressed(GLFW_KEY_KP_4))
			RotateLayer('y', -1, 1);// rotate mid layer clockwise
		if (m_input.WasKeyPressed(GLFW_KEY_KP_1))
			RotateLayer('y', -1, 0);// rotate bot layer clockwise

		if (m_input.WasKeyPressed(GLFW_KEY_KP_9))
			RotateLayer('y', 1, 2);// rotate top layer counter clockwise
		if (m_input.WasKeyPressed(GLFW_KEY_KP_6))
			RotateLayer('y', 1, 1);// rotate mid layer counter clockwise
		if (m_input.WasKeyPressed(GLFW_KEY_KP_3))
			RotateLayer('y', 1, 0);// rotate bot layer counter clockwise
	}
}

void CubeLogic::ShowMatrixOfCubie()
{
	// reset position
	if (m_input.WasKeyPressed(GLFW_KEY_R))
		ResetPosition();
	// show position of cubies
	if (m_input.WasKeyPressed(GLFW_KEY_SPACE))
	{
		float x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3, x4, y4, z4, w4;
		glm::mat4 m = m_cubies[2][1][2];
		x1 = m[0][0];x2 = m[1][0];x3 = m[2][0];x4 = m[3][0];
		y1 = m[0][1];y2 = m[1][1];y3 = m[2][1];y4 = m[3][1];
		z1 = m[0][2];z2 = m[1][2];z3 = m[2][2];z4 = m[3][2];
		w1 = m[0][3];w2 = m[1][3];w3 = m[2][3];w4 = m[3][3];

		std::cout << std::fixed << std::setprecision(2);
		std::cout << "x-Vector " << "\ty-Vector " << "\tz-Vector " << "\tt-Vector " << "\n";
		std::cout << "x1: " << x1 << "\tx2: " << x2 << "\tx3: " << x3 << "\tx4: " << x4 << "\n";
		std::cout << "y1: " << y1 << "\ty2: " << y2 << "\ty3: " << y3 << "\ty4: " << y4 << "\n";
		std::cout << "z1: " << z1 << "\tz2: " << z2 << "\tz3: " << z3 << "\tz4: " << z4 << "\n";
		std::cout << "w1: " << w1 << "\tw2: " << w2 << "\tw3: " << w3 << "\tw4: " << w4 << "\n\n";
	}
}

void CubeLogic::Update(double deltaTime)
{
	HandleArrowKeys(deltaTime);
	HandleNumpadKeys();
	ShowMatrixOfCubie();
}
