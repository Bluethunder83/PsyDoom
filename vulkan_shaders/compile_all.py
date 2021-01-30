#!python

############################################################################################################################################
# This script compiles all of the Vulkan GLSL format shaders for the project to SPIR-V binary code.
# The SPIR-V generated is saved in the format of C header files which can then be embedded in the application.
#
# Requirements:
#   (1) The Vulkan SDK 'glslangvalidator' tool must be invokable.
#       Install the SDK and ensure this tool is on your current system's BIN path.
#   (2) This script must be executed from the shaders directory.
############################################################################################################################################
import os
import subprocess
import sys

# Job-specs for all the files to compile.
# Corresponds to the 3 arguments of 'compile_shader'
files_to_compile = [
    [ "colored.frag",           "compiled/SPIRV_colored_frag.bin.h",            "frag", "gSPIRV_colored_frag"           ],
    [ "colored.vert",           "compiled/SPIRV_colored_vert.bin.h",            "vert", "gSPIRV_colored_vert"           ],
    [ "msaa_resolve.frag",      "compiled/SPIRV_msaa_resolve_frag.bin.h",       "frag", "gSPIRV_msaa_resolve_frag"      ],
    [ "msaa_resolve.vert",      "compiled/SPIRV_msaa_resolve_vert.bin.h",       "vert", "gSPIRV_msaa_resolve_vert"      ],
    [ "occ_plane.frag",         "compiled/SPIRV_occ_plane_frag.bin.h",          "frag", "gSPIRV_occ_plane_frag"         ],
    [ "occ_plane.vert",         "compiled/SPIRV_occ_plane_vert.bin.h",          "vert", "gSPIRV_occ_plane_vert"         ],
    [ "ui.vert",                "compiled/SPIRV_ui_vert.bin.h",                 "vert", "gSPIRV_ui_vert"                ],
    [ "ui_16bpp.frag",          "compiled/SPIRV_ui_16bpp_frag.bin.h",           "frag", "gSPIRV_ui_16bpp_frag"          ],
    [ "ui_4bpp.frag",           "compiled/SPIRV_ui_4bpp_frag.bin.h",            "frag", "gSPIRV_ui_4bpp_frag"           ],
    [ "ui_8bpp.frag",           "compiled/SPIRV_ui_8bpp_frag.bin.h",            "frag", "gSPIRV_ui_8bpp_frag"           ],
    [ "world_alpha_geom.frag",  "compiled/SPIRV_world_alpha_geom_frag.bin.h",   "frag", "gSPIRV_world_alpha_geom_frag"  ],
    [ "world_alpha_geom.vert",  "compiled/SPIRV_world_alpha_geom_vert.bin.h",   "vert", "gSPIRV_world_alpha_geom_vert"  ],
    [ "world_solid_geom.frag",  "compiled/SPIRV_world_solid_geom_frag.bin.h",   "frag", "gSPIRV_world_solid_geom_frag"  ],
    [ "world_solid_geom.vert",  "compiled/SPIRV_world_solid_geom_vert.bin.h",   "vert", "gSPIRV_world_solid_geom_vert"  ],
    [ "world_sprites.frag",     "compiled/SPIRV_world_sprites_frag.bin.h",      "frag", "gSPIRV_world_sprites_frag"     ],
    [ "world_sprites.vert",     "compiled/SPIRV_world_sprites_vert.bin.h",      "vert", "gSPIRV_world_sprites_vert"     ],
]

# Compiles the input GLSL file to an output file with the specified name.
# The SPIRV binary code is put into a uint32_t[] array of the specified name and marked as being for the specified shader stage.
def compile_shader(input_glsl_file, output_c_file, shader_stage, c_var_name):
    # Generate the basic shader code uint32_t array encased in '{}'
    result = subprocess.call(
        ["glslc", "-I", ".", "--target-env=vulkan1.0", "-fshader-stage=" + shader_stage, "-mfmt=c", "-O", "-o", output_c_file, input_glsl_file]
    )

    if result != 0:
        print("Compile FAILED for file: {0:s}!".format(input_glsl_file))
        sys.exit(1)

    # Read that shader code then write it back to the file in a properly named and terminated C array
    with open(output_c_file,'r') as file:
      shader_code = file.read()

    with open(output_c_file, "w") as file:
        file.write("static const uint32_t ")
        file.write(c_var_name)
        file.write("[] = \n")
        file.write(shader_code)
        file.write(";")

# Main script logic: compiles all of the shaders
def main():
    for job_spec in files_to_compile:
        compile_shader(job_spec[0], job_spec[1], job_spec[2], job_spec[3])

main()
