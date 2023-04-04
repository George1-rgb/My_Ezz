#pragma once
struct GLFWwindow;
namespace My_Ezz
{
	class UIModule
	{
	public:
		static void onWindowCreate(GLFWwindow* _pWindow);
		static void onWindowClose();

		static void onWindowUpdateBegin();
		static void onWindowUpdateDraw();
	private:

	};
}