#include "Player.h"
#include "glm/gtc/matrix_transform.hpp"
#include "stdafx.h"
#include <stdlib.h>

Player::Player(Mesh * mesh,
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

	speed = 0.05f;
	bullets = new GameEntity*[10];
	bulletCount = 0;
}


Player::~Player()
{
}

void Player::Update()
{
	this->BulletsUpdate();
	this->InputCheck();

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

void Player::InputCheck()
{
	float width = 2.85f;
	if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) && this->position.x < width)
	{
		this->position.x += speed;
	}

	if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) && this->position.x > -width)
	{
		this->position.x -= speed;
	}
}

void Player::BulletsUpdate()
{
	for (int i = 0; i < bulletCount; i++)
	{
		bullets[i]->Update();
	}
}

void Player::Render(Camera* camera)
{
	// setting the color
	this->ChangeColor(this->color.x, this->color.y, this->color.z, this->color.a);
	// bind the material
	material->Bind(camera, worldMatrix);
	mesh->Render();
}

void Player::BulletsRender()
{
	for (int i = 0; i < bulletCount; i++)
	{
		bullets[i]->Update();
	}
}