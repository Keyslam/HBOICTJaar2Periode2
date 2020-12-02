#define GLEW_STATIC

#include "Renderer.hpp"

#include <glad/glad.h>
#include <SDL.h>

#include <iostream>

#include "Instrumentor.hpp"

#include "stb_image.h"

Renderer::Renderer() {

}

Renderer::~Renderer() {
	delete shader;

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool Renderer::Init() {
	PROFILE_FUNCTION();

	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


	window = SDL_CreateWindow("segfault simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, context);

	{

		GLenum glewError = gladLoadGL();

		/*
		if (glewError)
		{
			std::cout << "Error initializing GLEW!" << std::endl;
			std::cout << glewGetErrorString(glewError) << std::endl;
			return false;
		}
		*/
	}

	if (!GLAD_GL_VERSION_3_3)
	{
		std::cout << "OpenGL 3.3 not supported!" << std::endl;
		return false;
	}

	

	/*
	vertices[0] = 14.0f; vertices[1] = 18.0f; vertices[2] = 1.0f; vertices[3] = 1.0f;
	vertices[4] = 14.0f; vertices[5] = -18.0f; vertices[6] = 1.0f; vertices[7] = 0.0f;
	vertices[8] = -14.0f; vertices[9] = -18.0f; vertices[10] = 0.0f; vertices[11] = 0.0f;
	vertices[12] = -14.0f; vertices[13] = 18.0f; vertices[14] = 0.0f; vertices[15] = 1.0f;
	*/
	/*
	vertices = {
		 14.0f,  18.0f, 1.0f, 1.0f, // top right
		 14.0f, -18.0f, 1.0f, 0.0f, // bottom right
		-14.0f, -18.0f, 0.0f, 0.0f, // bottom left
		-14.0f,  18.0f, 0.0f, 1.0f  // top left 
	};
	*/

	unsigned int indices[30000 * 6] = {};
	for (int i = 0; i < 30000; i++) {
		indices[i * 6 + 0] = i * 4 + 0;
		indices[i * 6 + 1] = i * 4 + 1;
		indices[i * 6 + 2] = i * 4 + 3;
		indices[i * 6 + 3] = i * 4 + 1;
		indices[i * 6 + 4] = i * 4 + 2;
		indices[i * 6 + 5] = i * 4 + 3;
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// load and create a texture 
	// -------------------------
	unsigned int texture1;
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load("Spritesheet.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	Shader* shader = new Shader("vertex.glsl", "fragment.glsl");

	shader->SetInt("texture1", 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(151 / 255.0f, 99 / 255.0f, 255 / 255.0f, 1.0f);
	shader->Use();
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	SDL_GL_SetSwapInterval(0);

	for (int i = 0; i < 5; i++) {
		SDL_GL_SwapWindow(window);
	}

	return true;
}

void Renderer::DrawBlueTank(int x, int y) {
	const Vertex bv[] = {
		 Vertex( 7.0f,  9.0f, 14.0f / 447.0f, 1.0f),                  // top right
		 Vertex( 7.0f, -9.0f, 14.0f / 447.0f, 1.0f - 17.0f / 314.0f), // bottom right
		 Vertex(-7.0f, -9.0f,  0.0f,          1.0f - 17.0f / 314.0f), // bottom left
		 Vertex(-7.0f,  9.0f,  0.0f,          1.0f)                   // top left 
	};

	vertices[vertexCount + 0].SetWithOffset(bv[0], x, -y, 0, 0);
	vertices[vertexCount + 1].SetWithOffset(bv[1], x, -y, 0, 0);
	vertices[vertexCount + 2].SetWithOffset(bv[2], x, -y, 0, 0);
	vertices[vertexCount + 3].SetWithOffset(bv[3], x, -y, 0, 0);

	vertexCount += 4;
}

void Renderer::DrawBlueRocket(int x, int y) {
	const Vertex bv[] = {
		 Vertex( 12.5f,  12.0f, 26.0f / 447.0f, 1.0f - 34.0f / 314.0f), // top right
		 Vertex( 12.5f, -12.0f, 26.0f / 447.0f, 1.0f - 59.0f / 314.0f), // bottom right
		 Vertex(-12.5f, -12.0f,  0.0f,          1.0f - 59.0f / 314.0f), // bottom left
		 Vertex(-12.5f,  12.0f,  0.0f,          1.0f - 34.0f / 314.0f)  // top left 
	};

	vertices[vertexCount + 0].SetWithOffset(bv[0], x, -y, 0, 0);
	vertices[vertexCount + 1].SetWithOffset(bv[1], x, -y, 0, 0);
	vertices[vertexCount + 2].SetWithOffset(bv[2], x, -y, 0, 0);
	vertices[vertexCount + 3].SetWithOffset(bv[3], x, -y, 0, 0);

	vertexCount += 4;
}

void Renderer::DrawRedTank(int x, int y) {
	const Vertex bv[] = {
		Vertex( 7.0f,  9.0f, 56.0f / 447.0f, 1.0f - 17.0f / 314.0f), // top right
		Vertex( 7.0f, -9.0f, 56.0f / 447.0f, 1.0f - 34.0f / 314.0f), // bottom right
		Vertex(-7.0f, -9.0f, 42.0f / 447.0f, 1.0f - 34.0f / 314.0f), // bottom left
		Vertex(-7.0f,  9.0f, 42.0f / 447.0f, 1.0f - 17.0f / 314.0f)  // top left 
	};

	vertices[vertexCount + 0].SetWithOffset(bv[0], x, -y, 0, 0);
	vertices[vertexCount + 1].SetWithOffset(bv[1], x, -y, 0, 0);
	vertices[vertexCount + 2].SetWithOffset(bv[2], x, -y, 0, 0);
	vertices[vertexCount + 3].SetWithOffset(bv[3], x, -y, 0, 0);

	vertexCount += 4;
}

void Renderer::DrawRedRocket(int x, int y) {
	const Vertex bv[] = {
		Vertex( 12.5f,  12.0f, 26.0f / 447.0f, 1.0f - 60.0f / 314.0f), // top right
		Vertex( 12.5f, -12.0f, 26.0f / 447.0f, 1.0f - 85.0f / 314.0f), // bottom right
		Vertex(-12.5f, -12.0f,  0.0f,          1.0f - 85.0f / 314.0f), // bottom left
		Vertex(-12.5f,  12.0f,  0.0f,          1.0f - 60.0f / 314.0f)  // top left 
	};

	vertices[vertexCount + 0].SetWithOffset(bv[0], x, -y, 0, 0);
	vertices[vertexCount + 1].SetWithOffset(bv[1], x, -y, 0, 0);
	vertices[vertexCount + 2].SetWithOffset(bv[2], x, -y, 0, 0);
	vertices[vertexCount + 3].SetWithOffset(bv[3], x, -y, 0, 0);

	vertexCount += 4;
}

void Renderer::Clear() {
	PROFILE_FUNCTION();

	glClear(GL_COLOR_BUFFER_BIT);
	vertexCount = 0;
}

void Renderer::Present() {
	PROFILE_FUNCTION();

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glDrawElements(GL_TRIANGLES, vertexCount * 1.5, GL_UNSIGNED_INT, 0);

	SDL_GL_SwapWindow(window);

	// SDL_Delay(20);
}