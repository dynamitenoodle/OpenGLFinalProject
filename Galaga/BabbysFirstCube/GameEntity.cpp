#define GLM_ENABLE_EXPERIMENTAL
#include "GameEntity.h"
#include "glm/gtc/matrix_transform.hpp"
#include <stdlib.h>
#include <time.h>  
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>

GameEntity::GameEntity(Mesh * mesh,
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
	worldMatrix = glm::identity<glm::mat4>();

	this->color.x = 0.0f;
	this->color.y = 0.0f;
	this->color.z = 0.0f;

	this->window = window;

	force = 0;
	maxVel = 0;
	seekingPos = glm::vec3(0, 0, 0);
	playDeathSound = false;

	isDead = false;
	bigColArea = new bool[4];
	medColArea = new bool[16];
	smlColArea = new bool[64];
}

GameEntity::GameEntity()
{
}

GameEntity::~GameEntity()
{
}

void GameEntity::UpdateTransform()
{
	// makes sure we still need to seek
	if (GetDistance(this->seekingPos.x, this->seekingPos.y) > .005f && this->maxVel != 0.05f)
	{
		// accel
		this->acceleration.x = this->seekingPos.x - this->position.x;
		this->acceleration.y = this->seekingPos.y - this->position.y;
		float num = glm::sqrt((this->acceleration.x * this->acceleration.x) + (this->acceleration.y * this->acceleration.y));

		// if we aren't not movin
		if (num != 0)
		{
			this->acceleration.x /= num;
			this->acceleration.y /= num;
			this->acceleration *= this->force;
		}

		// vel
		this->velocity += this->acceleration;

		// clamp
		if (glm::sqrt((this->velocity.x * this->velocity.x) + (this->velocity.y * this->velocity.y)) > this->maxVel)
		{
			float num2 = glm::sqrt((this->velocity.x * this->velocity.x) + (this->velocity.y * this->velocity.y));
			this->velocity.x /= num2;
			this->velocity.y /= num2;
			this->velocity *= this->maxVel;
		}

		//set position
		this->position += this->velocity;
	}
	else if (this->maxVel == 0.05f)
	{
		// accel
		this->acceleration.x = this->seekingPos.x - this->position.x;
		this->acceleration.y = this->seekingPos.y - this->position.y;
		float num = glm::sqrt((this->acceleration.x * this->acceleration.x) + (this->acceleration.y * this->acceleration.y));

		// if we aren't not movin
		if (num != 0)
		{
			this->acceleration.x /= num;
			this->acceleration.y /= num;
			this->acceleration *= this->force;
		}

		// vel
		this->velocity += this->acceleration;

		// clamp
		if (glm::sqrt((this->velocity.x * this->velocity.x) + (this->velocity.y * this->velocity.y)) > this->maxVel)
		{
			float num2 = glm::sqrt((this->velocity.x * this->velocity.x) + (this->velocity.y * this->velocity.y));
			this->velocity.x /= num2;
			this->velocity.y /= num2;
			this->velocity *= this->maxVel;
		}

		//set position
		this->position += this->velocity;
	}
	else
		this->velocity *= .96f;

	// setting where in the world it is (BSP)
	this->BSPSet(3.6f, 4.1f, 0, 0, 0, 0); 

	// World Matrix stuff
	{
		worldMatrix = glm::translate(
			glm::identity<glm::mat4>(),
			this->position
		);

		// Conversion from Euler angles (in radians) to Quaternion, times world matrix
		worldMatrix *= glm::toMat4(glm::quat(this->eulerAngles));

		worldMatrix = glm::scale(
			worldMatrix,
			this->scale
		);
	}
}

bool GameEntity::CollideCheck(GameEntity* entity)
{
	// Collision Checking!
	for (int i = 0; i < 4; i++) 
	{
		// Checking Big Space
		if (this->bigColArea[i] && this->bigColArea[i] == entity->bigColArea[i])
		{
			for (int j = 0; j < 16; j++)
			{
				// Checking Med Space
				if (this->medColArea[j] && this->medColArea[j] == entity->medColArea[j])
				{
					for (int k = 0; k < 64; k++) 
					{
						// Checking Small Space
						if (this->smlColArea[k] && this->smlColArea[k] == entity->smlColArea[k])
						{
							// Do an AABB check just to be sure we are hitting!
							if (this->RightCheck(entity) && this->LeftCheck(entity) && this->UpCheck(entity) && this->DownCheck(entity))
							{
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}

bool GameEntity::RightCheck(GameEntity* entity) { return this->position.x + this->scale.x >= entity->position.x - entity->scale.x; }

bool GameEntity::LeftCheck(GameEntity* entity) { return this->position.x - this->scale.x <= entity->position.x + entity->scale.x; }

bool GameEntity::UpCheck(GameEntity* entity) { return this->position.y + this->scale.y >= entity->position.y - entity->scale.y; }

bool GameEntity::DownCheck(GameEntity* entity) { return this->position.y - this->scale.y <= entity->position.y + entity->scale.y; }

void GameEntity::Render(Camera* camera)
{
	// setting the color
	this->ChangeColor(this->color.x, this->color.y, this->color.z, this->color.a);
	// bind the material
	material->Bind(camera, worldMatrix);
    mesh->Render();
}

void GameEntity::ChangeColor(float r, float g, float b, float a)
{
	material->color.x = r;
	material->color.y = g;
	material->color.z = b;
	material->color.a = a;
}

void GameEntity::NewColor(float r, float g, float b, float a)
{
	color.x = r;
	color.y = g;
	color.z = b;
	color.a = a;
}

float GameEntity::GetDistance(float xPos, float yPos)
{
	return glm::sqrt((xPos - this->position.x) * (xPos - this->position.x)) + ((yPos - this->position.y) * (yPos - this->position.y));
}

void GameEntity::SetDraw(bool set)
{
	mesh->SetDraw(set);
}

void GameEntity::BSPSet(float width, float height, float middleX, float middleY, int num, int depth)
{
	GLfloat right = this->position.x + this->scale.x;
	GLfloat left = this->position.x - this->scale.x;
	GLfloat top = this->position.y + this->scale.y;
	GLfloat bottom = this->position.y - this->scale.y;

	bool topRight = false;
	bool topLeft = false;
	bool botRight = false;
	bool botLeft = false;

	// in the top two areas
	if (bottom > middleY && bottom < middleY + height)
	{
		// top right
		if (left > middleX && left < middleX + width)
		{
			topRight = true;
		}
		// top left
		else if (right < middleX && right > middleX - width)
		{
			topLeft = true;
		}
		// both!
		else if (left > middleX - width && right < middleX + width)
		{
			topRight = true;
			topLeft = true;
		}
	}

	// in the bottom two areas
	else if (top < middleY && top > middleY - height)
	{
		// bot right
		if (left > middleX && left < middleX + width)
		{
			botRight = true;
		}
		// vot left
		else if (right < middleX && right > middleX - width)
		{
			botLeft = true;
		}
		// both!
		else if (left > middleX - width && right < middleX + width)
		{
			botRight = true;
			botLeft = true;
		}
	}

	// in the BOTH
	else if (bottom > middleY - height && top < middleY + height)
	{
		// right
		if (left > middleX && left < middleX + width)
		{
			topRight = true;
			botRight = true;
		}
		// left
		else if (right < middleX && right > middleX - width)
		{
			topLeft = true;
			botLeft = true;
		}
		// EVERYTHING!
		else if (left > middleX - width && right < middleX + width)
		{
			topRight = true;
			topLeft = true;
			botRight = true;
			botLeft = true;
		}
	}

	if (depth == 0)
	{
		this->bigColArea[0] = topLeft;
		this->bigColArea[1] = topRight;
		this->bigColArea[2] = botLeft;
		this->bigColArea[3] = botRight;

		// reset the medium size boys and small boys
		for (int i = 0; i < 16; i++)
		{
			medColArea[i] = false;
		}

		for (int i = 0; i < 64; i++)
		{
			smlColArea[i] = false;
		}

		if (topLeft)
			BSPSet(width / 2, height / 2, -width / 2, height / 2, 0, depth + 1);

		if (topRight)
			BSPSet(width / 2, height / 2, width / 2, height / 2, 1, depth + 1);

		if (botLeft)
			BSPSet(width / 2, height / 2, -width / 2, -height / 2, 2, depth + 1);

		if (botRight)
			BSPSet(width / 2, height / 2, width / 2, -height / 2, 3, depth + 1);
	}

	else if (depth == 1)
	{
		if (num != 0)
			num *= 4;

		this->medColArea[num] = topLeft;
		this->medColArea[num + 1] = topRight;
		this->medColArea[num + 2] = botLeft;
		this->medColArea[num + 3] = botRight;

		if (topLeft)
			BSPSet(width / 2, height / 2, middleX - (width / 2), middleY + (height / 2), num, depth + 1);
		
		if (topRight)
			BSPSet(width / 2, height / 2, middleX + (width / 2), middleY + (height / 2), num + 1, depth + 1);

		if (botLeft)
			BSPSet(width / 2, height / 2, middleX - (width / 2), middleY - (height / 2), num + 2, depth + 1);

		if (botRight)
			BSPSet(width / 2, height / 2, middleX + (width / 2), middleY - (height / 2), num + 3, depth + 1);
	}

	else if (depth == 2)
	{
		if (num != 0)
			num *= 4;

		this->smlColArea[num] = topLeft;
		this->smlColArea[num + 1] = topRight;
		this->smlColArea[num + 2] = botLeft;
		this->smlColArea[num + 3] = botRight;
	}
}
