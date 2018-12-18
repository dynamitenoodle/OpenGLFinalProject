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
private:
    Mesh* mesh;        
    Material* material;
    
	glm::mat4 worldMatrix;
	GLFWwindow* window;

	bool isPlayer;
	float speed;
	bool reflectX;
	bool reflectY;

	int* scorePlayer;
	int* scorePlayer2;
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
		GLFWwindow* window,
		bool isPlayer
    );

	//TODO - maybe this should be in a transform class?
	glm::vec3 position;
	glm::vec3 eulerAngles;
	glm::vec3 scale;
	glm::vec4 color;

    /// <summary>
    /// Destruction
    /// </summary>
    virtual ~GameEntity();

    /// <summary>
    /// Updates the worldMatrix
    /// </summary>
    virtual void Update();

	/// <summary>
	/// Checks to see if we should move
	/// </summary>
	void InputCheck();

	/// <summary>
	/// Moves the pong ball
	/// </summary>
	void BallMove();

	void PointLineCollideCheck(GameEntity * entity);

	/// <summary>
	/// Renders the gameEntity based on a camera
	/// </summary>
	void Render(Camera* camera);

	/// <summary>
	/// Changes the color of the game entity
	/// </summary>
	void ChangeColor(float r, float g, float b, float a);

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
	float GetDistance(float xPos, float yPos, float zPos);

	/// <summary>
	/// Checking Collision with other GameObjects
	/// </summary>
	void CollideCheck(GameEntity* entity);

	/// <summary>
	/// Seek the ball y position
	/// </summary>
	void TrackBall(GameEntity * ball);

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

	/// <summary>
	/// Set Score Pointers
	/// </summary>
	void SetScores(int* score1, int* score2);
};

