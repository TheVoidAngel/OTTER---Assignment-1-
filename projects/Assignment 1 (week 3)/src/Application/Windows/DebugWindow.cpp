#include "DebugWindow.h"
#include "Application/Application.h"
#include "Application/ApplicationLayer.h"
#include "Application/Layers/RenderLayer.h"

DebugWindow::DebugWindow() :
	IEditorWindow()
{
	Name = "Debug";
	SplitDirection = ImGuiDir_::ImGuiDir_None;
	SplitDepth = 0.5f;
	Requirements = EditorWindowRequirements::Menubar;
}

DebugWindow::~DebugWindow() = default;

void DebugWindow::RenderMenuBar() 
{
	Application& app = Application::Get();
	RenderLayer::Sptr renderLayer = app.GetLayer<RenderLayer>();

	BulletDebugMode physicsDrawMode = app.CurrentScene()->GetPhysicsDebugDrawMode();
	if (BulletDebugDraw::DrawModeGui("Physics Debug Mode:", physicsDrawMode)) {
		app.CurrentScene()->SetPhysicsDebugDrawMode(physicsDrawMode);
	}

	ImGui::Separator();

	RenderFlags flags = renderLayer->GetRenderFlags();
	bool changed = false;
	//I had to set multiple temp bools so each toggle had their own
	bool temp = *(flags & RenderFlags::EnableColorCorrection);
	bool temp1 = *(flags & RenderFlags::EnableAmbient);
	bool temp2 = *(flags & RenderFlags::EnableSpecular);
	bool temp3 = *(flags & RenderFlags::EnableAmbientSpecular);
	bool temp4 = *(flags & RenderFlags::EnableAmbientSpecularToon);



	if (ImGui::Checkbox("Enable Color Correction", &temp)) {
		changed = true;
		flags = (flags & ~*RenderFlags::EnableColorCorrection) | (temp ? RenderFlags::EnableColorCorrection : RenderFlags::None);
	}

	// This allows me to use the debug feture to activate my lighting as it partains to each object in the scene
	if (ImGui::Checkbox("Enable Ambient", &temp1)) {
		changed = true;
		flags = (flags & ~*RenderFlags::EnableAmbient) | (temp1 ? RenderFlags::EnableAmbient : RenderFlags::None);
	}

	if (ImGui::Checkbox("Enable Specular", &temp2)) {
		changed = true;
		flags = (flags & ~*RenderFlags::EnableSpecular) | (temp2 ? RenderFlags::EnableSpecular : RenderFlags::None);
	}

	if (ImGui::Checkbox("Enable Ambient and Specular", &temp3)) {
		changed = true;
		flags = (flags & ~*RenderFlags::EnableAmbientSpecular) | (temp3 ? RenderFlags::EnableAmbientSpecular : RenderFlags::None);
	}

	if (ImGui::Checkbox("Enable All + diffuse and toon", &temp4)) {
		changed = true;
		flags = (flags & ~*RenderFlags::EnableAmbientSpecularToon) | (temp4 ? RenderFlags::EnableAmbientSpecularToon : RenderFlags::None);
	}

	if (changed) {
		renderLayer->SetRenderFlags(flags);
	}
}
