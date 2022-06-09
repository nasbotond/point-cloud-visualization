#pragma once

// C++ includes
#include <memory>
#include <array>
#include <iostream>
#include <fstream>
#include <sstream>

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "includes/gCamera.h"
#include "includes/ProgramObject.h"
#include "includes/BufferObject.h"
#include "includes/VertexArrayObject.h"
#include "includes/TextureObject.h"

// mesh
#include "includes/ObjParser_OGL3.h"

class CMyApp
{
public:
	CMyApp(void);
	~CMyApp(void);

	bool Init();
	void Clean();

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);

protected:
	// init functions for better readability
	void InitSkyBox();

	// data parser method
	void ReadPointData();

	// variables for shaders
	ProgramObject		m_program;			// shader programs
	ProgramObject		m_programPointNormal; // point normal visualization
	ProgramObject		m_programRectangle; // small rectangles perpendicular to normal vector
	ProgramObject		m_programAxes;		
	ProgramObject		m_programSkybox;	// skybox shaders

	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 c1;
		glm::vec3 c2;
		glm::vec2 t1;
		glm::vec2 t2;
		glm::vec3 n;
	};

	// data reading variables
	const std::string fileName = "Assets/data.txt";
	std::vector<Vertex> vertices;
	int vertexNum;

	// point geometry
	VertexArrayObject m_Vao;
	ArrayBuffer m_VertexBuffer;

	// simpler inside out cube geometry for the skybox
	VertexArrayObject	m_SkyboxVao;		// VAO
	IndexBuffer			m_SkyboxIndices;	// index buffer
	ArrayBuffer			m_SkyboxPos;		// VBO

	gCamera				m_camera;

	// textures
	TextureCubeMap		m_skyboxTexture;

	Texture2D			m_bearTexture1;
	Texture2D			m_bearTexture2;

	// variables for calculating real time fps - modified in Update()
	double				m_delta_time;

	// variable for determining color or texture of points
	int e = 0;
	int normal_magnitude = 1;
	int l = 1;
	int w = 1;
	int rectColOrTex = 1;

	// imgui variables
	int useBearTexture1 = 0;
	int skyboxBackground = 0;
	bool showPoints = false;
	bool showNormals = false;
	bool showRectangles = false;
};

