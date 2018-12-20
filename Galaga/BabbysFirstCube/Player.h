#pragma once
#include "GameEntity.h"

class Player : public GameEntity
{
private:
	int bulletCount;
	bool spaceUp;
	irrklang::ISoundEngine* soundEngine;

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
	void BulletsRender(Camera* camera);

	/// <summary>
	/// Creates a bullet
	/// </summary>
	void GenerateBullet();

public:
	Player(
		Mesh* mesh,
		Material* material,
		glm::vec3 position,
		glm::vec3 eulerAngles,
		glm::vec3 scale,
		GLFWwindow* window,
		irrklang::ISoundEngine* soundEngine
	);
	~Player();
	GameEntity** bullets;
	GameEntity** enemies;
	int* enemyCount;

	/// <summary>
	/// Updates the worldMatrix
	/// </summary>
	void Update();

	/// <summary>
	/// Renders the gameEntity based on a camera
	/// </summary>
	void Render(Camera* camera);
};

