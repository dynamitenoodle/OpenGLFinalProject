#pragma once
#include "stdafx.h"

class Camera
{
private:
    glm::vec3 position;     //position of camera
	glm::vec3 forward;      //where the camera is facing
	glm::vec3 forwardOrg;   //where the camera is facing originally
    glm::vec3 up;           //what 'up' is for the camera

    float fov;      //field of view
    float width;    //width of screen
    float height;   //height of screen
    float nearZ;    //near Z-plane
    float farZ;     //far  Z-plane

    glm::mat4 viewMatrix;       //cached view matrix
    glm::mat4 projectionMatrix; //cached projection matrix
	GLFWwindow* window;

	double* xPos;
	double* yPos;
	double* xPosOrg;
	double* yPosOrg;
	bool press;

	float pitch;
	float yaw;
public:

    /// <summary>
    /// Constructor where you supply everything you need for the 2 matrices
    /// </summary>
    Camera(
        glm::vec3 position,
        glm::vec3 forward,
        glm::vec3 up,
        float fovAngleY,
        float width,
        float height,
        float nearZ,
        float farZ,
		GLFWwindow* window
    );

    /// <summary>
    /// Destruction
    /// </summary>
    ~Camera();

    /// <summary>
    /// Queries input and updates the matrices of the camera
    /// </summary>
    void Update();

    /// <summary>
    /// Gets the view matrix
    /// </summary>
    glm::mat4 GetView() const { return viewMatrix; }
    
    /// <summary>
    /// Gets the projection matrix
    /// </summary>
    glm::mat4 GetProjection() const { return projectionMatrix; }

	/// <summary>
	/// Gets the forward
	/// </summary>
	void Move(float amtX, float amtY, float amtZ);
};

