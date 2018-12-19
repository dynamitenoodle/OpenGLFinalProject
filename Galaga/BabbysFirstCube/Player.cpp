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

	this->force = 0.005f;
	this->maxVel = 0.05f;

	// Bullets
	this->bullets = new GameEntity*[5];
	this->bulletCount = 0;
	this->spaceUp = true;
}	


Player::~Player()
{
}

void Player::Update()
{
	this->BulletsUpdate();
	this->InputCheck();

	this->UpdateTransform();
}

void Player::InputCheck()
{
	float width = 2.85f;
	bool seekingCheck = true;

	// input right
	if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) && this->position.x < width)
	{
		this->seekingPos = this->position;
		this->seekingPos.x = this->position.x + 1.0f;
		seekingCheck = false;
	}

	// input left
	if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) && this->position.x > -width)
	{
		this->seekingPos = this->position;
		this->seekingPos.x = this->position.x - 1.0f;
		seekingCheck = false;
	}
	
	// if we need to STOP
	if (seekingCheck)
	{
		this->seekingPos = this->position;
		this->velocity *= .94f;
	}

	if (this->position.x > width)
		this->position.x = width;
	if (this->position.x < -width)
		this->position.x = -width;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && bulletCount < 5 && spaceUp)
	{
		spaceUp = false;
		GenerateBullet();
	}

	else if (!glfwGetKey(window, GLFW_KEY_SPACE))
		spaceUp = true;
}

void Player::BulletsUpdate()
{
	bool moveBullets = false;
	for (int i = 0; i < bulletCount; i++)
	{
		if (!bullets[i]->isDead)
		{
			bullets[i]->UpdateTransform();
			if (bullets[i]->position.y > 4.1f)
			{
				bullets[i]->isDead = true;
			}

			for (int j = 0; j < *enemyCount; j++)
			{
				bullets[i]->CollideCheck(enemies[j]);
			}
		}
		else
			moveBullets = true;
	}

	// Checks to see if a bullet needs to delete
	if (moveBullets) 
	{
		delete bullets[0];
		for (int i = 0; i < bulletCount - 1; i++)
			bullets[i] = bullets[i + 1];

		bulletCount--;
	}
}

void Player::GenerateBullet()
{
	bullets[bulletCount] = new GameEntity(
		mesh,
		material,
		position,
		glm::vec3(0, 0, 0),
		glm::vec3(0.05f, 0.05f, 1.0f),
		window
	);
	bullets[bulletCount]->force = .1f;
	bullets[bulletCount]->maxVel = .1f;
	bullets[bulletCount]->seekingPos = this->position;
	bullets[bulletCount]->seekingPos.y = 4.5f;
	bullets[bulletCount]->NewColor(1.0f, 1.0f, 1.0f, 1.f);
	bullets[bulletCount]->UpdateTransform();
	bulletCount++;
}

void Player::Render(Camera* camera)
{
	// setting the color
	this->ChangeColor(this->color.x, this->color.y, this->color.z, this->color.a);
	// bind the material
	material->Bind(camera, worldMatrix);
	mesh->Render();
	BulletsRender(camera);
}

void Player::BulletsRender(Camera* camera)
{
	for (int i = 0; i < bulletCount; i++)
	{
		bullets[i]->Render(camera);
	}
}