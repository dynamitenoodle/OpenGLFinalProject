#include "Star.h"
#include "glm/gtc/matrix_transform.hpp"
#include "stdafx.h"
#include <stdlib.h>

Star::Star(Mesh * mesh,
	Material * material,
	glm::vec3 position,
	glm::vec3 eulerAngles,
	glm::vec3 scale,
	GLFWwindow* window)
{
	this->mesh = mesh;
	this->material = material;
	this->position = position;
	this->eulerAngles = eulerAngles;
	this->scale = scale;
	this->worldMatrix = glm::identity<glm::mat4>();

	this->color.x = 0.0f;
	this->color.y = 0.0f;
	this->color.z = 0.0f;

	this->window = window;

	speed = 0.02f;
	flickerTimer = 0;
	flickerTimerMax = 30 + (rand() % 60);
}


Star::~Star()
{
}

void Star::Update()
{
	this->position.y -= speed;
	
	// Y reflection
	if (this->position.y < -4.1f)
	{
		this->position.x = (rand() % 690) / 100.0f - 3.45f; 
		this->NewColor((rand() % 100) / 100.0f, (rand() % 100) / 100.0f, (rand() % 100) / 100.0f, 1.f);
		this->position.y = 4.1f;
	}

#pragma region Flicker Timer
	flickerTimer++;
	if (flickerTimer >= flickerTimerMax)
	{
		flickerTimer = 0;
		flickerTimerMax = 30 + (rand() % 60);
		int random = rand() % 10;
		if (random == 0)
			color.a = 0.0f;
		else
			color.a = 1.0f;
	}
#pragma endregion

	this->worldMatrix = glm::translate(
		glm::identity<glm::mat4>(),
		this->position
	);

	this->worldMatrix = glm::rotate(
		worldMatrix,
		this->eulerAngles.y,
		glm::vec3(0.f, 1.f, 0.f)
	);

	this->worldMatrix = glm::rotate(
		worldMatrix,
		this->eulerAngles.x,
		glm::vec3(1.f, 0.f, 0.f)
	);

	this->worldMatrix = glm::rotate(
		worldMatrix,
		this->eulerAngles.z,
		glm::vec3(0.f, 0.f, 1.f)
	);

	this->worldMatrix = glm::scale(
		worldMatrix,
		this->scale
	);
}
