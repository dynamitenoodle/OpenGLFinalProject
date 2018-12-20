#include "Player.h"
#include "glm/gtc/matrix_transform.hpp"
#include "stdafx.h"
#include <stdlib.h>

Player::Player(Mesh * mesh,
	Material * material,
	glm::vec3 position,
	glm::vec3 eulerAngles,
	glm::vec3 scale,
	GLFWwindow* window,
	irrklang::ISoundEngine* soundEngine)
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
	this->hitTimer = 60;
	this->isHit = true;

	// Bullets
	this->bullets = new GameEntity*[5];
	this->bulletCount = 0;
	this->spaceUp = true;
	this->soundEngine = soundEngine;

	// super collisions
	this->bigColArea = new bool[4];
	this->medColArea = new bool[16];
	this->smlColArea = new bool[64];
}	


Player::~Player()
{
}

void Player::Update()
{
	this->BulletsUpdate();
	this->InputCheck();

	this->UpdateTransform();

	for (int i = 0; i < *this->enemyCount; i++)
	{
		if (this->CollideCheck(enemies[i]) && !this->isHit)
		{
			enemies[i]->playDeathSound = true;
			enemies[i]->isDead = true;
			this->isHit = true;
			this->hitTimer = 60;
		}
	}
	/*
	Making sure the numbers were right, they were and i debugged for 30 minutes when they were still right >:((((((
	std::cout << " BIG " << std::endl;
	for (int i = 0; i < 4; i++)
	{
		if(this->bigColArea[i])
			std::cout << i << " : " << bigColArea[i] << std::endl;
	}
	std::cout << " MED " << std::endl;
	for (int i = 0; i < 16; i++)
	{
		if(this->medColArea[i])
			std::cout << i << " : " << medColArea[i] << std::endl;
	}
	std::cout << " SML " << std::endl;
	for (int i = 0; i < 64; i++)
	{
		if (this->smlColArea[i])
			std::cout << i << " : " << smlColArea[i] << std::endl;
	}
	*/
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
		this->velocity *= .9f;
		this->seekingPos = this->position;
	}

	if (this->position.x > width)
		this->position.x = width;
	if (this->position.x < -width)
		this->position.x = -width;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && bulletCount < 5 && spaceUp && !this->isHit)
	{
		spaceUp = false;
		GenerateBullet();
	}

	else if (!glfwGetKey(window, GLFW_KEY_SPACE))
		spaceUp = true;
}

void Player::BulletsUpdate()
{
	int toDelete = -1;
	for (int i = 0; i < bulletCount; i++)
	{
		if (!bullets[i]->isDead)
		{
			bullets[i]->UpdateTransform();
			if (bullets[i]->position.y > 4.1f)
			{
				bullets[i]->isDead = true;
				toDelete = i;
			}

			for (int j = 0; j < *enemyCount; j++)
			{
				if (bullets[i]->CollideCheck(enemies[j]))
				{
					enemies[j]->playDeathSound = true;
					this->isDead = true;
					enemies[j]->isDead = true;
					toDelete = i;
				}
			}
		}
	}

	if (toDelete > -1) 
	{
		delete bullets[toDelete];
		// Checks to see if a bullet needs to delete
		for (int i = toDelete; i < bulletCount; i++)
		{
			while (i < bulletCount)
			{
				bullets[i] = bullets[i + 1];
				i++;
			}
		}
		bulletCount--;
	}
}

void Player::GenerateBullet()
{
	// plays sound
	soundEngine->play2D("assets/audio/shoot.wav", GL_FALSE);

	// creates the bullet
	bullets[bulletCount] = new GameEntity(
		mesh,
		material,
		position,
		glm::vec3(0, 0, 0),
		glm::vec3(0.03f, 0.05f, 1.0f),
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
	if (this->hitTimer > 0)
		hitTimer--;
	else
	{
		hitTimer = 0;
		this->isHit = false;
	}
	
	if ((!this->isHit && this->hitTimer == 0) || this->hitTimer % 2 == 1)
		this->ChangeColor(this->color.x, this->color.y, this->color.z, this->color.a);
	else
		this->ChangeColor(this->color.x, this->color.y, this->color.z, 0.0f);

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