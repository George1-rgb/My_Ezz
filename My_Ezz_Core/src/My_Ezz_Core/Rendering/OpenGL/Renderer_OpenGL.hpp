#pragma once

struct GLFWwindow;

namespace My_Ezz
{
	class VertexArray;
	class Renderer_OpenGL
	{
	public:
		static bool init(GLFWwindow* _pWindow);

		static void draw(const VertexArray& _vertexArray);
		static void setClearColor(const float _red, const float _green, const float _blue, const float _alpha);
		static void clear();
		static void setViewport(const unsigned int _width, const unsigned int _height, const unsigned int _leftOffset = 0, const unsigned int _bottomOffset = 0);
		static void EnableDepthTesting();
		static void DisableDepthTesting();


		static const char* getVendorStr();
		static const char* getRendererStr();
		static const char* getVersionStr();
	};
}
