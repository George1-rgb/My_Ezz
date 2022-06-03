#include <iostream>
#include <memory>
#include <My_Ezz_Core/Application.hpp>
#include <imgui/imgui.h>

class My_Ezz_Editor : public My_Ezz::Application
{
	virtual void on_update() override
	{
		//std::cout << "Update frame: " << frame++ << std::endl;
	}

	virtual void on_UIDraw() override
	{
		ImGui::Begin("Editor");
		ImGui::SliderFloat3("Camera position", cameraPosition, -10.0f, 10.0f);
		ImGui::SliderFloat3("Camera rotation", cameraRotation, 0.0f, 360.0f);
		ImGui::Checkbox("Perspective camera", &perspectiveCamera);
		ImGui::End();
	}

	int frame = 0;
};
int main()
{
	auto pMy_Ezz_Editor = std::make_unique<My_Ezz_Editor>();

	int returnCode = pMy_Ezz_Editor->start(1024, 768, "My_Ezz Editor");

	std::cin.get();
	return returnCode;
}