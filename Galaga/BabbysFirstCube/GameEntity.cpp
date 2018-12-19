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

	isDead = false;
}

GameEntity::GameEntity()
{
}

GameEntity::~GameEntity()
{
}

void GameEntity::UpdateTransform()
{
	// accel
	if (GetDistance(this->seekingPos.x, this->seekingPos.y) > .01f)
	{
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
			std::cout << "HIT" << std::endl;
			float num2 = glm::sqrt((this->velocity.x * this->velocity.x) + (this->velocity.y * this->velocity.y));
			this->velocity.x /= num2;
			this->velocity.y /= num2;
			this->velocity *= this->maxVel;
		}

		//set position
		this->position += this->velocity;
	}
	else
		this->velocity *= .95f;

	worldMatrix = glm::translate(
		glm::identity<glm::mat4>(),
		this->position
	);

	// Conversion from Euler angles (in radians) to Quaternion, times world matrix
	worldMatrix *= glm::toMat4(glm::quat(this->eulerAngles));

	/* Old Rotations 
	worldMatrix = glm::rotate(
		worldMatrix,
		this->eulerAngles.x,
		glm::vec3(1.f, 0.f, 0.f)
	);

	worldMatrix = glm::rotate(
		worldMatrix,
		this->eulerAngles.y,
		glm::vec3(0.f, 1.f, 0.f)
	);

	worldMatrix = glm::rotate(
		worldMatrix,
		this->eulerAngles.z,
		glm::vec3(0.f, 0.f, 1.f)
	);
	*/

	worldMatrix = glm::scale(
		worldMatrix,
		this->scale
	);
}

void GameEntity::CollideCheck(GameEntity* entity)
{
	// Collision Check
	if (this->RightCheck(entity) && this->LeftCheck(entity) && this->UpCheck(entity) && this->DownCheck(entity))
	{
		this->isDead = true;
		entity->isDead = true;
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

float GameEntity::GetDistance(float xPos, float yPos)
{
	return glm::sqrt((xPos - this->position.x) * (xPos - this->position.x)) + ((yPos - this->position.y) * (yPos - this->position.y));
}

void GameEntity::SetDraw(bool set)
{
	mesh->SetDraw(set);
}