#include "stdafx.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "GameEntity.h"
#include "Material.h"
#include "Input.h"
#include <stdlib.h>
#include <time.h>  
#include "Star.h"
#include "Player.h"

int main()
{
    {
		// get a random seed
		srand(time(NULL));

		//init GLFW
        {
            if (glfwInit() == GLFW_FALSE)
            {
#ifdef _DEBUG
                std::cout << "GLFW failed to initialize" << std::endl;
                _CrtDumpMemoryLeaks();
                std::cin.get();
#endif
                return 1;
            }
        }
#ifdef _DEBUG
        std::cout << "GLFW successfully initialized!" << std::endl;
#endif // _DEBUG

        //create & init window, set viewport
        int width = 700;
        int height = 800;
        GLFWwindow* window = glfwCreateWindow(width, height, "Babby's First Cube?", nullptr, nullptr);
        {
            if (window == nullptr)
            {
#ifdef _DEBUG
                std::cout << "GLFW failed to create window" << std::endl;
                _CrtDumpMemoryLeaks();
                std::cin.get();
#endif
                glfwTerminate();
                return 1;
            }

            //tells OpenGL to use this window for this thread
            //(this would be more important for multi-threaded apps)
            glfwMakeContextCurrent(window);

            //gets the width & height of the window and specify it to the viewport
            int windowWidth, windowHeight;
            glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
            glViewport(0, 0, windowWidth, windowHeight);
        }
#ifdef _DEBUG
        std::cout << "Window successfully initialized!" << std::endl;
#endif // _DEBUG

        //init GLEW
        {
            if (glewInit() != GLEW_OK)
            {
#ifdef _DEBUG
                std::cout << "GLEW failed to initialize" << std::endl;
                _CrtDumpMemoryLeaks();
                std::cin.get();
#endif
                glfwTerminate();
                return 1;
            }
        }
#ifdef _DEBUG
        std::cout << "GLEW successfully initialized!" << std::endl;
#endif // _DEBUG

        //init the shader program
        //TODO - this seems like a better job for a shader manager
        //       perhaps the Shader class can be refactored to fit a shader program
        //       rather than be a thing for vs and fs
        GLuint shaderProgram = glCreateProgram();
        {
            //create vS and attach to shader program
            Shader *vs = new Shader();
            vs->InitFromFile("assets/shaders/vertexShader.glsl", GL_VERTEX_SHADER);
			
            glAttachShader(shaderProgram, vs->GetShaderLoc());

            //create FS and attach to shader program
            Shader *fs = new Shader();
            fs->InitFromFile("assets/shaders/fragmentShader.glsl", GL_FRAGMENT_SHADER);
            glAttachShader(shaderProgram, fs->GetShaderLoc());

            //link everything that's attached together
            glLinkProgram(shaderProgram);

            GLint isLinked;
            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isLinked);
            if (!isLinked)
            {
                char infolog[1024];
                glGetProgramInfoLog(shaderProgram, 1024, NULL, infolog);
#ifdef _DEBUG
                std::cout << "Shader Program linking failed with error: " << infolog << std::endl;
                std::cin.get();
#endif

                // Delete the shader, and set the index to zero so that this object knows it doesn't have a shader.
                glDeleteProgram(shaderProgram);
                glfwTerminate();
                _CrtDumpMemoryLeaks();
                return 1;
            }

            //everything's in the program, we don't need this
            delete fs;
            delete vs;
        }

#ifdef _DEBUG
        std::cout << "Shaders compiled attached, and linked!" << std::endl;
#endif // _DEBUG


		// Sound Engine
		irrklang::ISoundEngine* soundEngine = irrklang::createIrrKlangDevice();
		soundEngine->setSoundVolume(0.3f);
		soundEngine->play2D("assets/audio/background.mp3", GL_TRUE);

		// square vertices
		GLfloat squareVertices[] = {
			-1.0f, -1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f
		};

		// triangle vertices
		GLfloat triangleVertices[] = {
			-1.0f, -1.0f, 1.0f,
			0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f
		};

        //create our mesh & material
		Material* material = new Material(shaderProgram);

		Mesh* squareMesh = new Mesh();
		squareMesh->InitWithVertexArray(squareVertices, _countof(squareVertices), shaderProgram);

		Mesh* triangleMesh = new Mesh();
		triangleMesh->InitWithVertexArray(triangleVertices, _countof(triangleVertices), shaderProgram);

		GameEntity** myGameEntities = new GameEntity*[25];
		Star** starEntities = new Star*[200];
		Player* playerEntity;
		int entityCount = 0;
		int starCount = 0;

		/*Creating the Game Objects*/
		{
			// Creating the player
			playerEntity = new Player(
				squareMesh,
				material,
				glm::vec3(0, -2.8f, 0), 
				glm::vec3(0, 0, 0), 
				glm::vec3(0.15f, 0.15f, 1.0f),
				window,
				soundEngine
			);
			playerEntity->NewColor(0.5f, 0.5f, 1.0f, 1.f);

			// Test Enemies
			
			for (int i = -1; i < 2; i++) 
			{
				GLfloat x = i * 1.0f;
				myGameEntities[entityCount] = new GameEntity(
					triangleMesh,
					material,
					glm::vec3(x, 4.0f, 0),
					glm::vec3(0.0f, 0.0f, 3.15f),
					glm::vec3(0.15f, 0.15f, 1.0f),
					window
				);
				myGameEntities[entityCount]->NewColor((rand() % 255) / 255.0f, (rand() % 255) / 255.0f, (rand() % 255) / 255.0f, 1.f);
				myGameEntities[entityCount]->seekingPos = glm::vec3(x, 2.0f, 0);
				myGameEntities[entityCount]->force = 0.01f;
				myGameEntities[entityCount]->maxVel = 0.03f;
				entityCount++;
			}
		}
		playerEntity->enemies = myGameEntities;
		playerEntity->enemyCount = &entityCount;

		/* Background effects */
		{
			for (int i = 0; i < 200; i++)
			{
				GLfloat xPos = (rand() % 690) / 100.0f - 3.45f;
				GLfloat yPos = (rand() % 790) / 100.0f - 3.95f;

				starEntities[starCount] = new Star(
					squareMesh,
					material,
					glm::vec3(xPos, yPos, 0),
					glm::vec3(0, 0, 0),
					glm::vec3(0.02f, 0.02f, 1.0f),
					window
				);
				starEntities[starCount]->NewColor((rand() % 100) / 100.0f, (rand() % 100) / 100.0f, (rand() % 100) / 100.0f, 1.f);
				starCount++;
			}
		}

        //TODO - maybe a CameraManager?
        Camera* myCamera = new Camera(
            glm::vec3(0.0f, 0.0f, -5.f),    //position of camera
            glm::vec3(0.0f, 0.0f, 1.f),     //the 'forward' of the camera
            glm::vec3(0.0f, 1.f, 0.0f),     //what 'up' is for the camera
            60.0f,                          //the field of view in radians
            (float)width,                   //the width of the window in float
            (float)height,                  //the height of the window in float
            0.01f,                          //the near Z-plane
            100.f,                           //the far Z-plane
			window
        );

        Input::GetInstance()->Init(window);

        glEnable(GL_DEPTH_TEST);

		// Enable blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// enable culling
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);

        glDepthFunc(GL_LESS);

        //main loop
		while (!glfwWindowShouldClose(window))
        {
            /* INPUT */
            {
                //checks events to see if there are pending input
                glfwPollEvents();

				//breaks out of the loop if user presses ESC
				if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				{
					break;
				}

            }

            /* GAMEPLAY UPDATE */
			playerEntity->Update();

			int toDelete = -1;
			for (int i = 0; i < entityCount; i++)
			{
				myGameEntities[i]->UpdateTransform();
				if (myGameEntities[i]->isDead) 
					toDelete = i;
			}

			// Checks to see if a bullet needs to delete
			if (toDelete > -1)
			{
				soundEngine->play2D("assets/audio/enemyDeath.wav", GL_FALSE);
				delete myGameEntities[toDelete];
				for (int i = toDelete; i < entityCount - 1; i++)
					myGameEntities[i] = myGameEntities[i + 1];

				entityCount--;
			}

			for (int i = 0; i < starCount; i++)
			{
				starEntities[i]->Update();
			}

            myCamera->Update();

            /* PRE-RENDER */
            {
                //start off with clearing the 'color buffer'
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                //clear the window to have p a s t e l o r a n g e
                glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            }

            /* RENDER */
			playerEntity->Render(myCamera);
			for (int i = 0; i < entityCount; i++)
			{
				if (!myGameEntities[i]->isDead)
					myGameEntities[i]->Render(myCamera);
			}

			for (int i = 0; i < starCount; i++)
			{
				starEntities[i]->Render(myCamera);
			}

            /* POST-RENDER */
            {
                //'clear' for next draw call
                glBindVertexArray(0);
                glUseProgram(0);
                //swaps the front buffer with the back buffer
                glfwSwapBuffers(window);
            }
        }

        //de-allocate our mesh!
		delete triangleMesh;
		delete squareMesh;
        delete material;
		delete playerEntity;
		for (int i = 0; i < entityCount; i++)
		{
			delete myGameEntities[i];
		}
		for (int i = 0; i < starCount; i++)
		{
			delete starEntities[i];
		}
        delete myCamera;
        Input::Release();
    }

    //clean up
    glfwTerminate();
#ifdef _DEBUG
    _CrtDumpMemoryLeaks();
#endif // _DEBUG
    return 0;
}