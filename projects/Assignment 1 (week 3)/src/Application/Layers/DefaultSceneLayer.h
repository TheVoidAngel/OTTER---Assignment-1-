#pragma once
#include "Application/ApplicationLayer.h"
#include "Gameplay/Scene.h"
#include "json.hpp"

/**
 * This example layer handles creating a default test scene, which we will use 
 * as an entry point for creating a sample scene
 */
class DefaultSceneLayer final : public ApplicationLayer {
public:
	MAKE_PTRS(DefaultSceneLayer)

	DefaultSceneLayer();
	virtual ~DefaultSceneLayer();

	// Inherited from ApplicationLayer

	virtual void OnAppLoad(const nlohmann::json& config) override;
	virtual void OnUpdate() override;

	bool warmed = false;
	bool cooled = false;
	bool greened = false;

	Texture3D::Sptr lut;
	Texture3D::Sptr warmlut;
	Texture3D::Sptr coollut;
	Texture3D::Sptr greenlut;

protected:
	void _CreateScene();
};