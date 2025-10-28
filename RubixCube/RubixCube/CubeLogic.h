#pragma once
#include "GameInterface.h"
#include "CubieRenderer.h"
#include "InputSystem.h"
#include <glm/ext/quaternion_float.hpp>

class CubeLogic : public GameInterface
{
public:
	void Initialize(GLFWwindow* window);
	void Render(float aspectRatio);
	void ClearResources();
	void Update(double deltaTime);

	void HandleArrowKeys(double deltaTime);
	void HandleNumpadKeys();
	void ShowMatrixOfCubie();

	void SetUpCubies();
	void ResetPosition();

	void RotateCube();
	void RotateLayer(char axis, int direction, int layer);

	void FindMiddleCubie(char axis, int layer);
	void FindLayer(char axis, int layer);
	void FindMiddleLayer(char axis);
	glm::vec3 FindRotationAxis(char axis, int CubeOfLayer);

	void UpdateCubiePositions(char axis, int direction);
	void UpdateCubieIndices(char axis, int direction);
	int FlipCubieTranslationDirectionIfNeeded(char axis, int direction);


	void PlayRotationSound();

private:
	CubieRenderer m_cubieRenderer;
	InputSystem m_input;
	glm::quat m_orientationQuaternion;
	glm::mat4 m_cubies[3][3][3];
	glm::mat4 startingPositions[3][3][3];
	glm::mat4* currentLayer[9];
	glm::mat4* middleCubies[6];
	glm::mat4* middleCubie;
};