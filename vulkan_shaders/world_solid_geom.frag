#version 460

//----------------------------------------------------------------------------------------------------------------------
// World solid geometry shader: fragment.
// This shader is responsible for drawing fully opaque walls and floors, with light diminishing effects.
// All textures are also assumed to be 8 bits per pixel.
//----------------------------------------------------------------------------------------------------------------------
#include "ShaderCommon_Frag.h"

layout(set = 0, binding = 0) uniform usampler2D vramTex;

layout(location = 0) in vec3 in_color;
layout(location = 1) in vec3 in_uv_z;
layout(location = 2) flat in vec3 in_lightDimModeStrength;
layout(location = 3) flat in uvec2 in_texWinPos;
layout(location = 4) flat in uvec2 in_texWinSize;
layout(location = 5) flat in uvec2 in_clutPos;

layout(location = 0) out vec4 out_color;

void main() {
    out_color = shadeWorldTexel(
        true,
        vramTex,
        in_color,
        in_uv_z,
        in_lightDimModeStrength,
        in_texWinPos,
        in_texWinSize,
        in_clutPos,
        vec4(1, 1, 1, 1)
    );
}
