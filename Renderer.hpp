#pragma once

#include <SDL.h>
#include "Shader.hpp"

struct Vertex {
	float x;
	float y;
	float u;
	float v;

	Vertex() : x(0), y(0), u(0), v(0) { }
	Vertex(float x, float y, float u, float v) : x(x), y(y), u(u), v(v) { }
	
	void Set(float x, float y, float u, float v) {
		this->x = x;
		this->y = y;
		this->u = u;
		this->v = v;
	}

	void SetWithOffset(const Vertex& vertex, float x, float y, float u, float v) {
		this->x = vertex.x + x;
		this->y = vertex.y + y;
		this->u = vertex.u + u;
		this->v = vertex.v + v;
	}
};

class Renderer
{
public:
	Renderer();
	~Renderer();

	bool Init();

	void DrawBlueTank(int x, int y);
	void DrawBlueRocket(int x, int y);

	void DrawRedTank(int x, int y);
	void DrawRedRocket(int x, int y);

	void Clear();
	void Present();
private:
	SDL_Window* window;
	SDL_GLContext context;

	Shader* shader;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	Vertex vertices[30000 * 4];
	int vertexCount;
};