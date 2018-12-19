#pragma once
#include "stdafx.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"

/// <summary>
/// Represents one 'renderable' objet
/// </summary>
class GameEntity
{
protected:
    Mesh* mesh;        
    Material* material;
    
	glm::mat4 worldMatrix;
	GLFWwindow* window;

	/// <summary>
	/// Changes the color of the game entity
	/// </summary>
	void ChangeColor(float r, float g, float b, float a);
public: 
    /// <summary>
    /// Basic paramterized constructor for most of our private vars
    /// </summary>
	GameEntity(
		Mesh* mesh,
		Material* material,
		glm::vec3 position,
		glm::vec3 eulerAngles,
		glm::vec3 scale,
		GLFWwindow* window
	);

	/// <summary>
	/// Default for inheritance
	/// </summary>
	GameEntity();

	glm::vec3 acceleration;
	glm::vec3 velocity;
	glm::vec3 position;
	glm::vec3 eulerAngles;
	glm::vec3 scale;
	glm::vec4 color;
	float force;
	float maxVel;
	bool isDead;
	glm::vec3 seekingPos;

    /// <summary>
    /// Destruction
    /// </summary>
    virtual ~GameEntity();

    /// <summary>
    /// Updates the worldMatrix
    /// </summary>
    virtual void UpdateTransform();

	/// <summary>
	/// Renders the gameEntity based on a camera
	/// </summary>
	void Render(Camera* camera);

	/// <summary>
	/// Sets the color to a specific color
	/// </summary>
	void NewColor(float r, float g, float b, float a);

	/// <summary>
	/// Sets the Draw style of the Mesh
	/// </summary>
	void SetDraw(bool set);

	/// <summary>
	/// Returns the distance between a point and the game entity position using MATH
	/// </summary>
	float GetDistance(float xPos, float yPos);

	/// <summary>
	/// Checking Collision with other GameObjects
	/// </summary>
	void CollideCheck(GameEntity* entity);

	/// <summary>
	/// Checking Right
	/// </summary>
	bool RightCheck(GameEntity* entity);

	/// <summary>
	/// Checking Left
	/// </summary>
	bool LeftCheck(GameEntity* entity);

	/// <summary>
	/// Checking Up
	/// </summary>
	bool UpCheck(GameEntity* entity);

	/// <summary>
	/// Checking Down
	/// </summary>
	bool DownCheck(GameEntity* entity);
};

