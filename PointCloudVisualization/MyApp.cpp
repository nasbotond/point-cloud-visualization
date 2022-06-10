#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <tuple>
#include <imgui/imgui.h>
#include <random>
#include <cmath>

#include "includes/GLUtils.hpp"

#include "glm/ext.hpp"

CMyApp::CMyApp(void)
{
	m_camera.SetView(glm::vec3(.2, -0.5, 5), glm::vec3(0.8, 0.5, -2), glm::vec3(0, 1, 0));
}


CMyApp::~CMyApp(void)
{
	std::cout << "dtor!\n";
}

void CMyApp::InitSkyBox()
{
	//
	// Defining geometry (std::vector<...>) and upload to GPU buffers (m_buffer*) with BufferData
	//

	// Position of vertices:
	/*
	The constructor of m_SkyboxPos has already created a GPU buffer identifier, and the following BufferData call will
	1. bind this to GL_ARRAY_BUFFER (because the type of m_SkyboxPos is ArrayBuffer) and
	2. upload the values of the container given in the argument to the GPU by calling glBufferData
	*/
	m_SkyboxPos.BufferData(
		std::vector<glm::vec3>{
		// back face
		glm::vec3(-1, -1, -1),
		glm::vec3(+1, -1, -1),
		glm::vec3(+1, +1, -1),
		glm::vec3(-1, +1, -1),
		// front face
		glm::vec3(-1, -1, +1),
		glm::vec3(+1, -1, +1),
		glm::vec3(+1, +1, +1),
		glm::vec3(-1, +1, +1),
	});

	// And the indices which the primitives are constructed by (from the arrays defined above) - prepared to draw them as a triangle list
	m_SkyboxIndices.BufferData(
		std::vector<int>{
		// back face
		0, 1, 2,
		2, 3, 0,
		// front face
		4, 6, 5,
		6, 4, 7,
		// left
		0, 3, 4,
		4, 3, 7,
		// right
		1, 5, 2,
		5, 6, 2,
		// bottom
		1, 0, 4,
		1, 4, 5,
		// top
		3, 2, 6,
		3, 6, 7,
	});

	// Registering geometry in VAO
	m_SkyboxVao.Init(
		{
			// Attribute 0 is "practically" an array of glm::vec3 and the data is in the GPU buffer (m_gpuBufferPos)
			{ CreateAttribute<		0,						// Channel 0
									glm::vec3,				// CPU-side data type which is used to define attributes of channel 0 <- the procedure deducts that the attribute 0 is made of 3 floats from the glm::vec3
									0,						// offset: The offset of the attribute, considered from the beginning of the container
									sizeof(glm::vec3)		// stride: This attribute of the next vertex will be this many bytes from the current
								>, m_SkyboxPos },
		},
		m_SkyboxIndices
	);

	// skybox texture
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	m_skyboxTexture.AttachFromFile("Assets/xpos.png", false, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	m_skyboxTexture.AttachFromFile("Assets/xneg.png", false, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	m_skyboxTexture.AttachFromFile("Assets/ypos.png", false, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	m_skyboxTexture.AttachFromFile("Assets/yneg.png", false, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	m_skyboxTexture.AttachFromFile("Assets/zpos.png", false, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	m_skyboxTexture.AttachFromFile("Assets/zneg.png", true, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

	// AttachFromFile sets GL_TEXTURE_MAG_FILTER and GL_TEXTURE_MIN_FILTER, we set the rest
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

bool CMyApp::Init()
{
	// Clear color will be blueish
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	// glEnable(GL_CULL_FACE); // Drop faces looking backwards
	glEnable(GL_DEPTH_TEST); // Enable depth test

	//
	// Loading shaders
	//

	// Create the program containing shaders
	m_program.AttachShaders({
		{ GL_VERTEX_SHADER, "pointCloud.vert"},
		{ GL_FRAGMENT_SHADER, "pointCloud.frag"}
	});

	// Mapping attributes between VAO and shaders
	m_program.BindAttribLocations({
		{ 0, "vs_in_pos" },
		{ 1, "vs_in_c1" },
		{ 2, "vs_in_c2" },
		{ 3, "vs_in_t1" },
		{ 4, "vs_in_t2" },
		{ 5, "vs_in_norm" },
	});

	m_program.LinkProgram();

	// Create the program containing shaders
	m_programPointNormal.AttachShaders({
		{ GL_VERTEX_SHADER, "pointNormal.vert"},
		{ GL_GEOMETRY_SHADER, "pointNormal.geom"},
		{ GL_FRAGMENT_SHADER, "pointNormal.frag"}
		});

	// Mapping attributes between VAO and shaders
	m_programPointNormal.BindAttribLocations({
		{ 0, "vs_in_pos" },
		{ 1, "vs_in_c1" },
		{ 2, "vs_in_c2" },
		{ 3, "vs_in_t1" },
		{ 4, "vs_in_t2" },
		{ 5, "vs_in_norm" },
	});

	m_programPointNormal.LinkProgram();

	// Create the program containing shaders
	m_programRectangle.AttachShaders({
		{ GL_VERTEX_SHADER, "rectangle.vert"},
		{ GL_GEOMETRY_SHADER, "rectangle.geom"},
		{ GL_FRAGMENT_SHADER, "rectangle.frag"}
	});

	// Mapping attributes between VAO and shaders
	m_programRectangle.BindAttribLocations({
		{ 0, "vs_in_pos" },
		{ 1, "vs_in_c1" },
		{ 2, "vs_in_c2" },
		{ 3, "vs_in_t1" },
		{ 4, "vs_in_t2" },
		{ 5, "vs_in_norm" },
	});

	m_programRectangle.LinkProgram();

	// Fast creation of the shader program (the 3 function calls above inside a single call)
	m_programSkybox.Init(
	{
		"skybox.vert"_vs,
		"skybox.frag"_fs
	},
	{
		{ 0, "vs_in_pos" },
	}
	);

	// axes
	m_programAxes.Init(
	{
		"axis.vert"_vs,
		"axis.frag"_fs
	});

	// get vertices from input file
	ReadPointData();
	vertexNum = vertices.size();

	m_VertexBuffer.BufferData(vertices);

	// Registering geometry in VAO
	m_Vao.Init(
		{
			// Attribute 0 is "practically" an array of glm::vec3 and the data is in the GPU buffer (m_CubeVertexBuffer)
			{ CreateAttribute<		0,						// Channel 0
									glm::vec3,				// CPU-side data type which is used to define attributes of channel 0 <- the procedure deducts that the attribute 0 is made of 3 floats from the glm::vec3
									offsetof(Vertex, p),	// offset: The offset of the attribute, considered from the beginning of the container
									sizeof(Vertex)			// stride: This attribute of the next vertex will be this many bytes from the current
								>, m_VertexBuffer },
			{ CreateAttribute<1, glm::vec3, offsetof(Vertex, c1), sizeof(Vertex)>, m_VertexBuffer },
			{ CreateAttribute<2, glm::vec3, offsetof(Vertex, c2), sizeof(Vertex)>, m_VertexBuffer },
			{ CreateAttribute<3, glm::vec2, offsetof(Vertex, t1), sizeof(Vertex)>, m_VertexBuffer },
			{ CreateAttribute<4, glm::vec2, offsetof(Vertex, t2), sizeof(Vertex)>, m_VertexBuffer },
			{ CreateAttribute<5, glm::vec3, offsetof(Vertex, n), sizeof(Vertex)>, m_VertexBuffer }
		}
	);

	// init skybox geometry and texture
	InitSkyBox();

	// load texture
	m_bearTexture1.FromFile("Assets/bear1.png");
	m_bearTexture2.FromFile("Assets/bear2.png");

	// Camera
	m_camera.SetProj(glm::radians(60.0f), 640.0f / 480.0f, 0.01f, 1000.0f);

	return true;
}


void CMyApp::Clean()
{

}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	m_delta_time = (SDL_GetTicks() - last_time) / 1000.0f;

	m_camera.Update(static_cast<float>(m_delta_time));

	last_time = SDL_GetTicks();
}

void CMyApp::Render()
{
	// Delete the frame buffer (GL_COLOR_BUFFER_BIT) and the depth (Z) buffer (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_Vao.Bind();

	m_program.Use();

	glm::mat4 model = glm::mat4(1);
	// glm::mat4 view = m_camera.GetViewMatrix();
	// glm::mat4 projection = m_camera.GetProj();
	// glm::vec3 direction = glm::vec3(view[2]);
	glm::vec3 direction = glm::vec3(.2, -0.5, 5); // original position of camera

	// point cloud

	glUniform1i(m_program.GetLocation("colOrTexIndex"), e);
	m_program.SetTexture("texImage", 0, m_bearTexture1);
	m_program.SetUniform("MVP", m_camera.GetViewProj() * model);
	if (showPoints) 
	{
		glDrawArrays(GL_POINTS, 0, vertexNum);
	}

	// point normal visualization

	m_programPointNormal.Use();

	glUniform1i(m_programPointNormal.GetLocation("normal_magnitude"), normal_magnitude);
	m_programPointNormal.SetUniform("direction", direction);
	m_programPointNormal.SetUniform("MVP", m_camera.GetViewProj() * model);
	m_programPointNormal.SetUniform("world", model);
	// m_programPointNormal.SetUniform("projection", projection);
	m_programPointNormal.SetUniform("worldIT", glm::inverse(glm::transpose(model)));
	if (showNormals)
	{
		glDrawArrays(GL_POINTS, 0, vertexNum);
	}

	// rectangles

	m_programRectangle.Use();

	if (useBearTexture1 == 0)
	{
		m_programRectangle.SetTexture("texImage", 0, m_bearTexture1);
	}
	else
	{
		m_programRectangle.SetTexture("texImage", 0, m_bearTexture2);
	}
	glUniform1i(m_programRectangle.GetLocation("l"), l);
	glUniform1i(m_programRectangle.GetLocation("w"), w);
	glUniform1i(m_programRectangle.GetLocation("rectColOrTex"), rectColOrTex);
	m_programRectangle.SetUniform("direction", direction);
	m_programRectangle.SetUniform("MVP", m_camera.GetViewProj() * model);
	m_programRectangle.SetUniform("world", model);
	m_programRectangle.SetUniform("worldIT", glm::inverse(glm::transpose(model)));
	if (showRectangles)
	{
		glDrawArrays(GL_POINTS, 0, vertexNum);
	}

	//
	// Skybox
	//

	// Save the last Z-test, namely the relation by which we update the pixel.
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);

	// Now we use less-then-or-equal, because we push everything to the far clipping plane
	glDepthFunc(GL_LEQUAL);

	m_SkyboxVao.Bind();

	m_programSkybox.Use();
	m_programSkybox.SetUniform("MVP", m_camera.GetViewProj() * glm::translate( m_camera.GetEye()) );
	
	// Set the cube map texture to the 0th sampler (in the shader too)
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
	glUniform1i(m_programSkybox.GetLocation("skyboxTexture"), 0);
	glUniform1i(m_programSkybox.GetLocation("skyboxBackground"), skyboxBackground);

	// The last 3 rows <=> m_programSkybox.SetCubeTexture("skyboxTexture", 0, m_skyboxTexture);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

	// Finally set it back
	glDepthFunc(prevDepthFnc);

	//
	// Axes
	//

	glDisable(GL_DEPTH_TEST);
	m_programAxes.Use();
	
	m_programAxes.SetUniform("MVP", m_camera.GetViewProj());
	// glDrawArrays(GL_LINES, 0, 6);

	m_programAxes.Unuse();
	glEnable(GL_DEPTH_TEST);
	
	//
	// GUI
	//

	// ImGui Testwindow
	// ImGui::ShowTestWindow();

	// another ImGui window
	if (ImGui::Begin("ImGui example"))
	{
		if(ImGui::CollapsingHeader("Refresh Options"))
		{
			static std::random_device rd;
			static std::mt19937 mt(rd());

			static float refresh_time = 0.1f;
			static float timer = 0;
			static int   frameCount = 0;
			static float fps = 0;

			timer += static_cast<float>(m_delta_time);
			++frameCount;
			if (timer > refresh_time) {
				fps = frameCount / timer;
				timer = 0;
				frameCount = 0;
			}
			ImGui::Text("FPS: %d", static_cast<int>(fps));

			ImGui::SliderFloat("Refresh time", &refresh_time, 0.01f, 1.0f);
		}
		if (ImGui::CollapsingHeader("Skybox Options"))
		{
			ImGui::Text("Select skybox background:");
			ImGui::RadioButton("Color gradient", &skyboxBackground, 0); ImGui::SameLine();
			ImGui::RadioButton("Sky texture", &skyboxBackground, 1);
		}
		if (ImGui::CollapsingHeader("Point Cloud Options"))
		{
			ImGui::Checkbox("Render Points", &showPoints);

			ImGui::Text("Select color or texture of points:");
			ImGui::RadioButton("Color 1", &e, 0); ImGui::SameLine();
			ImGui::RadioButton("Color 2", &e, 1); ImGui::SameLine();
			ImGui::RadioButton("Texture 1", &e, 2); ImGui::SameLine();
			ImGui::RadioButton("Texture 2", &e, 3);		
		}
		if (ImGui::CollapsingHeader("Point Normal Options"))
		{
			ImGui::Checkbox("Render Normals", &showNormals);

			ImGui::Text("Select length of normal vector (increments of 0.01):");
			ImGui::SliderInt("magnitude", &normal_magnitude, 0, 20);
		}
		if (ImGui::CollapsingHeader("Rectangle Options"))
		{
			ImGui::Checkbox("Render Rectangles", &showRectangles);

			ImGui::Text("Select size of rectangle (increments of 0.01):");
			ImGui::SliderInt("width", &w, 0, 15);
			ImGui::SliderInt("length", &l, 0, 15);

			ImGui::Text("Select texture:");
			ImGui::RadioButton("Bear 1", &useBearTexture1, 0); ImGui::SameLine();
			ImGui::RadioButton("Bear 2", &useBearTexture1, 1);

			ImGui::Text("Select Color/Texture of Rectangles:");
			ImGui::RadioButton("Rect. Color 1", &rectColOrTex, 0); ImGui::SameLine();
			ImGui::RadioButton("Rect. Color 2", &rectColOrTex, 1);
			ImGui::RadioButton("Rect. Tex 1", &rectColOrTex, 2); ImGui::SameLine();
			ImGui::RadioButton("Rect. Tex 2", &rectColOrTex, 3);
		}
	} // window
	ImGui::End();
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardDown(key);
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardUp(key);
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	m_camera.MouseMove(mouse);
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

// _w and _h are the width and height of the window's size
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h );

	m_camera.Resize(_w, _h);
}

void CMyApp::ReadPointData()
{
	std::ifstream input_file(fileName);
	int line_countdown = 5;
	std::string line;

	glm::vec3 p;
	glm::vec3 c1;
	glm::vec3 c2;
	glm::vec2 t1;
	glm::vec2 t2;
	glm::vec3 n;

	while (std::getline(input_file, line))
	{
		float x = 0.0, y = 0.0, z = 0.0;

		std::istringstream in(line);
		
		switch (line_countdown)
		{
			case 5:
				in >> x >> y >> z;
				p = glm::vec3(x, y, z);
				line_countdown--;
				break;
			case 4:
				in >> x >> y >> z;
				c1 = glm::vec3(x, y, z);
				line_countdown--;
				break;
			case 3:
				in >> x >> y >> z;
				c2 = glm::vec3(x, y, z);
				line_countdown--;
				break;
			case 2:
				in >> x >> y;
				t1 = glm::vec2(x, y);
				line_countdown--;
				break;
			case 1:
				in >> x >> y;
				t2 = glm::vec2(x, y);
				line_countdown--;
				break;
			case 0:
				in >> x >> y >> z;
				n = glm::vec3(x, y, z);
				vertices.push_back({ p, c1, c2, t1, t2, n });
				line_countdown = 5;
				break;
		}		
	}
}