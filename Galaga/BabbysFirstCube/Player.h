#pragma once
#include "GameEntity.h"

class Player : public GameEntity
{
private:

	/// <summary>
	/// Checks to see if we should move
	/// </summary>
	void InputCheck();

	/// <summary>
	/// Checks to see if we should move
	/// </summary>
	void BulletsUpdate();

	/// <summary>
	/// Checks to see if we should move
	/// </summary>
	void BulletsRender();

	int bulletCount;
public:
	Player(
		Mesh* mesh,
		Material* material,
		glm::vec3 position,
		glm::vec3 eulerAngles,
		glm::vec3 scale,
		GLFWwindow* window
	);
	~Player();
	GameEntity** bullets;

	/// <summary>
	/// Updates the worldMatrix
	/// </summary>
	void Update();

	/// <summary>
	/// Renders the gameEntity based on a camera
	/// </summary>
	void Render(Camera* camera);
};

