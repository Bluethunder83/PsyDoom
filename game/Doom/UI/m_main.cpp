#include "m_main.h"

#include "Doom/Base/i_crossfade.h"
#include "Doom/Base/i_main.h"
#include "Doom/Base/i_misc.h"
#include "Doom/Base/s_sound.h"
#include "Doom/d_main.h"
#include "Doom/Game/g_game.h"
#include "PsxVm/PsxVm.h"
#include "Wess/psxcd.h"

void RunMenu() noexcept {
loc_80035B24:
    sp -= 0x20;
    sw(s0, sp + 0x10);
    s0 = 0x800B0000;                                    // Result = 800B0000
    s0 -= 0x6F7C;                                       // Result = gPaletteClutId_Main (800A9084)
    sw(s1, sp + 0x14);
    s1 = 0x80090000;                                    // Result = 80090000
    s1 += 0x7A10;                                       // Result = gTexInfo_BACK[0] (80097A10)
    sw(s2, sp + 0x18);
    s2 = s1 + 0x40;                                     // Result = gTexInfo_DOOM[0] (80097A50)
    sw(ra, sp + 0x1C);
loc_80035B4C:
    a0 = 0x80030000;                                    // Result = 80030000
    a0 += 0x5C94;                                       // Result = M_Start (80035C94)
    a1 = 0x80030000;                                    // Result = 80030000
    a1 += 0x5E40;                                       // Result = M_Stop (80035E40)
    a2 = 0x80030000;                                    // Result = 80030000
    a2 += 0x5EC4;                                       // Result = M_Ticker (80035EC4)
    a3 = 0x80030000;                                    // Result = 80030000
    a3 += 0x6258;                                       // Result = M_Drawer (80036258)
    MiniLoop();
    v1 = 7;                                             // Result = 00000007
    {
        const bool bJump = (v0 == v1);
        v0 = 7;                                         // Result = 00000007
        if (bJump) goto loc_80035C78;
    }
    I_IncDrawnFrameCount();
    a0 = s1;                                            // Result = gTexInfo_BACK[0] (80097A10)
    a1 = 0;                                             // Result = 00000000
    a3 = lh(s0);                                        // Load from: gPaletteClutId_Main (800A9084)
    a2 = 0;                                             // Result = 00000000
    I_CacheAndDrawSprite();
    a0 = s2;                                            // Result = gTexInfo_DOOM[0] (80097A50)
    a1 = 0x4B;                                          // Result = 0000004B
    a3 = lh(s0 + 0x22);                                 // Load from: gPaletteClutId_Title (800A90A6)
    a2 = 0x14;                                          // Result = 00000014
    I_CacheAndDrawSprite();
    I_SubmitGpuCmds();
    I_DrawPresent();
    v0 = 0x80070000;                                    // Result = 80070000
    v0 = lw(v0 + 0x7604);                               // Load from: gStartGameType (80077604)
    a0 = s1 + 0x100;                                    // Result = gTexInfo_CONNECT[0] (80097B10)
    if (v0 == 0) goto loc_80035C4C;
    a1 = 0x36;                                          // Result = 00000036
    a3 = lh(s0);                                        // Load from: gPaletteClutId_Main (800A9084)
    a2 = 0x67;                                          // Result = 00000067
    I_DrawPlaque();
    I_NetSetup();
    I_IncDrawnFrameCount();
    a0 = s1;                                            // Result = gTexInfo_BACK[0] (80097A10)
    a1 = 0;                                             // Result = 00000000
    a3 = lh(s0);                                        // Load from: gPaletteClutId_Main (800A9084)
    a2 = 0;                                             // Result = 00000000
    I_CacheAndDrawSprite();
    a0 = s2;                                            // Result = gTexInfo_DOOM[0] (80097A50)
    a1 = 0x4B;                                          // Result = 0000004B
    a3 = lh(s0 + 0x22);                                 // Load from: gPaletteClutId_Title (800A90A6)
    a2 = 0x14;                                          // Result = 00000014
    I_CacheAndDrawSprite();
    I_SubmitGpuCmds();
    I_DrawPresent();
    a0 = 0;                                             // Result = 00000000
    a1 = 7;                                             // Result = 00000007
    S_StartSound();
    v0 = 0x80070000;                                    // Result = 80070000
    v0 = lw(v0 + 0x7C0C);                               // Load from: gbDidAbortGame (80077C0C)
    if (v0 != 0) goto loc_80035B4C;
loc_80035C4C:
    a0 = 0x80070000;                                    // Result = 80070000
    a0 = lw(a0 + 0x75FC);                               // Load from: gStartSkill (800775FC)
    a1 = 0x80070000;                                    // Result = 80070000
    a1 = lw(a1 + 0x7600);                               // Load from: gStartMapOrEpisode (80077600)
    a2 = 0x80070000;                                    // Result = 80070000
    a2 = lw(a2 + 0x7604);                               // Load from: gStartGameType (80077604)
    G_InitNew();
    G_RunGame();
    v0 = 0;                                             // Result = 00000000
loc_80035C78:
    ra = lw(sp + 0x1C);
    s2 = lw(sp + 0x18);
    s1 = lw(sp + 0x14);
    s0 = lw(sp + 0x10);
    sp += 0x20;
    return;
}

void M_Start() noexcept {
    sp -= 0x28;
    v0 = 1;                                             // Result = 00000001
    sw(ra, sp + 0x24);
    sw(s0, sp + 0x20);
    at = 0x80080000;                                    // Result = 80080000
    sw(0, at - 0x7FA4);                                 // Store to: gNetGame (8007805C)
    at = 0x80070000;                                    // Result = 80070000
    sw(0, at + 0x7618);                                 // Store to: gCurPlayerIndex (80077618)
    at = 0x80080000;                                    // Result = 80080000
    sw(v0, at - 0x7F54);                                // Store to: gbPlayerInGame[0] (800780AC)
    at = 0x80080000;                                    // Result = 80080000
    sw(0, at - 0x7F50);                                 // Store to: gbPlayerInGame[1] (800780B0)
    I_ResetTexCache();
    s0 = 0x80090000;                                    // Result = 80090000
    s0 += 0x7A90;                                       // Result = gTexInfo_LOADING[0] (80097A90)
    a0 = s0;                                            // Result = gTexInfo_LOADING[0] (80097A90)
    a1 = 0x80070000;                                    // Result = 80070000
    a1 += 0x7C4C;                                       // Result = STR_LumpName_LOADING[0] (80077C4C)
    a2 = 0;                                             // Result = 00000000
    I_CacheTexForLumpName();
    a0 = s0;                                            // Result = gTexInfo_LOADING[0] (80097A90)
    a1 = 0x5F;                                          // Result = 0000005F
    a3 = 0x800B0000;                                    // Result = 800B0000
    a3 = lh(a3 - 0x6F5C);                               // Load from: gPaletteClutId_UI (800A90A4)
    a2 = 0x6D;                                          // Result = 0000006D
    I_DrawPlaque();
    a0 = 0;                                             // Result = 00000000
    S_LoadSoundAndMusic();
    a0 = s0 - 0x80;                                     // Result = gTexInfo_BACK[0] (80097A10)
    a1 = 0x80070000;                                    // Result = 80070000
    a1 += 0x7C8C;                                       // Result = STR_LumpName_BACK[0] (80077C8C)
    a2 = 0;                                             // Result = 00000000
    I_CacheTexForLumpName();
    a0 = s0 - 0x40;                                     // Result = gTexInfo_DOOM[0] (80097A50)
    a1 = 0x80070000;                                    // Result = 80070000
    a1 += 0x7C94;                                       // Result = STR_LumpName_DOOM[0] (80077C94)
    a2 = 0;                                             // Result = 00000000
    I_CacheTexForLumpName();
    a0 = s0 + 0x80;                                     // Result = gTexInfo_CONNECT[0] (80097B10)
    a1 = 0x80070000;                                    // Result = 80070000
    a1 += 0x7C9C;                                       // Result = STR_LumpName_CONNECT[0] (80077C9C)
    a2 = 0;                                             // Result = 00000000
    I_CacheTexForLumpName();
    v0 = 0x80070000;                                    // Result = 80070000
    v0 = lw(v0 + 0x7604);                               // Load from: gStartGameType (80077604)
    sw(0, gp + 0xBF8);                                  // Store to: gCursorFrame (800781D8)
    sw(0, gp + 0xA20);                                  // Store to: gCursorPos (80078000)
    sw(0, gp + 0x918);                                  // Store to: gVBlanksUntilMenuMove (80077EF8)
    {
        const bool bJump = (v0 != 0);
        v0 = 0x36;                                      // Result = 00000036
        if (bJump) goto loc_80035D64;
    }
    v0 = 2;                                             // Result = 00000002
loc_80035D64:
    sw(v0, gp + 0xB9C);                                 // Store to: gMaxStartEpisodeOrMap (8007817C)
    v1 = 0x80070000;                                    // Result = 80070000
    v1 = lw(v1 + 0x7600);                               // Load from: gStartMapOrEpisode (80077600)
    v0 = lw(gp + 0xB9C);                                // Load from: gMaxStartEpisodeOrMap (8007817C)
    v0 = (i32(v0) < i32(v1));
    {
        const bool bJump = (v0 != 0);
        v0 = 1;                                         // Result = 00000001
        if (bJump) goto loc_80035D8C;
    }
    v0 = 2;                                             // Result = 00000002
    if (i32(v1) >= 0) goto loc_80035D94;
loc_80035D8C:
    at = 0x80070000;                                    // Result = 80070000
    sw(v0, at + 0x7600);                                // Store to: gStartMapOrEpisode (80077600)
loc_80035D94:
    v0 = 0x80070000;                                    // Result = 80070000
    v0 += 0x3E50;                                       // Result = CDTrackNum_MainMenu (80073E50)
    a0 = lw(v0);                                        // Load from: CDTrackNum_MainMenu (80073E50)
    a1 = 0x80070000;                                    // Result = 80070000
    a1 = lw(a1 + 0x75F8);                               // Load from: gCdMusicVol (800775F8)
    a2 = 0;                                             // Result = 00000000
    sw(0, sp + 0x18);
    sw(0, sp + 0x1C);
    v0 = lw(v0);                                        // Load from: CDTrackNum_MainMenu (80073E50)
    a3 = 0;                                             // Result = 00000000
    sw(v0, sp + 0x10);
    sw(a1, sp + 0x14);
    psxcd_play_at_andloop();
loc_80035DC8:
    psxcd_elapsed_sectors();
    if (v0 == 0) goto loc_80035DC8;
    v0 = 0x80080000;                                    // Result = 80080000
    v0 = lw(v0 - 0x7F08);                               // Load from: gCurDrawDispBufferIdx (800780F8)
    s0 = 0x800B0000;                                    // Result = 800B0000
    s0 -= 0x6F3C;                                       // Result = gDrawEnv1[C] (800A90C4)
    sb(0, s0);                                          // Store to: gDrawEnv1[C] (800A90C4)
    at = 0x800B0000;                                    // Result = 800B0000
    sb(0, at - 0x6EE0);                                 // Store to: gDrawEnv2[C] (800A9120)
    v0 ^= 1;
    at = 0x80080000;                                    // Result = 80080000
    sw(v0, at - 0x7F08);                                // Store to: gCurDrawDispBufferIdx (800780F8)
    M_Drawer();
    I_CrossFadeFrameBuffers();
    v1 = 0x80080000;                                    // Result = 80080000
    v1 = lw(v1 - 0x7EB4);                               // Load from: gTicCon (8007814C)
    v0 = 1;                                             // Result = 00000001
    sb(v0, s0);                                         // Store to: gDrawEnv1[C] (800A90C4)
    at = 0x800B0000;                                    // Result = 800B0000
    sb(v0, at - 0x6EE0);                                // Store to: gDrawEnv2[C] (800A9120)
    sw(v1, gp + 0x92C);                                 // Store to: gMenuTimeoutStartTicCon (80077F0C)
    ra = lw(sp + 0x24);
    s0 = lw(sp + 0x20);
    sp += 0x28;
    return;
}

void M_Stop() noexcept {
    sp -= 0x18;
    sw(s0, sp + 0x10);
    s0 = a0;
    a0 = 0;                                             // Result = 00000000
    sw(ra, sp + 0x14);
    a1 = 7;                                             // Result = 00000007
    S_StartSound();
    psxcd_stop();
    v0 = 9;                                             // Result = 00000009
    if (s0 != v0) goto loc_80035EB0;
    v0 = 0x80070000;                                    // Result = 80070000
    v0 = lw(v0 + 0x7604);                               // Load from: gStartGameType (80077604)
    {
        const bool bJump = (v0 != 0);
        v0 = 1;                                         // Result = 00000001
        if (bJump) goto loc_80035EB0;
    }
    v1 = 0x80070000;                                    // Result = 80070000
    v1 = lw(v1 + 0x7600);                               // Load from: gStartMapOrEpisode (80077600)
    {
        const bool bJump = (v1 != v0);
        v0 = 0x1F;                                      // Result = 0000001F
        if (bJump) goto loc_80035EA8;
    }
    at = 0x80070000;                                    // Result = 80070000
    sw(v1, at + 0x7600);                                // Store to: gStartMapOrEpisode (80077600)
    goto loc_80035EB0;
loc_80035EA8:
    at = 0x80070000;                                    // Result = 80070000
    sw(v0, at + 0x7600);                                // Store to: gStartMapOrEpisode (80077600)
loc_80035EB0:
    ra = lw(sp + 0x14);
    s0 = lw(sp + 0x10);
    sp += 0x18;
    return;
}

void M_Ticker() noexcept {
    sp -= 0x18;
    sw(s0, sp + 0x10);
    s0 = 0x80070000;                                    // Result = 80070000
    s0 = lw(s0 + 0x7F44);                               // Load from: gPlayerPadButtons[0] (80077F44)
    a0 = 0x80080000;                                    // Result = 80080000
    a0 = lw(a0 - 0x7DEC);                               // Load from: gPlayerOldPadButtons[0] (80078214)
    sw(ra, sp + 0x14);
    if (s0 == 0) goto loc_80035EF4;
    v0 = 0x80080000;                                    // Result = 80080000
    v0 = lw(v0 - 0x7EB4);                               // Load from: gTicCon (8007814C)
    sw(v0, gp + 0x92C);                                 // Store to: gMenuTimeoutStartTicCon (80077F0C)
loc_80035EF4:
    v0 = 0x80080000;                                    // Result = 80080000
    v0 = lw(v0 - 0x7EB4);                               // Load from: gTicCon (8007814C)
    v1 = lw(gp + 0x92C);                                // Load from: gMenuTimeoutStartTicCon (80077F0C)
    v0 -= v1;
    v0 = (i32(v0) < 0x708);
    {
        const bool bJump = (v0 == 0);
        v0 = 7;                                         // Result = 00000007
        if (bJump) goto loc_80036244;
    }
    v1 = 0x80080000;                                    // Result = 80080000
    v1 = lw(v1 - 0x7FB4);                               // Load from: gGameTic (8007804C)
    v0 = 0x80070000;                                    // Result = 80070000
    v0 = lw(v0 + 0x7FA4);                               // Load from: gPrevGameTic (80077FA4)
    v0 = (i32(v0) < i32(v1));
    {
        const bool bJump = (v0 == 0);
        v0 = v1 & 3;
        if (bJump) goto loc_80035F4C;
    }
    if (v0 != 0) goto loc_80035F4C;
    v0 = lw(gp + 0xBF8);                                // Load from: gCursorFrame (800781D8)
    v0 ^= 1;
    sw(v0, gp + 0xBF8);                                 // Store to: gCursorFrame (800781D8)
loc_80035F4C:
    v0 = s0 & 0x800;
    if (s0 == a0) goto loc_80035FCC;
    {
        const bool bJump = (v0 == 0);
        v0 = s0 & 0xF0;
        if (bJump) goto loc_80035F64;
    }
    v0 = 9;                                             // Result = 00000009
    goto loc_80036244;
loc_80035F64:
    if (v0 == 0) goto loc_80035FCC;
    v1 = lw(gp + 0xA20);                                // Load from: gCursorPos (80078000)
    v0 = (i32(v1) < 3);
    if (i32(v1) < 0) goto loc_80035FCC;
    {
        const bool bJump = (v0 != 0);
        v0 = 9;                                         // Result = 00000009
        if (bJump) goto loc_80036244;
    }
    v0 = 3;                                             // Result = 00000003
    {
        const bool bJump = (v1 != v0);
        v0 = s0 & 0xF000;
        if (bJump) goto loc_80035FD0;
    }
    a0 = 0x80040000;                                    // Result = 80040000
    a0 -= 0x16F0;                                       // Result = O_Init (8003E910)
    a1 = 0x80040000;                                    // Result = 80040000
    a1 -= 0x1630;                                       // Result = O_Shutdown (8003E9D0)
    a2 = 0x80040000;                                    // Result = 80040000
    a2 -= 0x160C;                                       // Result = O_Control (8003E9F4)
    a3 = 0x80040000;                                    // Result = 80040000
    a3 -= 0x1138;                                       // Result = O_Drawer (8003EEC8)
    MiniLoop();
    v1 = 4;                                             // Result = 00000004
    {
        const bool bJump = (v0 != v1);
        v0 = s0 & 0xF000;
        if (bJump) goto loc_80035FD0;
    }
    v0 = 4;                                             // Result = 00000004
    goto loc_80036244;
loc_80035FCC:
    v0 = s0 & 0xF000;
loc_80035FD0:
    {
        const bool bJump = (v0 != 0);
        v0 = 0;                                         // Result = 00000000
        if (bJump) goto loc_80035FE4;
    }
    sw(0, gp + 0x918);                                  // Store to: gVBlanksUntilMenuMove (80077EF8)
    goto loc_80036244;
loc_80035FE4:
    a0 = 0x80070000;                                    // Result = 80070000
    a0 += 0x7EF8;                                       // Result = gVBlanksUntilMenuMove (80077EF8)
    v0 = lw(a0);                                        // Load from: gVBlanksUntilMenuMove (80077EF8)
    v1 = 0x80070000;                                    // Result = 80070000
    v1 = lw(v1 + 0x7FBC);                               // Load from: gPlayersElapsedVBlanks[0] (80077FBC)
    v0 -= v1;
    sw(v0, a0);                                         // Store to: gVBlanksUntilMenuMove (80077EF8)
    if (i32(v0) > 0) goto loc_80036240;
    v0 = 0xF;                                           // Result = 0000000F
    sw(v0, a0);                                         // Store to: gVBlanksUntilMenuMove (80077EF8)
    v0 = s0 & 0x4000;
    v1 = 4;                                             // Result = 00000004
    if (v0 == 0) goto loc_80036040;
    a0 = 0x80080000;                                    // Result = 80080000
    a0 -= 0x8000;                                       // Result = gCursorPos (80078000)
    v0 = lw(a0);                                        // Load from: gCursorPos (80078000)
    v0++;
    sw(v0, a0);                                         // Store to: gCursorPos (80078000)
    if (v0 != v1) goto loc_80036070;
    sw(0, a0);                                          // Store to: gCursorPos (80078000)
    goto loc_80036070;
loc_80036040:
    v0 = s0 & 0x1000;
    v1 = -1;                                            // Result = FFFFFFFF
    if (v0 == 0) goto loc_8003607C;
    a0 = 0x80080000;                                    // Result = 80080000
    a0 -= 0x8000;                                       // Result = gCursorPos (80078000)
    v0 = lw(a0);                                        // Load from: gCursorPos (80078000)
    v0--;
    sw(v0, a0);                                         // Store to: gCursorPos (80078000)
    if (v0 != v1) goto loc_80036070;
    v0 = 3;                                             // Result = 00000003
    sw(v0, a0);                                         // Store to: gCursorPos (80078000)
loc_80036070:
    a0 = 0;                                             // Result = 00000000
    a1 = 0x12;                                          // Result = 00000012
    S_StartSound();
loc_8003607C:
    v1 = lw(gp + 0xA20);                                // Load from: gCursorPos (80078000)
    a0 = 1;                                             // Result = 00000001
    v0 = (i32(v1) < 2);
    if (v1 == a0) goto loc_80036178;
    if (v0 == 0) goto loc_800360A4;
    v0 = s0 & 0x2000;
    if (v1 == 0) goto loc_800360B8;
    v0 = 0;                                             // Result = 00000000
    goto loc_80036244;
loc_800360A4:
    v0 = 2;                                             // Result = 00000002
    {
        const bool bJump = (v1 == v0);
        v0 = s0 & 0x2000;
        if (bJump) goto loc_800361E8;
    }
    v0 = 0;                                             // Result = 00000000
    goto loc_80036244;
loc_800360B8:
    {
        const bool bJump = (v0 == 0);
        v0 = s0 & 0x8000;
        if (bJump) goto loc_800360F0;
    }
    v1 = 0x80070000;                                    // Result = 80070000
    v1 = lw(v1 + 0x7604);                               // Load from: gStartGameType (80077604)
    v0 = (v1 < 2);
    {
        const bool bJump = (v0 == 0);
        v0 = v1 + 1;
        if (bJump) goto loc_80036130;
    }
    at = 0x80070000;                                    // Result = 80070000
    sw(v0, at + 0x7604);                                // Store to: gStartGameType (80077604)
    if (v0 == a0) goto loc_8003611C;
    a0 = 0;                                             // Result = 00000000
    goto loc_80036128;
loc_800360F0:
    if (v0 == 0) goto loc_80036130;
    v0 = 0x80070000;                                    // Result = 80070000
    v0 = lw(v0 + 0x7604);                               // Load from: gStartGameType (80077604)
    {
        const bool bJump = (v0 == 0);
        v0--;
        if (bJump) goto loc_80036144;
    }
    at = 0x80070000;                                    // Result = 80070000
    sw(v0, at + 0x7604);                                // Store to: gStartGameType (80077604)
    if (v0 != 0) goto loc_80036124;
loc_8003611C:
    at = 0x80070000;                                    // Result = 80070000
    sw(a0, at + 0x7600);                                // Store to: gStartMapOrEpisode (80077600)
loc_80036124:
    a0 = 0;                                             // Result = 00000000
loc_80036128:
    a1 = 0x17;                                          // Result = 00000017
    S_StartSound();
loc_80036130:
    v0 = 0x80070000;                                    // Result = 80070000
    v0 = lw(v0 + 0x7604);                               // Load from: gStartGameType (80077604)
    {
        const bool bJump = (v0 != 0);
        v0 = 0x36;                                      // Result = 00000036
        if (bJump) goto loc_80036148;
    }
loc_80036144:
    v0 = 2;                                             // Result = 00000002
loc_80036148:
    sw(v0, gp + 0xB9C);                                 // Store to: gMaxStartEpisodeOrMap (8007817C)
    v1 = 0x80070000;                                    // Result = 80070000
    v1 = lw(v1 + 0x7600);                               // Load from: gStartMapOrEpisode (80077600)
    v0 = lw(gp + 0xB9C);                                // Load from: gMaxStartEpisodeOrMap (8007817C)
    v0 = (i32(v0) < i32(v1));
    {
        const bool bJump = (v0 == 0);
        v0 = 1;                                         // Result = 00000001
        if (bJump) goto loc_80036240;
    }
    at = 0x80070000;                                    // Result = 80070000
    sw(v0, at + 0x7600);                                // Store to: gStartMapOrEpisode (80077600)
    v0 = 0;                                             // Result = 00000000
    goto loc_80036244;
loc_80036178:
    v0 = s0 & 0x2000;
    {
        const bool bJump = (v0 == 0);
        v0 = s0 & 0x8000;
        if (bJump) goto loc_800361B0;
    }
    v0 = 0x80070000;                                    // Result = 80070000
    v0 = lw(v0 + 0x7600);                               // Load from: gStartMapOrEpisode (80077600)
    v1 = lw(gp + 0xB9C);                                // Load from: gMaxStartEpisodeOrMap (8007817C)
    v0++;
    at = 0x80070000;                                    // Result = 80070000
    sw(v0, at + 0x7600);                                // Store to: gStartMapOrEpisode (80077600)
    v0 = (i32(v1) < i32(v0));
    a0 = 0;                                             // Result = 00000000
    if (v0 != 0) goto loc_800361D8;
    goto loc_80036238;
loc_800361B0:
    {
        const bool bJump = (v0 == 0);
        v0 = 0;                                         // Result = 00000000
        if (bJump) goto loc_80036244;
    }
    v0 = 0x80070000;                                    // Result = 80070000
    v0 = lw(v0 + 0x7600);                               // Load from: gStartMapOrEpisode (80077600)
    v0--;
    at = 0x80070000;                                    // Result = 80070000
    sw(v0, at + 0x7600);                                // Store to: gStartMapOrEpisode (80077600)
    a0 = 0;                                             // Result = 00000000
    if (i32(v0) > 0) goto loc_80036238;
loc_800361D8:
    at = 0x80070000;                                    // Result = 80070000
    sw(v1, at + 0x7600);                                // Store to: gStartMapOrEpisode (80077600)
    v0 = 0;                                             // Result = 00000000
    goto loc_80036244;
loc_800361E8:
    {
        const bool bJump = (v0 == 0);
        v0 = s0 & 0x8000;
        if (bJump) goto loc_80036210;
    }
    v1 = 0x80070000;                                    // Result = 80070000
    v1 = lw(v1 + 0x75FC);                               // Load from: gStartSkill (800775FC)
    v0 = (v1 < 3);
    {
        const bool bJump = (v0 == 0);
        v0 = v1 + 1;
        if (bJump) goto loc_80036240;
    }
    goto loc_8003622C;
loc_80036210:
    {
        const bool bJump = (v0 == 0);
        v0 = 0;                                         // Result = 00000000
        if (bJump) goto loc_80036244;
    }
    v0 = 0x80070000;                                    // Result = 80070000
    v0 = lw(v0 + 0x75FC);                               // Load from: gStartSkill (800775FC)
    {
        const bool bJump = (v0 == 0);
        v0--;
        if (bJump) goto loc_80036240;
    }
loc_8003622C:
    at = 0x80070000;                                    // Result = 80070000
    sw(v0, at + 0x75FC);                                // Store to: gStartSkill (800775FC)
    a0 = 0;                                             // Result = 00000000
loc_80036238:
    a1 = 0x17;                                          // Result = 00000017
    S_StartSound();
loc_80036240:
    v0 = 0;                                             // Result = 00000000
loc_80036244:
    ra = lw(sp + 0x14);
    s0 = lw(sp + 0x10);
    sp += 0x18;
    return;
}

void M_Drawer() noexcept {
loc_80036258:
    sp -= 0x28;
    sw(ra, sp + 0x24);
    sw(s0, sp + 0x20);
    I_IncDrawnFrameCount();
    s0 = 0x80090000;                                    // Result = 80090000
    s0 += 0x7A10;                                       // Result = gTexInfo_BACK[0] (80097A10)
    a0 = s0;                                            // Result = gTexInfo_BACK[0] (80097A10)
    a1 = 0;                                             // Result = 00000000
    a3 = 0x800B0000;                                    // Result = 800B0000
    a3 = lh(a3 - 0x6F7C);                               // Load from: gPaletteClutId_Main (800A9084)
    a2 = 0;                                             // Result = 00000000
    I_CacheAndDrawSprite();
    a0 = s0 + 0x40;                                     // Result = gTexInfo_DOOM[0] (80097A50)
    a1 = 0x4B;                                          // Result = 0000004B
    a3 = 0x800B0000;                                    // Result = 800B0000
    a3 = lh(a3 - 0x6F5A);                               // Load from: gPaletteClutId_Title (800A90A6)
    a2 = 0x14;                                          // Result = 00000014
    I_CacheAndDrawSprite();
    a2 = 0x32;                                          // Result = 00000032
    a0 = 0x800B0000;                                    // Result = 800B0000
    a0 = lhu(a0 - 0x6B0E);                              // Load from: gTexInfo_STATUS[2] (800A94F2)
    a1 = 0x800B0000;                                    // Result = 800B0000
    a1 = lh(a1 - 0x6F5C);                               // Load from: gPaletteClutId_UI (800A90A4)
    v1 = lw(gp + 0xA20);                                // Load from: gCursorPos (80078000)
    v0 = lw(gp + 0xBF8);                                // Load from: gCursorFrame (800781D8)
    v1 <<= 1;
    v0 <<= 4;
    at = 0x80070000;                                    // Result = 80070000
    at += 0x7C34;                                       // Result = MainMenu_GameMode_YPos (80077C34)
    at += v1;
    a3 = lh(at);
    v0 += 0x84;
    sw(v0, sp + 0x10);
    v0 = 0xC0;                                          // Result = 000000C0
    sw(v0, sp + 0x14);
    v0 = 0x10;                                          // Result = 00000010
    sw(v0, sp + 0x18);
    v0 = 0x12;                                          // Result = 00000012
    sw(v0, sp + 0x1C);
    a3 -= 2;
    I_DrawSprite();
    a1 = lh(gp + 0x654);                                // Load from: MainMenu_GameMode_YPos (80077C34)
    a2 = 0x80010000;                                    // Result = 80010000
    a2 += 0x1568;                                       // Result = STR_GameMode[0] (80011568)
    a0 = 0x4A;                                          // Result = 0000004A
    I_DrawString();
    a0 = 0x5A;                                          // Result = 0000005A
    v0 = 0x80070000;                                    // Result = 80070000
    v0 += 0x3CDC;                                       // Result = STR_MenuOpt_SinglePlayer[0] (80073CDC)
    a1 = lh(gp + 0x654);                                // Load from: MainMenu_GameMode_YPos (80077C34)
    a2 = 0x80070000;                                    // Result = 80070000
    a2 = lw(a2 + 0x7604);                               // Load from: gStartGameType (80077604)
    a1 += 0x14;
    a2 <<= 4;
    a2 += v0;
    I_DrawString();
    v0 = 0x80070000;                                    // Result = 80070000
    v0 = lw(v0 + 0x7604);                               // Load from: gStartGameType (80077604)
    {
        const bool bJump = (v0 != 0);
        v0 = 1;                                         // Result = 00000001
        if (bJump) goto loc_80036398;
    }
    v1 = 0x80070000;                                    // Result = 80070000
    v1 = lw(v1 + 0x7600);                               // Load from: gStartMapOrEpisode (80077600)
    if (v1 != v0) goto loc_8003637C;
    a1 = lh(gp + 0x656);                                // Load from: MainMenu_Episode_YPos (80077C36)
    a2 = 0x80010000;                                    // Result = 80010000
    a2 += 0x1574;                                       // Result = STR_UltimateDoom[0] (80011574)
    a0 = 0x4A;                                          // Result = 0000004A
    I_DrawString();
    goto loc_800363D4;
loc_8003637C:
    a1 = lh(gp + 0x656);                                // Load from: MainMenu_Episode_YPos (80077C36)
    a2 = 0x80070000;                                    // Result = 80070000
    a2 += 0x7CA4;                                       // Result = STR_Doom2[0] (80077CA4)
    a0 = 0x4A;                                          // Result = 0000004A
    I_DrawString();
    goto loc_800363D4;
loc_80036398:
    a1 = lh(gp + 0x656);                                // Load from: MainMenu_Episode_YPos (80077C36)
    a2 = 0x80070000;                                    // Result = 80070000
    a2 += 0x7CAC;                                       // Result = STR_Level[0] (80077CAC)
    a0 = 0x4A;                                          // Result = 0000004A
    I_DrawString();
    a2 = 0x80070000;                                    // Result = 80070000
    a2 = lw(a2 + 0x7600);                               // Load from: gStartMapOrEpisode (80077600)
    v0 = (i32(a2) < 0xA);
    a0 = 0x88;                                          // Result = 00000088
    if (v0 != 0) goto loc_800363C8;
    a0 = 0x94;                                          // Result = 00000094
loc_800363C8:
    a1 = lh(gp + 0x656);                                // Load from: MainMenu_Episode_YPos (80077C36)
    I_DrawNumber();
loc_800363D4:
    a1 = lh(gp + 0x658);                                // Load from: MainMenu_Difficulty_YPos (80077C38)
    a2 = 0x80010000;                                    // Result = 80010000
    a2 += 0x1584;                                       // Result = STR_Difficulty[0] (80011584)
    a0 = 0x4A;                                          // Result = 0000004A
    I_DrawString();
    a0 = 0x5A;                                          // Result = 0000005A
    v0 = 0x80070000;                                    // Result = 80070000
    v0 += 0x3D0C;                                       // Result = STR_MenuOpt_IAmAWimp[0] (80073D0C)
    a1 = lh(gp + 0x658);                                // Load from: MainMenu_Difficulty_YPos (80077C38)
    a2 = 0x80070000;                                    // Result = 80070000
    a2 = lw(a2 + 0x75FC);                               // Load from: gStartSkill (800775FC)
    a1 += 0x14;
    a2 <<= 4;
    a2 += v0;
    I_DrawString();
    a1 = lh(gp + 0x65A);                                // Load from: MainMenu_Options_YPos (80077C3A)
    a2 = 0x80070000;                                    // Result = 80070000
    a2 += 0x7CB4;                                       // Result = STR_Options[0] (80077CB4)
    a0 = 0x4A;                                          // Result = 0000004A
    I_DrawString();
    I_SubmitGpuCmds();
    I_DrawPresent();
    ra = lw(sp + 0x24);
    s0 = lw(sp + 0x20);
    sp += 0x28;
    return;
}