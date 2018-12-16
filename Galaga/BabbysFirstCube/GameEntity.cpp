#include "GameEntity.h"
#include "glm/gtc/matrix_transform.hpp"
#include <stdlib.h>
#include <time.h>  

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

	if (scale.y < 0.25f)
		speed = 0.05f;
	else
		speed = 0.08f;

	reflectX = false;
	reflectY = false;
}

GameEntity::GameEntity()
{
}

GameEntity::~GameEntity()
{
}

void GameEntity::Update()
{
	worldMatrix = glm::translate(
		glm::identity<glm::mat4>(),
		this->position
	);

	worldMatrix = glm::rotate(
		worldMatrix,
		this->eulerAngles.y,
		glm::vec3(0.f, 1.f, 0.f)
	);

	worldMatrix = glm::rotate(
		worldMatrix,
		this->eulerAngles.x,
		glm::vec3(1.f, 0.f, 0.f)
	);

	worldMatrix = glm::rotate(
		worldMatrix,
		this->eulerAngles.z,
		glm::vec3(0.f, 0.f, 1.f)
	);

	worldMatrix = glm::scale(
		worldMatrix,
		this->scale
	);
}

void GameEntity::CollideCheck(GameEntity* entity)
{
	// Collision Check
	if (RightCheck(entity) && LeftCheck(entity) && UpCheck(entity) && DownCheck(entity))
	{
		// BOT CHECK
		if (entity->position.y - (entity->scale.y * .98f) > this->position.y)
		{
			this->position.y = entity->position.y - (entity->scale.y * 1.2f);
			reflectY = !reflectY;
		}

		// TOP CHECK
		else if (entity->position.y + (entity->scale.y * .98f) < this->position.y)
		{
			this->position.y = entity->position.y + (entity->scale.y * 1.2f);
			reflectY = !reflectY;
		}

		reflectX = !reflectX;
	}
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

float GameEntity::GetDistance(float xPos, float yPos, float zPos)
{
	return glm::sqrt((xPos - this->position.x) * (xPos - this->position.x)) + ((yPos - this->position.y) * (yPos - this->position.y) + (zPos - this->position.z) * (zPos - this->position.z));
}

void GameEntity::SetDraw(bool set)
{
	mesh->SetDraw(set);
}