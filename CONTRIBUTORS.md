StationDoom contributors (sorted alphabetically)
============================================

* **[Erick Vásquez García (Erick194)](https://github.com/Erick194)**

    * Various information and code for PSX DOOM based on his own extensive reverse engineering efforts, including:
        * Z_Zone comparison:
            https://gist.github.com/Erick194/cccab97eb7cad64fb338fa1048a4743d
            https://gist.github.com/Erick194/2469d4bc1c04748b60e87235885c1977
        * P_LoadBlocks comparison:
            https://www.doomworld.com/forum/topic/111024-stationdoom-reverse-engineered-source-port-of-psx-doom-for-pc-early-progress/?page=1
        * 'vertex_t' fields and zone memory manager purge flags:
            https://github.com/BodbDearg/StationDoom/commit/a593db0ad7acb693749446e4a7e65756d8c01433
        * Password encoding/decoding:
            https://github.com/BodbDearg/StationDoom/commit/b92176e5755bb55f7a20783281b57b645f8252ca
        * Various enums and constants, 'seg_t' fields, 'buttons_t' fields, 'psxobj_t' fields, fire sky code:
            https://github.com/BodbDearg/StationDoom/commit/38ecd7bebb4ddf3c84002412cb3f83e3f42d45dd
        * P_CheckCheats, sfx numbers, automap control & drawing code:
            https://github.com/BodbDearg/StationDoom/commit/ef070d19c8cdbe808838be577c24ea674308d678
        * P_Base/P_Move naming suggestions, PM_PointOnLineSide, R_RenderSKY:
            https://github.com/BodbDearg/StationDoom/commit/b34c232fd833d5df23d652c6da6353424d62dc23
        * I/W_AddPrim comparison:
            https://github.com/BodbDearg/StationDoom/commit/c3968607c64b82f4165af347303bb84265e524a8
        * FixedMul/Div comparison:
            https://github.com/BodbDearg/StationDoom/commit/bba3955c2c04fba9199929e87de3b7834ce90f98
        * Pretty much the entire PSX renderer!
            https://gist.github.com/Erick194/96f360d99e905f085f0ee50c097d3b65
            https://gist.github.com/Erick194/0f57e686e57ec43f1067f254f2fbe79a
            https://gist.github.com/Erick194/e609690fe0250c5243cb4c705815dacf

* **[Samuel Villarreal (svkaiser)](https://github.com/svkaiser)**

    * Console DOOM reverse engineering, specs & tools:
    https://www.doomworld.com/forum/topic/38608-the-console-doom-hacking-project-console-specs
    * Doom64-EX source code (DOOM 64 was based on PSX DOOM, thus can serve as a reference point for it):
    https://github.com/svkaiser/Doom64EX