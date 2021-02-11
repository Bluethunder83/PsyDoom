#version 460

//----------------------------------------------------------------------------------------------------------------------
// Sky shader: fragment
// This is used for rendering the sky in parts of the ceiling or on walls that have been marked as 'sky'.
//----------------------------------------------------------------------------------------------------------------------
#include "ShaderCommon_Frag.h"

DECLARE_UNIFORMS()

layout(set = 0, binding = 0) uniform usampler2D vramTex;

layout(location = 0) in vec4 in_clipPos;
layout(location = 1) flat in vec2 in_uv_offset;
layout(location = 2) flat in ivec2 in_texWinPos;
layout(location = 3) flat in ivec2 in_texWinSize;
layout(location = 4) flat in ivec2 in_clutPos;

layout(location = 0) out vec4 out_color;

//----------------------------------------------------------------------------------------------------------------------
// Converts a clip space position into the original PSX framebuffer coordinate system (256x240)
//----------------------------------------------------------------------------------------------------------------------
vec2 clipspaceToPsxCoords(vec4 clipSpace) {
    // Convert to normalized device coordinates and make NDC coords be from 0-2 instead of -1 to +1
    clipSpace /= clipSpace.w;
    vec2 ndc = clipSpace.xy + 1.0;

    // Convert to the PSX coord system
    return (ndc - constants.psxNdcOffset) * constants.ndcToPsxScale;
}

//----------------------------------------------------------------------------------------------------------------------
// Shades the sky fragment
//----------------------------------------------------------------------------------------------------------------------
void main() {
    // Convert the clipspace vertex position to PSX framebuffer coordinates
    vec2 psxCoords = clipspaceToPsxCoords(in_clipPos);

    // The uv coord to use for the sky is based on the PSX framebuffer coord.
    // We also add offsetting to account for view rotation and that is stored in the vertex uv attribute.
    vec2 uv = psxCoords + in_uv_offset;

    // Just do a standard 8bpp texel lookup to draw the sky
    out_color = tex8bpp(vramTex, ivec2(uv), in_texWinPos, in_texWinSize, in_clutPos, vec4(1, 1, 1, 1));
    out_color = psxR5G5B5BitCrush(out_color);
}

