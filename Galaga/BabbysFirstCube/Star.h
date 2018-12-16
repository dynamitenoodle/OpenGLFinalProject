#pragma once
#include "GameEntity.h"

class Star : public GameEntity
{
private:
	int flickerTimer;
	int flickerTimerMax;
public:
	Star(
		Mesh* mesh,
		Material* material,
		glm::vec3 position,
		glm::vec3 eulerAngles,
		glm::vec3 scale,
		GLFWwindow* window
	);
	~Star();
	void Update();
};

