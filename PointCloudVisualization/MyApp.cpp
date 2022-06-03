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
	m_camera.SetView(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}


CMyApp::~CMyApp(void)
{
	std::cout << "dtor!\n";
}

void CMyApp::InitPointCloud()
{
	//struct Vertex{ glm::vec3 position; glm::vec3 normal; glm::vec2 texture; };
	std::vector<Vertex>vertices;

	// test points									 
	vertices.push_back({ glm::vec3(0.727813, -1.4546, -2.40505), glm::vec3(0.811765, 0.843137, 1), glm::vec3(0.690196, 0.254902, 0.0784314), glm::vec2(0.39972, 0.815711), glm::vec2(0.438485, 0.766398), glm::vec3(-0.0077142, 0.98009, 0.98009) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, +0.5), glm::vec3(0, 0, 0), glm::vec3(255, 255, 255), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec3(0, 0, 1) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, +0.5), glm::vec3(0, 0, 0), glm::vec3(255, 255, 255), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec3(0, 0, 1) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, +0.5), glm::vec3(0, 0, 0), glm::vec3(255, 255, 255), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec3(0, 0, 1) });

	//std::vector<int> indices(36);
	//int index = 0;
	//for (int i = 0; i < 6 * 4; i += 4)
	//{
	//	indices[index + 0] = i + 0;
	//	indices[index + 1] = i + 1;
	//	indices[index + 2] = i + 2;
	//	indices[index + 3] = i + 1;
	//	indices[index + 4] = i + 3;
	//	indices[index + 5] = i + 2;
	//	index += 6;
	//}

	//
	// Defining geometry (std::vector<...>) and upload to GPU buffers (m_buffer*) with BufferData
	//

	// Position of vertices:
	/*
	The constructor of m_CubeVertexBuffer has already created a GPU buffer identifier, and the following BufferData call will
	1. bind this to GL_ARRAY_BUFFER (because the type of m_gpuBufferPos is ArrayBuffer) and
	2. upload the values of the container given in the argument to the GPU by calling glBufferData
	*/

	m_PCVertexBuffer.BufferData(vertices);

	// And the indices which the primitives are constructed by (from the array defined above) - prepared to draw them as a triangle list
	// m_CubeIndices.BufferData(indices);

	// Registering geometry in VAO
	m_PCVao.Init(
		{
			// Attribute 0 is "practically" an array of glm::vec3 and the data is in the GPU buffer (m_CubeVertexBuffer)
			{ CreateAttribute<		0,						// Channel 0
									glm::vec3,				// CPU-side data type which is used to define attributes of channel 0 <- the procedure deducts that the attribute 0 is made of 3 floats from the glm::vec3
									offsetof(Vertex, p),	// offset: The offset of the attribute, considered from the beginning of the container
									sizeof(Vertex)			// stride: This attribute of the next vertex will be this many bytes from the current
								>, m_PCVertexBuffer },
			{ CreateAttribute<1, glm::vec3, offsetof(Vertex, c1), sizeof(Vertex)>, m_PCVertexBuffer },
			{ CreateAttribute<2, glm::vec3, offsetof(Vertex, c2), sizeof(Vertex)>, m_PCVertexBuffer },
			{ CreateAttribute<3, glm::vec2, offsetof(Vertex, t1), sizeof(Vertex)>, m_PCVertexBuffer },
			{ CreateAttribute<4, glm::vec2, offsetof(Vertex, t2), sizeof(Vertex)>, m_PCVertexBuffer },
			{ CreateAttribute<5, glm::vec3, offsetof(Vertex, n), sizeof(Vertex)>, m_PCVertexBuffer },
		}
	);
}

void CMyApp::InitPointNormal()
{
	//struct Vertex{ glm::vec3 position; glm::vec3 normal; glm::vec2 texture; };
	std::vector<Vertex>vertices;

	// test points									 
	vertices.push_back({ glm::vec3(0.727813, -1.4546, -2.40505), glm::vec3(0.811765*255, 0.843137*255, 1*255), glm::vec3(0.690196 * 255, 0.254902 * 255, 0.0784314 * 255), glm::vec2(0.39972, 0.815711), glm::vec2(0.438485, 0.766398), glm::vec3(-0.0077142, 0.98009, 0.98009) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, +0.5), glm::vec3(0, 0, 0), glm::vec3(255, 255, 255), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec3(0, 0, 1) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, +0.5), glm::vec3(0, 0, 0), glm::vec3(255, 255, 255), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec3(0, 0, 1) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, +0.5), glm::vec3(0, 0, 0), glm::vec3(255, 255, 255), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec3(0, 0, 1) });


	m_PNVertexBuffer.BufferData(vertices);

	// Registering geometry in VAO
	m_PNVao.Init(
		{
			// Attribute 0 is "practically" an array of glm::vec3 and the data is in the GPU buffer (m_CubeVertexBuffer)
			{ CreateAttribute<		0,						// Channel 0
									glm::vec3,				// CPU-side data type which is used to define attributes of channel 0 <- the procedure deducts that the attribute 0 is made of 3 floats from the glm::vec3
									offsetof(Vertex, p),	// offset: The offset of the attribute, considered from the beginning of the container
									sizeof(Vertex)			// stride: This attribute of the next vertex will be this many bytes from the current
								>, m_PNVertexBuffer },
			{ CreateAttribute<1, glm::vec3, offsetof(Vertex, c1), sizeof(Vertex)>, m_PNVertexBuffer },
			{ CreateAttribute<2, glm::vec3, offsetof(Vertex, c2), sizeof(Vertex)>, m_PNVertexBuffer },
			{ CreateAttribute<3, glm::vec2, offsetof(Vertex, t1), sizeof(Vertex)>, m_PNVertexBuffer },
			{ CreateAttribute<4, glm::vec2, offsetof(Vertex, t2), sizeof(Vertex)>, m_PNVertexBuffer },
			{ CreateAttribute<5, glm::vec3, offsetof(Vertex, n), sizeof(Vertex)>, m_PNVertexBuffer }
		}
	);
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

	glEnable(GL_CULL_FACE); // Drop faces looking backwards
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

	//// Mapping attributes between VAO and shaders
	//m_program.BindAttribLocations({
	//	{ 0, "vs_in_pos" },				// Channel 0 of the VAO should go into vs_in_pos
	//	{ 1, "vs_in_norm" },			// Channel 1 of the VAO should go into vs_in_norm
	//	{ 2, "vs_in_tex" },				// Channel 1 of the VAO should go into vs_in_tex		
	//});

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

	// init geometry
	InitPointCloud();
	InitPointNormal();
	// InitCube();
	// init skybox geometry and texture
	InitSkyBox();

	// load texture
	m_mossyTexture.FromFile("Assets/mossy.png");

	// load mesh
	//m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("Assets/Suzanne.obj"));
	//m_mesh->initBuffers();

	// Camera
	m_camera.SetProj(glm::radians(60.0f), 640.0f / 480.0f, 0.01f, 1000.0f);


	// Initializing the cubes
	std::fill_n(m_isVisible.begin(), m_isVisible.size(), true);

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

	//
	// Mesh (Suzanne)
	//
	//glm::mat4 suzanneWorld = glm::translate(glm::vec3(0.0f, 2.0f, 0.0f));
	//m_program.Use();
	//m_program.SetTexture("texImage", 0, m_mossyTexture);
	//m_program.SetUniform("MVP", m_camera.GetViewProj() * suzanneWorld);
	//m_program.SetUniform("world", suzanneWorld);
	//m_program.SetUniform("worldIT", glm::inverse(glm::transpose(suzanneWorld)));
	//m_mesh->draw();

	//
	// Cubes
	//
	// m_CubeVao.Bind();
	m_PCVao.Bind();

	m_program.Use();

	// point cloud
	glm::mat4 pointCloudWorld = glm::mat4(1);
	glUniform1i(m_program.GetLocation("colOrTexIndex"), e); // TODO: using ImGui to select between c1, c2, t1, t2
	m_program.SetTexture("texImage", 0, m_mossyTexture); // TODO: using ImGui select either texture image 1 or 2
	m_program.SetUniform("MVP", m_camera.GetViewProj() * pointCloudWorld);
	m_program.SetUniform("world", pointCloudWorld);
	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(m_camera.GetViewProj() * pointCloudWorld)));
	// glDrawElements(GL_POINTS, 5, GL_UNSIGNED_INT, nullptr);
	glDrawArrays(GL_POINTS, 0, 5);

	// point normal visualization

	m_PNVao.Bind();

	m_programPointNormal.Use();

	glm::mat4 pointNormalWorld = glm::mat4(1);
	glm::mat4 view = m_camera.GetViewMatrix();
	glm::mat4 projection = m_camera.GetProj();
	glUniform1i(m_programPointNormal.GetLocation("normal_magnitude"), normal_magnitude);
	m_programPointNormal.SetUniform("projection", projection);
	m_programPointNormal.SetUniform("MVP", view * pointNormalWorld);
	m_programPointNormal.SetUniform("world", pointNormalWorld);
	m_programPointNormal.SetUniform("worldIT", glm::inverse(glm::transpose(view * pointNormalWorld)));
	glDrawArrays(GL_POINTS, 0, 5);

	// Main cube
	//glm::mat4 cubeWorld = glm::mat4(1);
	//m_program.SetUniform("MVP", m_camera.GetViewProj() * cubeWorld);
	//m_program.SetUniform("world", cubeWorld);
	//m_program.SetUniform("worldIT", glm::inverse(glm::transpose(cubeWorld)));
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

	// Little cubes
	//for (int i = 0; i < m_cubeNo; ++i)
	//{
	//	if (!m_isVisible[i])
	//		continue;

	//	cubeWorld = 
	//		glm::rotate( SDL_GetTicks()/1000.0f + 2 * glm::pi<float>()/10*i, glm::vec3(0,1,0) )*
	//		glm::translate(glm::vec3(10 + 5*sinf(SDL_GetTicks()/1000.0f),0,0))*
	//		glm::rotate( (i+1)*SDL_GetTicks() / 1000.0f, glm::vec3(0, 1, 0))*
	//		glm::scale(glm::vec3(0.5));
	//	m_program.SetUniform("MVP", m_camera.GetViewProj() * cubeWorld);
	//	m_program.SetUniform("world", cubeWorld);
	//	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(cubeWorld)));
	//	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	//}

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
	glDrawArrays(GL_LINES, 0, 6);

	m_programAxes.Unuse();
	glEnable(GL_DEPTH_TEST);
	
	//
	// GUI
	//

	// ImGui Testwindow
	ImGui::ShowTestWindow();

	// another ImGui window
	if (ImGui::Begin("ImGui example"))
	{
		if (ImGui::CollapsingHeader("Options"))
		{
			static std::random_device rd;
			static std::mt19937 mt(rd());
			static std::uniform_int_distribution<int> random(0, m_cubeNo-1);

			ImGui::Text("Trigger the visibility of one random cube");
			if (ImGui::Button("Button")) {
				auto& visible = m_isVisible[random(mt)];
				visible = !visible; 
			}

			ImGui::Text("Select Color or Texture of points");
			// static int e = 0;
			ImGui::RadioButton("Color 1", &e, 0); ImGui::SameLine();
			ImGui::RadioButton("Color 2", &e, 1); ImGui::SameLine();
			ImGui::RadioButton("Texture 1", &e, 2); ImGui::SameLine();
			ImGui::RadioButton("Texture 2", &e, 3);

			ImGui::Text("Select length of normal vector");
			ImGui::SliderInt("magnitude", &normal_magnitude, 0, 15);

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