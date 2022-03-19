#version 430

#include "../fragments/fs_common_inputs.glsl"

// We output a single color to the color buffer
layout(location = 0) out vec4 frag_color;

////////////////////////////////////////////////////////////////
/////////////// Instance Level Uniforms ////////////////////////
////////////////////////////////////////////////////////////////

// Represents a collection of attributes that would define a material
// For instance, you can think of this like material settings in 
// Unity
struct Material {
	sampler2D Diffuse;
	sampler2D Specular;
	float     Shininess;
	int       Steps;
};
// Create a uniform for the material
uniform Material u_Material;

uniform sampler1D s_ToonTerm;

////////////////////////////////////////////////////////////////
///////////// Application Level Uniforms ///////////////////////
////////////////////////////////////////////////////////////////

#include "../fragments/multiple_point_lights.glsl"

////////////////////////////////////////////////////////////////
/////////////// Frame Level Uniforms ///////////////////////////
////////////////////////////////////////////////////////////////

#include "../fragments/frame_uniforms.glsl"
#include "../fragments/color_correction.glsl"

// https://learnopengl.com/Advanced-Lighting/Advanced-Lighting
void main() {
	//set variables
	float amb = 0.1;
	float specstr = 0.6;

	// Normalize our input normal
	vec3 normal = normalize(inNormal);

	//find the power of the specular acording to the materials
	float specPower = texture(u_Material.Specular, inUV).r;
	
	vec3 toEye = normalize(u_CamPos.xyz - inWorldPos);
	vec3 environmentDir = reflect(-toEye, normal);

	// Get the albedo from the diffuse / albedo map
	vec4 textureColor = texture(u_Material.Diffuse, inUV);

	//find the specular reflection towards the camera 
	float specular = pow(max(dot(toEye, environmentDir), 0.0), 32);

	//calculate the diffuse lighting
	vec3 lightAccumulation = CalcAllLightContribution(inWorldPos, normal, u_CamPos.xyz, u_Material.Shininess);


	// flags

	vec3 result = vec3 (0);

	// set flag for ambient
    if (IsFlagSet(FLAG_ENABLE_AMBIENT)) {
        result = amb * textureColor.rgb;
    }
    
	// set flag for specular
    if (IsFlagSet(FLAG_ENABLE_SPECULAR)) {
        result = (specstr * specular) * textureColor.rgb;
    }

	// set flag for ambient + specular
	if (IsFlagSet(FLAG_ENABLE_AMBIENTSPECULAR)) {
        result = ((specstr * specular) + amb) * textureColor.rgb;
    }

	// set flag for ambient + specular + diffuse + toon
	if (IsFlagSet(FLAG_ENABLE_AMBIENTSPECULARTOON)) {
        result = ((specstr * specular) + amb + (lightAccumulation  * inColor)) * textureColor.rgb;

		// to add toon we have to warp the results rgb values in accordince with the toonterm
		result.r = texture(s_ToonTerm, result.r).r;
		result.g = texture(s_ToonTerm, result.g).g;
		result.b = texture(s_ToonTerm, result.b).b;
    }


	frag_color = vec4(ColorCorrect(result), 1.0);
}