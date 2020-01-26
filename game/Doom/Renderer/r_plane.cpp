#include "r_plane.h"

#include "Doom/Base/i_drawcmds.h"
#include "Doom/Base/i_main.h"
#include "Doom/Base/w_wad.h"
#include "PcPsx/Finally.h"
#include "PsxVm/PsxVm.h"
#include "PsyQ/LIBETC.h"
#include "PsyQ/LIBGPU.h"
#include "r_data.h"
#include "r_local.h"
#include "r_main.h"

//------------------------------------------------------------------------------------------------------------------------------------------
// Draw either the floor or ceiling flat for a subsector's leaf
//------------------------------------------------------------------------------------------------------------------------------------------
void R_DrawSubsectorFlat(leaf_t& leaf, const bool bIsCeiling) noexcept {
    // Get the texture for the flat
    const sector_t& drawsec = **gpCurDrawSector;
    const int32_t flatPicNum = (bIsCeiling) ? drawsec.ceilingpic : drawsec.floorpic;
    const int32_t flatTexNum = (*gpFlatTranslation)[flatPicNum];
    texture_t& tex = (*gpFlatTextures)[flatTexNum];

    // Upload the flat texture to VRAM if required.
    // This case will be triggered for animated flats like water & slime - only one frame will be in VRAM at a time.
    // Most normal flats however will already be uploaded to VRAM on level start.
    if (tex.uploadFrameNum == TEX_INVALID_UPLOAD_FRAME_NUM) {
        // Decompress the lump data to the temporary buffer if required
        const void* pLumpData;
        const bool bIsUncompressedLump = (*gpbIsUncompressedLump)[tex.lumpNum];

        if (bIsUncompressedLump) {
            pLumpData = (*gpLumpCache)[tex.lumpNum].get();
        } else {
            const void* pCompressedLumpData = (*gpLumpCache)[tex.lumpNum].get();
            decode(pCompressedLumpData, gTmpBuffer.get());
            pLumpData = gTmpBuffer.get();
        }

        // Load the decompressed texture to the required part of VRAM and mark as loaded 
        const RECT vramRect = getTextureVramRect(tex);
        LIBGPU_LoadImage(vramRect, (uint32_t*) pLumpData + 2);      // TODO: figure out what 8 bytes are being skipped        
        tex.uploadFrameNum = *gNumFramesDrawn;
    }

    // Setup the texture window so that repeating occurs.
    // Note: the PSX version hardcoded the flat size here to 64x64 - but I will use the actual texture size instead.
    // The behavior should be the same but this way is more flexible for potential modding.
    {
        RECT texWinRect;
        LIBGPU_setRECT(texWinRect, tex.texPageCoordX, tex.texPageCoordY, tex.width, tex.height);

        DR_TWIN* const pTexWinPrim = (DR_TWIN*) getScratchAddr(128);
        LIBGPU_SetTexWindow(*pTexWinPrim, texWinRect);
        I_AddPrim(pTexWinPrim);
    }
	
    // Get Z position of the plane (height) in viewspace
    fixed_t planeZ;

    if (bIsCeiling) {
        planeZ = drawsec.ceilingheight - *gViewZ;
    } else {
        planeZ = drawsec.floorheight - *gViewZ;
    }

    // Draw the horizontal spans of the leaf
    R_DrawFlatSpans(leaf, planeZ >> FRACBITS, tex);
}

void R_DrawFlatSpans(leaf_t& leaf, const fixed_t planeViewZ, const texture_t& tex) noexcept {
loc_8002E714:
    a0 = ptrToVmAddr(&leaf);
    a1 = planeViewZ;
    a2 = ptrToVmAddr(&tex);

    sp -= 0x60;
    v1 = a0 + 4;
    a3 = 0x1F800000;                                    // Result = 1F800000
    a3 += 0x280;                                        // Result = 1F800280
    t0 = a3 + 0x54;                                     // Result = 1F8002D4
    sw(fp, sp + 0x58);
    sw(s7, sp + 0x54);
    sw(s6, sp + 0x50);
    sw(s5, sp + 0x4C);
    sw(s4, sp + 0x48);
    sw(s3, sp + 0x44);
    sw(s2, sp + 0x40);
    sw(s1, sp + 0x3C);
    sw(s0, sp + 0x38);
    sw(a1, sp);
    t2 = lw(a0);
    t8 = 0;                                             // Result = 00000000
    if (i32(t2) <= 0) goto loc_8002E7B4;
    a0 = 0x63;                                          // Result = 00000063
loc_8002E764:
    v0 = lw(v1);
    v0 = lw(v0 + 0x14);
    sw(v0, a3);
    v0 = lw(v1);
    s4 = lw(sp);
    v0 = lw(v0 + 0x8);
    mult(s4, v0);
    t8++;
    a3 += 4;
    v1 += 8;
    v0 = lo;
    v0 = u32(i32(v0) >> 16);
    v0 = a0 - v0;
    sw(v0, t0);
    v0 = (i32(t8) < i32(t2));
    t0 += 4;
    if (v0 != 0) goto loc_8002E764;
loc_8002E7B4:
    t5 = 0xC8;                                          // Result = 000000C8
    t8 = 0;                                             // Result = 00000000
    sw(0, sp + 0x8);
    if (i32(t2) <= 0) goto loc_8002E918;
    t4 = 0x1F800000;                                    // Result = 1F800000
    t4 += 0x2D4;                                        // Result = 1F8002D4
    t6 = t4 - 0x54;                                     // Result = 1F800280
    t3 = t4;                                            // Result = 1F8002D4
loc_8002E7D4:
    v0 = t2 - 1;
    v1 = t8 + 1;
    if (t8 != v0) goto loc_8002E7E4;
    v1 = 0;                                             // Result = 00000000
loc_8002E7E4:
    v0 = v1 << 2;
    v0 += t4;
    a0 = lw(v0);
    v0 = lw(t3);
    {
        const bool bJump = (a0 == v0);
        v0 = (i32(v0) < i32(a0));
        if (bJump) goto loc_8002E908;
    }
    a0 = t8;
    if (v0 == 0) goto loc_8002E810;
    t1 = 1;                                             // Result = 00000001
    goto loc_8002E81C;
loc_8002E810:
    a0 = v1;
    v1 = t8;
    t1 = 0;                                             // Result = 00000000
loc_8002E81C:
    a0 <<= 2;
    v0 = a0 + t4;
    v1 <<= 2;
    a3 = lw(v0);
    v0 = v1 + t4;
    a0 += t6;
    v1 += t6;
    t0 = lw(v0);
    v0 = lw(a0);
    v1 = lw(v1);
    t7 = v0 << 16;
    a1 = v1 << 16;
    v1 = a1 - t7;
    v0 = t0 - a3;
    div(v1, v0);
    if (v0 != 0) goto loc_8002E864;
    _break(0x1C00);
loc_8002E864:
    at = -1;                                            // Result = FFFFFFFF
    {
        const bool bJump = (v0 != at);
        at = 0x80000000;                                // Result = 80000000
        if (bJump) goto loc_8002E87C;
    }
    if (v1 != at) goto loc_8002E87C;
    tge(zero, zero, 0x5D);
loc_8002E87C:
    s6 = lo;
    v0 = (i32(t0) < 0xC9);
    if (i32(a3) >= 0) goto loc_8002E89C;
    mult(a3, s6);
    a3 = 0;                                             // Result = 00000000
    v0 = lo;
    t7 -= v0;
    v0 = (i32(t0) < 0xC9);
loc_8002E89C:
    {
        const bool bJump = (v0 != 0);
        v0 = (i32(a3) < i32(t5));
        if (bJump) goto loc_8002E8A8;
    }
    t0 = 0xC8;                                          // Result = 000000C8
loc_8002E8A8:
    if (v0 == 0) goto loc_8002E8B4;
    t5 = a3;
loc_8002E8B4:
    s4 = lw(sp + 0x8);
    v0 = (i32(s4) < i32(t0));
    a0 = a3 << 3;
    if (v0 == 0) goto loc_8002E8CC;
    sw(t0, sp + 0x8);
loc_8002E8CC:
    v0 = t1 << 2;                                       // Result = 00000000
    v1 = 0x800A0000;                                    // Result = 800A0000
    v1 -= 0x7F30;                                       // Result = 800980D0
    v0 += v1;                                           // Result = 800980D0
    s3 = a0 + v0;
    v0 = (i32(a3) < i32(t0));
    if (v0 == 0) goto loc_8002E908;
loc_8002E8EC:
    v0 = u32(i32(t7) >> 16);
    sw(v0, s3);
    s3 += 8;
    a3++;
    v0 = (i32(a3) < i32(t0));
    t7 += s6;
    if (v0 != 0) goto loc_8002E8EC;
loc_8002E908:
    t8++;
    v0 = (i32(t8) < i32(t2));
    t3 += 4;
    if (v0 != 0) goto loc_8002E7D4;
loc_8002E918:
    v1 = t5 << 3;                                       // Result = 00000640
    v0 = 0x800A0000;                                    // Result = 800A0000
    v0 -= 0x7F30;                                       // Result = 800980D0
    s3 = v1 + v0;                                       // Result = 80098710
    v1 = *g3dViewPaletteClutId;
    s4 = lw(sp + 0x8);
    v0 = 7;                                             // Result = 00000007
    at = 0x1F800000;                                    // Result = 1F800000
    sb(v0, at + 0x203);                                 // Store to: 1F800203
    v0 = 0x24;                                          // Result = 00000024
    at = 0x1F800000;                                    // Result = 1F800000
    sb(v0, at + 0x207);                                 // Store to: 1F800207
    at = 0x1F800000;                                    // Result = 1F800000
    sh(v1, at + 0x20E);                                 // Store to: 1F80020E
    v0 = lhu(a2 + 0xA);
    s1 = t5;                                            // Result = 000000C8
    at = 0x1F800000;                                    // Result = 1F800000
    sh(v0, at + 0x216);                                 // Store to: 1F800216
    v0 = (i32(s1) < i32(s4));
    s2 = 0xFF0000;                                      // Result = 00FF0000
    if (v0 == 0) goto loc_8002F300;
    s2 |= 0xFFFF;                                       // Result = 00FFFFFF
    s4 = 0x80080000;                                    // Result = 80080000
    s4 += 0x6550;                                       // Result = gGpuCmdsBuffer[0] (80086550)
    s4 &= s2;                                           // Result = 00086550
    sw(s4, sp + 0x10);
    fp = -1;                                            // Result = FFFFFFFF
loc_8002E988:
    t7 = lw(s3);
    s3 += 4;
    a1 = lw(s3);
    s3 += 4;
    if (t7 == a1) goto loc_8002F2EC;
    v0 = (i32(a1) < i32(t7));
    {
        const bool bJump = (v0 == 0);
        v0 = t7;
        if (bJump) goto loc_8002E9B4;
    }
    t7 = a1;
    a1 = v0;
loc_8002E9B4:
    v0 = s1 << 2;
    at = 0x80070000;                                    // Result = 80070000
    at += 0x395C;                                       // Result = 8007395C
    at += v0;
    v0 = lw(at);
    s4 = lw(sp);
    mult(s4, v0);
    v0 = lo;
    a3 = *gViewCos;
    t0 = u32(i32(v0) >> 16);
    mult(t0, a3);
    a0 = lo;
    v0 = *gViewSin;
    mult(t0, v0);
    t7 -= 2;
    a1 += 2;
    v1 = *gViewY;
    v0 = lo;
    a2 = v0;
    v0 = *gViewX;
    t3 = a2 + v1;
    t2 = a0 + v0;
    if (i32(a2) >= 0) goto loc_8002EA2C;
    a2 += 0x7F;
loc_8002EA2C:
    v0 = -a3;
    mult(v0, t0);
    v0 = lo;
    s5 = u32(i32(a2) >> 7);
    if (i32(v0) >= 0) goto loc_8002EA44;
    v0 += 0x7F;
loc_8002EA44:
    a2 = t7 - 0x80;
    mult(a2, s5);
    a0 = lo;
    v0 = u32(i32(v0) >> 7);
    mult(a2, v0);
    v1 = lo;
    a2 = a1 - 0x80;
    mult(a2, s5);
    sw(v0, sp + 0x30);
    v0 = lo;
    s4 = lw(sp + 0x30);
    mult(a2, s4);
    a0 += t2;
    a0 = u32(i32(a0) >> 16);
    v1 += t3;
    v0 += t2;
    t4 = u32(i32(v0) >> 16);
    v0 = lo;
    v0 += t3;
    t5 = u32(i32(v0) >> 16);
    v0 = (i32(a0) < i32(t4));
    t1 = u32(i32(v1) >> 16);
    if (v0 == 0) goto loc_8002EAB8;
    s4 = -0x40;                                         // Result = FFFFFFC0
    t6 = a0 & s4;
    a0 -= t6;
    t4 -= t6;
    goto loc_8002EAC8;
loc_8002EAB8:
    s4 = -0x40;                                         // Result = FFFFFFC0
    t6 = t4 & s4;
    t4 -= t6;
    a0 -= t6;
loc_8002EAC8:
    v0 = (i32(t1) < i32(t5));
    s4 = -0x40;                                         // Result = FFFFFFC0
    if (v0 == 0) goto loc_8002EAE4;
    t3 = t1 & s4;
    t1 -= t3;
    t5 -= t3;
    goto loc_8002EAF0;
loc_8002EAE4:
    t3 = t5 & s4;
    t5 -= t3;
    t1 -= t3;
loc_8002EAF0:
    v0 = *gbDoViewLighting;
    {
        const bool bJump = (v0 == 0);
        v0 = u32(i32(t0) >> 1);
        if (bJump) goto loc_8002EBB4;
    }
    v1 = 0xA0;                                          // Result = 000000A0
    v1 -= v0;
    v0 = (i32(v1) < 0x40);
    {
        const bool bJump = (v0 == 0);
        v0 = (i32(v1) < 0xA1);
        if (bJump) goto loc_8002EB20;
    }
    v1 = 0x40;                                          // Result = 00000040
    goto loc_8002EB2C;
loc_8002EB20:
    if (v0 != 0) goto loc_8002EB2C;
    v1 = 0xA0;                                          // Result = 000000A0
loc_8002EB2C:
    v0 = *gCurLightValR;
    mult(v1, v0);
    v0 = lo;
    a3 = u32(i32(v0) >> 7);
    v0 = (i32(a3) < 0x100);
    if (v0 != 0) goto loc_8002EB54;
    a3 = 0xFF;                                          // Result = 000000FF
loc_8002EB54:
    v0 = 0x80080000;                                    // Result = 80080000
    v0 = lw(v0 - 0x7FCC);                               // Load from: gCurLightValG (80078034)
    mult(v1, v0);
    v0 = lo;
    a2 = u32(i32(v0) >> 7);
    v0 = (i32(a2) < 0x100);
    if (v0 != 0) goto loc_8002EB7C;
    a2 = 0xFF;                                          // Result = 000000FF
loc_8002EB7C:
    v0 = *gCurLightValB;
    mult(v1, v0);
    v0 = lo;
    v1 = u32(i32(v0) >> 7);
    v0 = (i32(v1) < 0x100);
    {
        const bool bJump = (v0 != 0);
        v0 = (i32(a0) < 0x100);
        if (bJump) goto loc_8002EBD0;
    }
    v1 = 0xFF;                                          // Result = 000000FF
    goto loc_8002EBD0;
loc_8002EBA8:
    v0 = t1 + 4;
    v0 += a0;
    goto loc_8002EE08;
loc_8002EBB4:
    a3 = *gCurLightValR;
    a2 = *gCurLightValG;
    v1 = *gCurLightValB;
    v0 = (i32(a0) < 0x100);
loc_8002EBD0:
    at = 0x1F800000;                                    // Result = 1F800000
    sb(a3, at + 0x204);                                 // Store to: 1F800204
    at = 0x1F800000;                                    // Result = 1F800000
    sb(a2, at + 0x205);                                 // Store to: 1F800205
    at = 0x1F800000;                                    // Result = 1F800000
    sb(v1, at + 0x206);                                 // Store to: 1F800206
    t9 = 0;                                             // Result = 00000000
    if (v0 == 0) goto loc_8002EC0C;
    v0 = (i32(t4) < 0x100);
    {
        const bool bJump = (v0 == 0);
        v0 = (i32(t1) < 0x100);
        if (bJump) goto loc_8002EC0C;
    }
    {
        const bool bJump = (v0 == 0);
        v0 = (i32(t5) < 0x100);
        if (bJump) goto loc_8002EC0C;
    }
    if (v0 != 0) goto loc_8002EC44;
loc_8002EC0C:
    v0 = t4 - a0;
    if (i32(v0) >= 0) goto loc_8002EC1C;
    v0 = -v0;
loc_8002EC1C:
    t9 = u32(i32(v0) >> 7);
    v0 = t5 - t1;
    if (i32(v0) >= 0) goto loc_8002EC30;
    v0 = -v0;
loc_8002EC30:
    v1 = u32(i32(v0) >> 7);
    v0 = (i32(t9) < i32(v1));
    if (v0 == 0) goto loc_8002EC44;
    t9 = v1;
loc_8002EC44:
    v0 = a1 - t7;
    if (t9 != 0) goto loc_8002EEEC;
    s4 = 0x1F800000;                                    // Result = 1F800000
    s4 += 0x200;                                        // Result = 1F800200
    t3 = s4 + 4;                                        // Result = 1F800204
    t2 = 0x1F800000;                                    // Result = 1F800000
    t2 = lbu(t2 + 0x203);                               // Load from: 1F800203
    t0 = 0x80070000;                                    // Result = 80070000
    t0 = lw(t0 + 0x7C18);                               // Load from: gpGpuPrimsEnd (80077C18)
    v0 = s1 + 1;
    at = 0x1F800000;                                    // Result = 1F800000
    sh(t7, at + 0x208);                                 // Store to: 1F800208
    at = 0x1F800000;                                    // Result = 1F800000
    sh(s1, at + 0x20A);                                 // Store to: 1F80020A
    at = 0x1F800000;                                    // Result = 1F800000
    sh(a1, at + 0x210);                                 // Store to: 1F800210
    at = 0x1F800000;                                    // Result = 1F800000
    sh(s1, at + 0x212);                                 // Store to: 1F800212
    at = 0x1F800000;                                    // Result = 1F800000
    sh(a1, at + 0x218);                                 // Store to: 1F800218
    at = 0x1F800000;                                    // Result = 1F800000
    sh(v0, at + 0x21A);                                 // Store to: 1F80021A
    at = 0x1F800000;                                    // Result = 1F800000
    sb(a0, at + 0x20C);                                 // Store to: 1F80020C
    at = 0x1F800000;                                    // Result = 1F800000
    sb(t1, at + 0x20D);                                 // Store to: 1F80020D
    at = 0x1F800000;                                    // Result = 1F800000
    sb(t4, at + 0x214);                                 // Store to: 1F800214
    at = 0x1F800000;                                    // Result = 1F800000
    sb(t5, at + 0x215);                                 // Store to: 1F800215
    at = 0x1F800000;                                    // Result = 1F800000
    sb(t4, at + 0x21C);                                 // Store to: 1F80021C
    at = 0x1F800000;                                    // Result = 1F800000
    sb(t5, at + 0x21D);                                 // Store to: 1F80021D
    t1 = t2 << 2;
    t4 = t1 + 4;
loc_8002ECD4:
    a0 = 0x80070000;                                    // Result = 80070000
    a0 = lw(a0 + 0x7C18);                               // Load from: gpGpuPrimsEnd (80077C18)
    v0 = 0x80070000;                                    // Result = 80070000
    v0 = lw(v0 + 0x7C14);                               // Load from: gpGpuPrimsBeg (80077C14)
    v0 = (a0 < v0);
    {
        const bool bJump = (v0 != 0);
        v0 = t1 + a0;
        if (bJump) goto loc_8002ED44;
    }
    v0 += 4;
    v1 = 0x80090000;                                    // Result = 80090000
    v1 += 0x6550;                                       // Result = gThinkerCap[0] (80096550)
    v0 = (v0 < v1);
    v1 = 0xFF000000;                                    // Result = FF000000
    if (v0 != 0) goto loc_8002EBA8;
    s4 = 0x80080000;                                    // Result = 80080000
    s4 += 0x6550;                                       // Result = gGpuCmdsBuffer[0] (80086550)
    v0 = lw(t0);
    at = 0x80070000;                                    // Result = 80070000
    sw(s4, at + 0x7C18);                                // Store to: gpGpuPrimsEnd (80077C18)
    s4 = lw(sp + 0x10);
    v0 &= v1;
    v0 |= s4;
    sw(v0, t0);
    sb(0, t0 + 0x3);
    t0 = 0x80070000;                                    // Result = 80070000
    t0 = lw(t0 + 0x7C18);                               // Load from: gpGpuPrimsEnd (80077C18)
    a0 = 0x80070000;                                    // Result = 80070000
    a0 = lw(a0 + 0x7C18);                               // Load from: gpGpuPrimsEnd (80077C18)
loc_8002ED44:
    v1 = 0x80070000;                                    // Result = 80070000
    v1 = lw(v1 + 0x7C14);                               // Load from: gpGpuPrimsBeg (80077C14)
    v0 = t1 + a0;
    v0 += 4;
    v0 = (v0 < v1);
    if (v0 != 0) goto loc_8002EDF8;
    if (v1 == a0) goto loc_8002ECD4;
loc_8002ED68:
    v0 = lw(gp + 0x5D8);                                // Load from: GPU_REG_GP1 (80077BB8)
    v0 = lw(v0);
    s4 = 0x4000000;                                     // Result = 04000000
    v0 &= s4;
    if (v0 == 0) goto loc_8002ECD4;
    a0 = 0x80070000;                                    // Result = 80070000
    a0 = lw(a0 + 0x7C14);                               // Load from: gpGpuPrimsBeg (80077C14)
    s4 = 0x80000000;                                    // Result = 80000000
    a2 = lbu(a0 + 0x3);
    v0 = lw(a0);
    a2--;
    v0 &= s2;
    v0 |= s4;
    at = 0x80070000;                                    // Result = 80070000
    sw(v0, at + 0x7C14);                                // Store to: gpGpuPrimsBeg (80077C14)
    a0 += 4;
    if (a2 == fp) goto loc_8002EDD4;
    a3 = -1;                                            // Result = FFFFFFFF
loc_8002EDB8:
    v1 = lw(a0);
    a0 += 4;
    v0 = lw(gp + 0x5D4);                                // Load from: GPU_REG_GP0 (80077BB4)
    a2--;
    sw(v1, v0);
    if (a2 != a3) goto loc_8002EDB8;
loc_8002EDD4:
    v1 = 0x80070000;                                    // Result = 80070000
    v1 = lw(v1 + 0x7C14);                               // Load from: gpGpuPrimsBeg (80077C14)
    v0 = 0x80070000;                                    // Result = 80070000
    v0 = lw(v0 + 0x7C18);                               // Load from: gpGpuPrimsEnd (80077C18)
    if (v1 == v0) goto loc_8002ECD4;
    goto loc_8002ED68;
loc_8002EDF8:
    v0 = 0x80070000;                                    // Result = 80070000
    v0 = lw(v0 + 0x7C18);                               // Load from: gpGpuPrimsEnd (80077C18)
    v0 += t4;
loc_8002EE08:
    at = 0x80070000;                                    // Result = 80070000
    sw(v0, at + 0x7C18);                                // Store to: gpGpuPrimsEnd (80077C18)
    a0 = 0xFF000000;                                    // Result = FF000000
    v1 = lw(t0);
    v0 = 0x80070000;                                    // Result = 80070000
    v0 = lw(v0 + 0x7C18);                               // Load from: gpGpuPrimsEnd (80077C18)
    v1 &= a0;
    v0 &= s2;
    v1 |= v0;
    sw(v1, t0);
    sb(t2, t0 + 0x3);
    t2--;
    t0 += 4;
    if (t2 == fp) goto loc_8002EE5C;
    v1 = -1;                                            // Result = FFFFFFFF
loc_8002EE44:
    v0 = lw(t3);
    t3 += 4;
    t2--;
    sw(v0, t0);
    t0 += 4;
    if (t2 != v1) goto loc_8002EE44;
loc_8002EE5C:
    v1 = 0x80070000;                                    // Result = 80070000
    v1 = lw(v1 + 0x7C14);                               // Load from: gpGpuPrimsBeg (80077C14)
    v0 = 0x80070000;                                    // Result = 80070000
    v0 = lw(v0 + 0x7C18);                               // Load from: gpGpuPrimsEnd (80077C18)
    if (v1 == v0) goto loc_8002F2EC;
    v0 = lw(gp + 0x5D8);                                // Load from: GPU_REG_GP1 (80077BB8)
    v0 = lw(v0);
    s4 = 0x4000000;                                     // Result = 04000000
    v0 &= s4;
    s4 = 0x80000000;                                    // Result = 80000000
    if (v0 == 0) goto loc_8002F2EC;
    a0 = 0x80070000;                                    // Result = 80070000
    a0 = lw(a0 + 0x7C14);                               // Load from: gpGpuPrimsBeg (80077C14)
    a2 = lbu(a0 + 0x3);
    v0 = lw(a0);
    a2--;
    v0 &= s2;
    v0 |= s4;
    at = 0x80070000;                                    // Result = 80070000
    sw(v0, at + 0x7C14);                                // Store to: gpGpuPrimsBeg (80077C14)
    a0 += 4;
    if (a2 == fp) goto loc_8002EE5C;
    a3 = -1;                                            // Result = FFFFFFFF
loc_8002EEC8:
    v1 = lw(a0);
    a0 += 4;
    v0 = lw(gp + 0x5D4);                                // Load from: GPU_REG_GP0 (80077BB4)
    a2--;
    sw(v1, v0);
    if (a2 == a3) goto loc_8002EE5C;
    goto loc_8002EEC8;
loc_8002EEEC:
    t9++;
    div(v0, t9);
    if (t9 != 0) goto loc_8002EF00;
    _break(0x1C00);
loc_8002EF00:
    at = -1;                                            // Result = FFFFFFFF
    {
        const bool bJump = (t9 != at);
        at = 0x80000000;                                // Result = 80000000
        if (bJump) goto loc_8002EF18;
    }
    if (v0 != at) goto loc_8002EF18;
    tge(zero, zero, 0x5D);
loc_8002EF18:
    s6 = lo;
    v0 = t4 - a0;
    div(v0, t9);
    if (t9 != 0) goto loc_8002EF30;
    _break(0x1C00);
loc_8002EF30:
    at = -1;                                            // Result = FFFFFFFF
    {
        const bool bJump = (t9 != at);
        at = 0x80000000;                                // Result = 80000000
        if (bJump) goto loc_8002EF48;
    }
    if (v0 != at) goto loc_8002EF48;
    tge(zero, zero, 0x5D);
loc_8002EF48:
    s5 = lo;
    v0 = t5 - t1;
    div(v0, t9);
    if (t9 != 0) goto loc_8002EF60;
    _break(0x1C00);
loc_8002EF60:
    at = -1;                                            // Result = FFFFFFFF
    {
        const bool bJump = (t9 != at);
        at = 0x80000000;                                // Result = 80000000
        if (bJump) goto loc_8002EF78;
    }
    if (v0 != at) goto loc_8002EF78;
    tge(zero, zero, 0x5D);
loc_8002EF78:
    s4 = lo;
    t8 = 0;                                             // Result = 00000000
    sw(s4, sp + 0x30);
    if (i32(t9) <= 0) goto loc_8002F2EC;
    s7 = -0x80;                                         // Result = FFFFFF80
loc_8002EF8C:
    a1 = t7 + s6;
    t4 = a0 + s5;
    s4 = lw(sp + 0x30);
    t6 = 0;                                             // Result = 00000000
    v0 = (i32(a0) < i32(t4));
    t5 = t1 + s4;
    if (v0 == 0) goto loc_8002EFC4;
    v0 = (i32(t4) < 0x100);
    {
        const bool bJump = (v0 != 0);
        v0 = (i32(t4) < i32(a0));
        if (bJump) goto loc_8002EFC8;
    }
    t6 = a0 & s7;
    a0 -= t6;
    t4 -= t6;
    goto loc_8002EFE4;
loc_8002EFC4:
    v0 = (i32(t4) < i32(a0));
loc_8002EFC8:
    {
        const bool bJump = (v0 == 0);
        v0 = (i32(a0) < 0x100);
        if (bJump) goto loc_8002EFE4;
    }
    {
        const bool bJump = (v0 != 0);
        v0 = (i32(t1) < i32(t5));
        if (bJump) goto loc_8002EFE8;
    }
    t6 = t4 & s7;
    t4 -= t6;
    a0 -= t6;
loc_8002EFE4:
    v0 = (i32(t1) < i32(t5));
loc_8002EFE8:
    t3 = 0;                                             // Result = 00000000
    if (v0 == 0) goto loc_8002F018;
    v0 = (i32(t5) < 0x100);
    {
        const bool bJump = (v0 != 0);
        v0 = (i32(t5) < i32(t1));
        if (bJump) goto loc_8002F01C;
    }
    t3 = t1 & s7;
    t1 -= t3;
    t5 -= t3;
    goto loc_8002F038;
loc_8002F00C:
    v0 = t1 + 4;
    v0 += a0;
    goto loc_8002F1F4;
loc_8002F018:
    v0 = (i32(t5) < i32(t1));
loc_8002F01C:
    {
        const bool bJump = (v0 == 0);
        v0 = (i32(t1) < 0x100);
        if (bJump) goto loc_8002F038;
    }
    if (v0 != 0) goto loc_8002F038;
    t3 = t5 & s7;
    t5 -= t3;
    t1 -= t3;
loc_8002F038:
    s4 = 0x1F800000;                                    // Result = 1F800000
    s4 += 0x200;                                        // Result = 1F800200
    s0 = s4 + 4;                                        // Result = 1F800204
    t2 = 0x1F800000;                                    // Result = 1F800000
    t2 = lbu(t2 + 0x203);                               // Load from: 1F800203
    a3 = 0x80070000;                                    // Result = 80070000
    a3 = lw(a3 + 0x7C18);                               // Load from: gpGpuPrimsEnd (80077C18)
    v0 = s1 + 1;
    at = 0x1F800000;                                    // Result = 1F800000
    sh(t7, at + 0x208);                                 // Store to: 1F800208
    at = 0x1F800000;                                    // Result = 1F800000
    sh(s1, at + 0x20A);                                 // Store to: 1F80020A
    at = 0x1F800000;                                    // Result = 1F800000
    sh(a1, at + 0x210);                                 // Store to: 1F800210
    at = 0x1F800000;                                    // Result = 1F800000
    sh(s1, at + 0x212);                                 // Store to: 1F800212
    at = 0x1F800000;                                    // Result = 1F800000
    sh(a1, at + 0x218);                                 // Store to: 1F800218
    at = 0x1F800000;                                    // Result = 1F800000
    sh(v0, at + 0x21A);                                 // Store to: 1F80021A
    at = 0x1F800000;                                    // Result = 1F800000
    sb(a0, at + 0x20C);                                 // Store to: 1F80020C
    at = 0x1F800000;                                    // Result = 1F800000
    sb(t1, at + 0x20D);                                 // Store to: 1F80020D
    at = 0x1F800000;                                    // Result = 1F800000
    sb(t4, at + 0x214);                                 // Store to: 1F800214
    at = 0x1F800000;                                    // Result = 1F800000
    sb(t5, at + 0x215);                                 // Store to: 1F800215
    at = 0x1F800000;                                    // Result = 1F800000
    sb(t4, at + 0x21C);                                 // Store to: 1F80021C
    at = 0x1F800000;                                    // Result = 1F800000
    sb(t5, at + 0x21D);                                 // Store to: 1F80021D
    t1 = t2 << 2;
    t7 = t1 + 4;
loc_8002F0C0:
    a0 = 0x80070000;                                    // Result = 80070000
    a0 = lw(a0 + 0x7C18);                               // Load from: gpGpuPrimsEnd (80077C18)
    v0 = 0x80070000;                                    // Result = 80070000
    v0 = lw(v0 + 0x7C14);                               // Load from: gpGpuPrimsBeg (80077C14)
    v0 = (a0 < v0);
    {
        const bool bJump = (v0 != 0);
        v0 = t1 + a0;
        if (bJump) goto loc_8002F130;
    }
    v0 += 4;
    v1 = 0x80090000;                                    // Result = 80090000
    v1 += 0x6550;                                       // Result = gThinkerCap[0] (80096550)
    v0 = (v0 < v1);
    v1 = 0xFF000000;                                    // Result = FF000000
    if (v0 != 0) goto loc_8002F00C;
    s4 = 0x80080000;                                    // Result = 80080000
    s4 += 0x6550;                                       // Result = gGpuCmdsBuffer[0] (80086550)
    v0 = lw(a3);
    at = 0x80070000;                                    // Result = 80070000
    sw(s4, at + 0x7C18);                                // Store to: gpGpuPrimsEnd (80077C18)
    s4 = lw(sp + 0x10);
    v0 &= v1;
    v0 |= s4;
    sw(v0, a3);
    sb(0, a3 + 0x3);
    a3 = 0x80070000;                                    // Result = 80070000
    a3 = lw(a3 + 0x7C18);                               // Load from: gpGpuPrimsEnd (80077C18)
    a0 = 0x80070000;                                    // Result = 80070000
    a0 = lw(a0 + 0x7C18);                               // Load from: gpGpuPrimsEnd (80077C18)
loc_8002F130:
    v1 = 0x80070000;                                    // Result = 80070000
    v1 = lw(v1 + 0x7C14);                               // Load from: gpGpuPrimsBeg (80077C14)
    v0 = t1 + a0;
    v0 += 4;
    v0 = (v0 < v1);
    if (v0 != 0) goto loc_8002F1E4;
    if (v1 == a0) goto loc_8002F0C0;
loc_8002F154:
    v0 = lw(gp + 0x5D8);                                // Load from: GPU_REG_GP1 (80077BB8)
    v0 = lw(v0);
    s4 = 0x4000000;                                     // Result = 04000000
    v0 &= s4;
    if (v0 == 0) goto loc_8002F0C0;
    a0 = 0x80070000;                                    // Result = 80070000
    a0 = lw(a0 + 0x7C14);                               // Load from: gpGpuPrimsBeg (80077C14)
    s4 = 0x80000000;                                    // Result = 80000000
    a2 = lbu(a0 + 0x3);
    v0 = lw(a0);
    a2--;
    v0 &= s2;
    v0 |= s4;
    at = 0x80070000;                                    // Result = 80070000
    sw(v0, at + 0x7C14);                                // Store to: gpGpuPrimsBeg (80077C14)
    a0 += 4;
    if (a2 == fp) goto loc_8002F1C0;
    t0 = -1;                                            // Result = FFFFFFFF
loc_8002F1A4:
    v1 = lw(a0);
    a0 += 4;
    v0 = lw(gp + 0x5D4);                                // Load from: GPU_REG_GP0 (80077BB4)
    a2--;
    sw(v1, v0);
    if (a2 != t0) goto loc_8002F1A4;
loc_8002F1C0:
    v1 = 0x80070000;                                    // Result = 80070000
    v1 = lw(v1 + 0x7C14);                               // Load from: gpGpuPrimsBeg (80077C14)
    v0 = 0x80070000;                                    // Result = 80070000
    v0 = lw(v0 + 0x7C18);                               // Load from: gpGpuPrimsEnd (80077C18)
    if (v1 == v0) goto loc_8002F0C0;
    goto loc_8002F154;
loc_8002F1E4:
    v0 = 0x80070000;                                    // Result = 80070000
    v0 = lw(v0 + 0x7C18);                               // Load from: gpGpuPrimsEnd (80077C18)
    v0 += t7;
loc_8002F1F4:
    at = 0x80070000;                                    // Result = 80070000
    sw(v0, at + 0x7C18);                                // Store to: gpGpuPrimsEnd (80077C18)
    v0 = 0xFF000000;                                    // Result = FF000000
    v1 = lw(a3);
    a0 = 0x80070000;                                    // Result = 80070000
    a0 = lw(a0 + 0x7C18);                               // Load from: gpGpuPrimsEnd (80077C18)
    v1 &= v0;
    a0 &= s2;
    v1 |= a0;
    sw(v1, a3);
    sb(t2, a3 + 0x3);
    t2--;
    a3 += 4;
    if (t2 == fp) goto loc_8002F2BC;
    v1 = -1;                                            // Result = FFFFFFFF
loc_8002F230:
    v0 = lw(s0);
    s0 += 4;
    t2--;
    sw(v0, a3);
    a3 += 4;
    if (t2 != v1) goto loc_8002F230;
    goto loc_8002F2BC;
loc_8002F250:
    v0 = lw(gp + 0x5D8);                                // Load from: GPU_REG_GP1 (80077BB8)
    v0 = lw(v0);
    s4 = 0x4000000;                                     // Result = 04000000
    v0 &= s4;
    t7 = a1;
    if (v0 == 0) goto loc_8002F2D8;
    a0 = 0x80070000;                                    // Result = 80070000
    a0 = lw(a0 + 0x7C14);                               // Load from: gpGpuPrimsBeg (80077C14)
    s4 = 0x80000000;                                    // Result = 80000000
    a2 = lbu(a0 + 0x3);
    v0 = lw(a0);
    a2--;
    v0 &= s2;
    v0 |= s4;
    at = 0x80070000;                                    // Result = 80070000
    sw(v0, at + 0x7C14);                                // Store to: gpGpuPrimsBeg (80077C14)
    a0 += 4;
    if (a2 == fp) goto loc_8002F2BC;
    a3 = -1;                                            // Result = FFFFFFFF
loc_8002F2A0:
    v1 = lw(a0);
    a0 += 4;
    v0 = lw(gp + 0x5D4);                                // Load from: GPU_REG_GP0 (80077BB4)
    a2--;
    sw(v1, v0);
    if (a2 != a3) goto loc_8002F2A0;
loc_8002F2BC:
    v1 = 0x80070000;                                    // Result = 80070000
    v1 = lw(v1 + 0x7C14);                               // Load from: gpGpuPrimsBeg (80077C14)
    v0 = 0x80070000;                                    // Result = 80070000
    v0 = lw(v0 + 0x7C18);                               // Load from: gpGpuPrimsEnd (80077C18)
    t7 = a1;
    if (v1 != v0) goto loc_8002F250;
loc_8002F2D8:
    a0 = t4 + t6;
    t8++;
    v0 = (i32(t8) < i32(t9));
    t1 = t5 + t3;
    if (v0 != 0) goto loc_8002EF8C;
loc_8002F2EC:
    s4 = lw(sp + 0x8);
    s1++;
    v0 = (i32(s1) < i32(s4));
    if (v0 != 0) goto loc_8002E988;
loc_8002F300:
    fp = lw(sp + 0x58);
    s7 = lw(sp + 0x54);
    s6 = lw(sp + 0x50);
    s5 = lw(sp + 0x4C);
    s4 = lw(sp + 0x48);
    s3 = lw(sp + 0x44);
    s2 = lw(sp + 0x40);
    s1 = lw(sp + 0x3C);
    s0 = lw(sp + 0x38);
    sp += 0x60;
    return;
}
