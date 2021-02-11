#version 460

//----------------------------------------------------------------------------------------------------------------------
// World shader: vertex.
// This shader handles all in-game walls, floors, and sprites with or without light diminishing effects.
// All textures are also assumed to be 8 bits per pixel.
//----------------------------------------------------------------------------------------------------------------------
#include "ShaderCommon_Vert.h"

DECLARE_UNIFORMS()
DECLARE_VS_INPUTS_VVERTEX_DRAW()

layout(location = 0) out vec3 out_color;
layout(location = 1) out vec3 out_uv_z;
layout(location = 2) flat out vec3 out_lightDimModeStrength;
layout(location = 3) flat out ivec2 out_texWinPos;              // Note: pre-convert these to ivec2 for the frag shader
layout(location = 4) flat out ivec2 out_texWinSize;
layout(location = 5) flat out ivec2 out_clutPos;
layout(location = 6) flat out vec4 out_stmul;

void main() {
    gl_Position = constants.mvpMatrix * vec4(in_pos, 1);
    out_color = in_color;
    out_uv_z = vec3(in_uv, gl_Position.z);
    out_lightDimModeStrength.x = (in_lightDimMode == 0) ? 1.0 : 0.0;    // No light diminishing
    out_lightDimModeStrength.y = (in_lightDimMode == 1) ? 1.0 : 0.0;    // Wall light diminishing
    out_lightDimModeStrength.z = (in_lightDimMode == 2) ? 1.0 : 0.0;    // Floor light diminishing
    out_texWinPos = ivec2(in_texWinPos);
    out_texWinSize = ivec2(in_texWinSize);
    out_clutPos = ivec2(in_clutPos);
    out_stmul = in_stmul / 128.0;
}
