#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Input.h"

Camera::Camera(glm::vec3 position,
    glm::vec3 forward, 
    glm::vec3 up, 
    float fovAngleY, 
    float width, 
    float height,
    float nearZ, 
    float farZ,
	GLFWwindow* window)
{
    this->position = position;
    this->forward = forward; 
	this->forwardOrg = forward;
    this->up = up;

    //does this calculation once
    this->fov = fovAngleY * glm::pi<float>() / 180.0f;
    this->width = width;
    this->height = height;
    this->nearZ = nearZ;
    this->farZ = farZ;
	this->window = window;

	// setting default for input
	this->xPos = new double();
	this->yPos = new double();
	this->xPosOrg = new double();
	this->yPosOrg = new double();
	this->press = false;

	this->pitch = 0;
	this->yaw = 90;
}

Camera::~Camera()
{
	delete xPos;
	delete xPosOrg;
	delete yPos;
	delete yPosOrg;
}

//TODO - incorporate deltaTime
//TODO - maybe separate this function should instead call two private functions
//       (something like UpdateInput() and UpdateMatrices())
void Camera::Update()
{
	/*
    //TODO - cache the pointer to the Input single instance instead of calling 
    //       GetInstance() multiple times
    if (Input::GetInstance()->IsKeyDown(GLFW_KEY_A))
    {
        //TODO - change this to move along local axes instead of global axes
        position.x -= 0.01f;    
    } 
     
    //TODO - cache the pointer to the Input single instance instead of calling 
    //       GetInstance() multiple times
    else if (Input::GetInstance()->IsKeyDown(GLFW_KEY_D))
    {
        //TODO - change this to move along local axes instead of global axes
        position.x += 0.01f;
    } 
	*/

    //this call may not be needed every frame
    forward = glm::normalize<3>(forward);   

    //view matrix
    //we use forward here instead of a lookTarget because it's easier to track
    //and conceptualize.
    viewMatrix = glm::lookAtLH(
        position,               //where the camera is
        position + forward,     //where the camera is looking at 
        up                      //what is 'up' for the camera
    );

    //create the projection matrix
    projectionMatrix = glm::perspectiveFovLH<GLfloat>(
        fov,
        width,                  //the width of the window in float
        height,                 //the height of the window in float
        nearZ,                  //the near Z-plane
        farZ                    //the far Z-plane
    );

	/* INPUT */
	/*{
		// Mouse Position and press get
		// if keyDown
		if (glfwGetMouseButton(window, 0) == GLFW_PRESS && !press)
		{
			glfwGetCursorPos(window, xPosOrg, yPosOrg);
			forwardOrg = forward;
		}

		// if keyUp
		if (!glfwGetMouseButton(window, 0) == GLFW_PRESS && press)
		{
			forwardOrg = forward;
		}

		// if Key
		press = glfwGetMouseButton(window, 0) == GLFW_PRESS;
		if (press)
			glfwGetCursorPos(window, xPos, yPos);

		// Camera Input
		{
			float amt = 0.05f;
			float amtX = 0.0f;
			float amtY = 0.0f;
			float amtZ = 0.0f;

			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			{
				amtX += amt;
			}

			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			{
				amtX -= amt;
			}

			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			{
				amtZ += amt;

			}

			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			{
				amtZ -= amt;
			}

			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			{
				amtY += amt;
			}

			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			{
				amtY -= amt;
			}

			// set the camera position and the forward direction
			this->Move(amtX, amtY, amtZ);


			if (press) 
			{
				float xOffset = *xPos - *xPosOrg;
				float yOffset = *yPos - *yPosOrg; // reversed since y-coordinates range from bottom to top
				glfwGetCursorPos(window, xPosOrg, yPosOrg);

				float sensitivity = 0.05f;
				xOffset *= sensitivity;
				yOffset *= sensitivity;

				yaw += xOffset;
				pitch += yOffset;
				if (pitch > 89.0f)
					pitch = 89.0f;
				if (pitch < -89.0f)
					pitch = -89.0f;

				glm::vec3 front = forward;
				front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
				front.y = sin(glm::radians(pitch));
				front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
				forward = glm::normalize(front);
			}
		}
	}*/


	// set a temp vec
	/*
	glm::vec3 forwTemp = forwardOrg;
	if (press)
	{
	// if we are pressed, changed the position accordingly
	forwTemp.x -= ((*xPosOrg - *xPos) / 1000.f);
	forwTemp.y -= (*yPosOrg - *yPos) / 1000.f;
	}


	// Setting the forward
	forward = forwTemp;

	forward = glm::normalize<3>(forward);
	*/
}

void Camera::Move(float amtX, float amtY, float amtZ)
{
	position += forward * amtZ;
	position += up * amtY;
	position += glm::normalize(glm::cross(up, forward)) * amtX;
}
