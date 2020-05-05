#include "p_move.h"

#include "Doom/Renderer/r_local.h"
#include "Doom/Renderer/r_main.h"
#include "doomdata.h"
#include "p_local.h"
#include "p_map.h"
#include "p_maputl.h"
#include "p_setup.h"
#include "p_spec.h"
#include "PsxVm/PsxVm.h"
#include <algorithm>

static constexpr int32_t MAX_CROSS_LINES = 8;

static const VmPtr<VmPtr<subsector_t>>              gpNewSubsec(0x800782BC);            // Destination subsector for the current move: set by 'PM_CheckPosition'
static const VmPtr<uint32_t>                        gTmFlags(0x80078078);               // Flags for the thing being moved
static const VmPtr<fixed_t[4]>                      gTestTmBBox(0x80097C10);            // Bounding box for the current thing being collision tested. Set in 'PM_CheckPosition'.
static const VmPtr<VmPtr<mobj_t>>                   gpMoveThing(0x800782C4);            // The thing collided with (for code doing interactions with the thing)
static const VmPtr<VmPtr<line_t>>                   gpBlockLine(0x80078248);            // The line collided with
static const VmPtr<fixed_t>                         gTmCeilingZ(0x80077F04);            // The Z value for the lowest ceiling the collider is in contact with
static const VmPtr<fixed_t>                         gTmFloorZ(0x800781E8);              // The Z value for the highest floor the collider is in contact with
static const VmPtr<fixed_t>                         gTmDropoffZ(0x80077F3C);            // The Z value for the lowest floor the collider is in contact with. Used by monsters so they don't walk off cliffs.
static const VmPtr<int32_t>                         gNumCrossCheckLines(0x800780C0);    // How many lines to test for whether the thing crossed them or not: for determining when to trigger line specials
static const VmPtr<VmPtr<line_t>[MAX_CROSS_LINES]>  gpCrossCheckLines(0x800A8F28);      // Lines to test for whether the thing crossed them or not: for determining when to trigger line specials

// Not required externally: making private to this module
static void PM_UnsetThingPosition(mobj_t& thing) noexcept;
static void PM_SetThingPosition(mobj_t& mobj) noexcept;
static bool PM_BlockLinesIterator(const int32_t x, const int32_t y) noexcept;
static bool PM_BlockThingsIterator(const int32_t x, const int32_t y) noexcept;

void P_TryMove2() noexcept {
loc_8001E4F4:
    v0 = 0x80080000;                                    // Result = 80080000
    v0 = lw(v0 - 0x7F74);                               // Load from: gpTryMoveThing (8007808C)
    sp -= 0x20;
    sw(ra, sp + 0x1C);
    sw(s2, sp + 0x18);
    sw(s1, sp + 0x14);
    sw(s0, sp + 0x10);
    v1 = lw(v0);
    v0 = lw(v0 + 0x4);
    sw(0, gp + 0xB5C);                                  // Store to: gbTryMove2 (8007813C)
    sw(0, gp + 0xA9C);                                  // Store to: gbFloatOk (8007807C)
    sw(v1, gp + 0xC60);                                 // Store to: gOldX (80078240)
    sw(v0, gp + 0xC64);                                 // Store to: gOldY (80078244)
    PM_CheckPosition();
    v0 = 0x80080000;                                    // Result = 80080000
    v0 = lw(v0 - 0x7F18);                               // Load from: gbCheckPosOnly (800780E8)
    if (v0 == 0) goto loc_8001E554;
    at = 0x80080000;                                    // Result = 80080000
    sw(0, at - 0x7F18);                                 // Store to: gbCheckPosOnly (800780E8)
    goto loc_8001E704;
loc_8001E554:
    v0 = lw(gp + 0xB5C);                                // Load from: gbTryMove2 (8007813C)
    if (v0 == 0) goto loc_8001E704;
    a0 = 0x80080000;                                    // Result = 80080000
    a0 = lw(a0 - 0x7F74);                               // Load from: gpTryMoveThing (8007808C)
    v0 = lw(a0 + 0x64);
    v0 &= 0x1000;
    if (v0 != 0) goto loc_8001E608;
    a3 = lw(gp + 0x924);                                // Load from: gTmCeilingZ (80077F04)
    a2 = lw(gp + 0xC08);                                // Load from: gTmFloorZ (800781E8)
    v1 = lw(a0 + 0x44);
    sw(0, gp + 0xB5C);                                  // Store to: gbTryMove2 (8007813C)
    v0 = a3 - a2;
    v0 = (i32(v0) < i32(v1));
    {
        const bool bJump = (v0 != 0);
        v0 = 1;                                         // Result = 00000001
        if (bJump) goto loc_8001E704;
    }
    t0 = lw(a0 + 0x64);
    sw(v0, gp + 0xA9C);                                 // Store to: gbFloatOk (8007807C)
    v0 = t0 & 0x8000;
    {
        const bool bJump = (v0 != 0);
        v0 = t0 & 0x4400;
        if (bJump) goto loc_8001E5E0;
    }
    a1 = lw(a0 + 0x8);
    v1 = lw(a0 + 0x44);
    v0 = a3 - a1;
    v0 = (i32(v0) < i32(v1));
    v1 = a2 - a1;
    if (v0 != 0) goto loc_8001E704;
    v0 = 0x180000;                                      // Result = 00180000
    v0 = (i32(v0) < i32(v1));
    {
        const bool bJump = (v0 != 0);
        v0 = t0 & 0x4400;
        if (bJump) goto loc_8001E704;
    }
loc_8001E5E0:
    {
        const bool bJump = (v0 != 0);
        v0 = 0x180000;                                  // Result = 00180000
        if (bJump) goto loc_8001E600;
    }
    v1 = lw(gp + 0x95C);                                // Load from: gTmDropoffZ (80077F3C)
    v1 = a2 - v1;
    v0 = (i32(v0) < i32(v1));
    if (v0 != 0) goto loc_8001E704;
loc_8001E600:
    a0 = 0x80080000;                                    // Result = 80080000
    a0 = lw(a0 - 0x7F74);                               // Load from: gpTryMoveThing (8007808C)
loc_8001E608:
    PM_UnsetThingPosition(*vmAddrToPtr<mobj_t>(a0));

    a0 = 0x80080000;                                    // Result = 80080000
    a0 = lw(a0 - 0x7F74);                               // Load from: gpTryMoveThing (8007808C)
    v0 = lw(gp + 0xC08);                                // Load from: gTmFloorZ (800781E8)
    v1 = lw(gp + 0x924);                                // Load from: gTmCeilingZ (80077F04)
    a1 = 0x80080000;                                    // Result = 80080000
    a1 = lw(a1 - 0x7EB0);                               // Load from: gTryMoveX (80078150)
    a2 = 0x80080000;                                    // Result = 80080000
    a2 = lw(a2 - 0x7EAC);                               // Load from: gTryMoveY (80078154)
    sw(v0, a0 + 0x38);
    sw(v1, a0 + 0x3C);
    sw(a1, a0);
    sw(a2, a0 + 0x4);
    PM_SetThingPosition(*vmAddrToPtr<mobj_t>(a0));
    v1 = 0x80080000;                                    // Result = 80080000
    v1 = lw(v1 - 0x7F74);                               // Load from: gpTryMoveThing (8007808C)
    v0 = lw(v1 + 0x80);
    {
        const bool bJump = (v0 != 0);
        v0 = 1;                                         // Result = 00000001
        if (bJump) goto loc_8001E700;
    }
    v0 = lw(v1 + 0x64);
    v0 &= 0x9000;
    {
        const bool bJump = (v0 != 0);
        v0 = 1;                                         // Result = 00000001
        if (bJump) goto loc_8001E700;
    }
    v0 = lw(gp + 0xAE0);                                // Load from: gNumCrossCheckLines (800780C0)
    v1 = -1;                                            // Result = FFFFFFFF
    v0--;
    sw(v0, gp + 0xAE0);                                 // Store to: gNumCrossCheckLines (800780C0)
    {
        const bool bJump = (v0 == v1);
        v0 = 1;                                         // Result = 00000001
        if (bJump) goto loc_8001E700;
    }
    s2 = 0x800B0000;                                    // Result = 800B0000
    s2 -= 0x70D8;                                       // Result = gpCrossCheckLines[0] (800A8F28)
loc_8001E694:
    v1 = lw(gp + 0xAE0);                                // Load from: gNumCrossCheckLines (800780C0)
    v0 = 0x80080000;                                    // Result = 80080000
    v0 = lw(v0 - 0x7F74);                               // Load from: gpTryMoveThing (8007808C)
    v1 <<= 2;
    v1 += s2;
    a0 = lw(v0);
    s1 = lw(v1);
    a1 = lw(v0 + 0x4);
    a2 = s1;
    v0 = P_PointOnLineSide(a0, a1, *vmAddrToPtr<line_t>(a2));
    a2 = s1;
    a0 = lw(gp + 0xC60);                                // Load from: gOldX (80078240)
    a1 = lw(gp + 0xC64);                                // Load from: gOldY (80078244)
    s0 = v0;
    v0 = P_PointOnLineSide(a0, a1, *vmAddrToPtr<line_t>(a2));
    if (s0 == v0) goto loc_8001E6E8;
    a1 = 0x80080000;                                    // Result = 80080000
    a1 = lw(a1 - 0x7F74);                               // Load from: gpTryMoveThing (8007808C)
    a0 = s1;
    P_CrossSpecialLine();
loc_8001E6E8:
    v1 = lw(gp + 0xAE0);                                // Load from: gNumCrossCheckLines (800780C0)
    v0 = -1;                                            // Result = FFFFFFFF
    v1--;
    sw(v1, gp + 0xAE0);                                 // Store to: gNumCrossCheckLines (800780C0)
    {
        const bool bJump = (v1 != v0);
        v0 = 1;                                         // Result = 00000001
        if (bJump) goto loc_8001E694;
    }
loc_8001E700:
    sw(v0, gp + 0xB5C);                                 // Store to: gbTryMove2 (8007813C)
loc_8001E704:
    ra = lw(sp + 0x1C);
    s2 = lw(sp + 0x18);
    s1 = lw(sp + 0x14);
    s0 = lw(sp + 0x10);
    sp += 0x20;
    return;
}

//------------------------------------------------------------------------------------------------------------------------------------------
// Tell what side of the given line a point is on: returns '0' if on the front side, '1' if on the back side.
// Same logic as 'R_PointOnSide' pretty much, but without the special optimized cases.
//
// PC-PSX: not compiling this as it is unused, and generates an unused warning.
//------------------------------------------------------------------------------------------------------------------------------------------
#if !PC_PSX_DOOM_MODS

static int32_t PM_PointOnLineSide(const fixed_t x, const fixed_t y, const line_t& line) noexcept {
    const int32_t dx = x - line.vertex1->x;
    const int32_t dy = y - line.vertex1->y;
    const int32_t lprod = (dx >> FRACBITS) * (line.dy >> FRACBITS);
    const int32_t rprod = (dy >> FRACBITS) * (line.dx >> FRACBITS);
    return (rprod >= lprod);
}

#endif  // !PC_PSX_DOOM_MODS

//------------------------------------------------------------------------------------------------------------------------------------------
// Unlinks the given thing from sector thing lists and the blockmap.
// Very similar to 'P_UnsetThingPosition' except the thing is always unlinked from sectors.
//------------------------------------------------------------------------------------------------------------------------------------------
static void PM_UnsetThingPosition(mobj_t& thing) noexcept {
    // Remove the thing from sector thing lists
    if (thing.snext) {
        thing.snext->sprev = thing.sprev;
    }

    if (thing.sprev) {
        thing.sprev->snext = thing.snext;
    } else {
        thing.subsector->sector->thinglist = thing.snext;
    }

    // Does this thing get added to the blockmap?
    // If so remove it from the blockmap.
    if ((thing.flags & MF_NOBLOCKMAP) == 0) {
        if (thing.bnext) {
            thing.bnext->bprev = thing.bprev;
        }
        
        if (thing.bprev) {
            thing.bprev->bnext = thing.bnext;
        } else {
            const int32_t blockx = (thing.x - *gBlockmapOriginX) >> MAPBLOCKSHIFT;
            const int32_t blocky = (thing.y - *gBlockmapOriginY) >> MAPBLOCKSHIFT;

            // PC-PSX: prevent buffer overflow if the map object is out of bounds.
            // This is part of the fix for the famous 'linedef deletion' bug.
            #if PC_PSX_DOOM_MODS
                if ((blockx >= 0) && (blockx < *gBlockmapWidth)) {
                    if ((blocky >= 0) && (blocky < *gBlockmapHeight)) {
                        (*gppBlockLinks)[blocky * (*gBlockmapWidth) + blockx] = thing.bnext;
                    }
                }
            #else
                (*gppBlockLinks)[blocky * (*gBlockmapWidth) + blockx] = thing.bnext;
            #endif
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------
// Update the subsector for the thing. Also add the thing to sector and blockmap thing lists if applicable.
// Almost the exact same as 'P_SetThingPosition' except the subsector for the thing must be precomputed first by 'PM_CheckPosition'.
//------------------------------------------------------------------------------------------------------------------------------------------
static void PM_SetThingPosition(mobj_t& mobj) noexcept {
    // Note: this function needs the subsector precomputed externally
    subsector_t& newSubsec = *gpNewSubsec->get();
    mobj.subsector = &newSubsec;
    
    // Add the thing to sector thing lists, if the thing flags allow it
    if ((mobj.flags & MF_NOSECTOR) == 0) {
        sector_t& newSector = *newSubsec.sector.get();
        mobj.sprev = nullptr;
        mobj.snext = newSector.thinglist.get();
        
        if (newSector.thinglist) {
            newSector.thinglist->sprev = &mobj;
        }

        newSector.thinglist = &mobj;
    }

    // Add the thing to blockmap thing lists, if the thing flags allow it
    if ((mobj.flags & MF_NOBLOCKMAP) == 0) {
        const int32_t blockX = (mobj.x - *gBlockmapOriginX) >> MAPBLOCKSHIFT;
        const int32_t blockY = (mobj.y - *gBlockmapOriginY) >> MAPBLOCKSHIFT;

        // Make sure the thing is bounds for the blockmap: if not then just don't add it to the blockmap
        if ((blockX >= 0) && (blockY >= 0) && (blockX < *gBlockmapWidth) && (blockY < *gBlockmapHeight)) {
            VmPtr<mobj_t>& blockList = gppBlockLinks->get()[blockY * (*gBlockmapWidth) + blockX];
            mobj.bprev = nullptr;
            mobj.bnext = blockList.get();

            if (blockList) {
                blockList->bprev = &mobj;
            }

            blockList = &mobj;
        } else {
            mobj.bprev = nullptr;
            mobj.bnext = nullptr;
        }
    }
}

void PM_CheckPosition() noexcept {
loc_8001E978:
    v1 = 0x80080000;                                    // Result = 80080000
    v1 = lw(v1 - 0x7F74);                               // Load from: gpTryMoveThing (8007808C)
    a1 = 0x80080000;                                    // Result = 80080000
    a1 = lw(a1 - 0x7EAC);                               // Load from: gTryMoveY (80078154)
    sp -= 0x28;
    sw(s0, sp + 0x10);
    s0 = 0x80090000;                                    // Result = 80090000
    s0 += 0x7C10;                                       // Result = gtTmbBox[0] (80097C10)
    sw(ra, sp + 0x24);
    sw(s4, sp + 0x20);
    sw(s3, sp + 0x1C);
    sw(s2, sp + 0x18);
    sw(s1, sp + 0x14);
    v0 = lw(v1 + 0x40);
    a2 = lw(v1 + 0x64);
    v0 += a1;
    sw(v0, s0);                                         // Store to: gtTmbBox[0] (80097C10)
    v0 = lw(v1 + 0x40);
    a0 = 0x80080000;                                    // Result = 80080000
    a0 = lw(a0 - 0x7EB0);                               // Load from: gTryMoveX (80078150)
    v0 = a1 - v0;
    at = 0x80090000;                                    // Result = 80090000
    sw(v0, at + 0x7C14);                                // Store to: gtTmbBox[1] (80097C14)
    v0 = lw(v1 + 0x40);
    v0 += a0;
    at = 0x80090000;                                    // Result = 80090000
    sw(v0, at + 0x7C1C);                                // Store to: gtTmbBox[3] (80097C1C)
    v0 = lw(v1 + 0x40);
    sw(a2, gp + 0xA98);                                 // Store to: gTmFlags (80078078)
    v0 = a0 - v0;
    at = 0x80090000;                                    // Result = 80090000
    sw(v0, at + 0x7C18);                                // Store to: gtTmbBox[2] (80097C18)
    _thunk_R_PointInSubsector();
    v1 = lw(v0);
    sw(v0, gp + 0xCDC);                                 // Store to: gpNewSubsec (800782BC)
    v0 = lw(v0);
    sw(0, gp + 0xAE0);                                  // Store to: gNumCrossCheckLines (800780C0)
    sw(0, gp + 0xCE4);                                  // Store to: gpMoveThing (800782C4)
    sw(0, gp + 0xC68);                                  // Store to: gpBlockLine (80078248)
    a0 = lw(v1);
    a1 = lw(v0 + 0x4);
    v0 = 0x80070000;                                    // Result = 80070000
    v0 = lw(v0 + 0x7BC4);                               // Load from: gValidCount (80077BC4)
    v1 = lw(gp + 0xA98);                                // Load from: gTmFlags (80078078)
    v0++;
    v1 &= 0x1000;
    at = 0x80070000;                                    // Result = 80070000
    sw(v0, at + 0x7BC4);                                // Store to: gValidCount (80077BC4)
    sw(a0, gp + 0x95C);                                 // Store to: gTmDropoffZ (80077F3C)
    sw(a0, gp + 0xC08);                                 // Store to: gTmFloorZ (800781E8)
    sw(a1, gp + 0x924);                                 // Store to: gTmCeilingZ (80077F04)
    {
        const bool bJump = (v1 == 0);
        v1 = 0xFFE00000;                                // Result = FFE00000
        if (bJump) goto loc_8001EA68;
    }
    v0 = 1;                                             // Result = 00000001
    goto loc_8001EC40;
loc_8001EA5C:
    sw(0, gp + 0xB5C);                                  // Store to: gbTryMove2 (8007813C)
    goto loc_8001EC44;
loc_8001EA68:
    v0 = 0x80090000;                                    // Result = 80090000
    v0 = lw(v0 + 0x7C18);                               // Load from: gtTmbBox[2] (80097C18)
    a1 = *gBlockmapOriginX;
    a0 = 0x80090000;                                    // Result = 80090000
    a0 = lw(a0 + 0x7C1C);                               // Load from: gtTmbBox[3] (80097C1C)
    a2 = *gBlockmapOriginY;
    v0 -= a1;
    v0 += v1;
    a3 = u32(i32(v0) >> 23);
    a0 -= a1;
    a1 = 0x200000;                                      // Result = 00200000
    a0 += a1;
    s3 = u32(i32(a0) >> 23);
    v1 = 0x80090000;                                    // Result = 80090000
    v1 = lw(v1 + 0x7C14);                               // Load from: gtTmbBox[1] (80097C14)
    v0 = lw(s0);                                        // Load from: gtTmbBox[0] (80097C10)
    v1 -= a2;
    v1 -= a1;
    s4 = u32(i32(v1) >> 23);
    v0 -= a2;
    v0 += a1;
    s2 = u32(i32(v0) >> 23);
    if (i32(a3) >= 0) goto loc_8001EAD0;
    a3 = 0;                                             // Result = 00000000
loc_8001EAD0:
    if (i32(s4) >= 0) goto loc_8001EADC;
    s4 = 0;                                             // Result = 00000000
loc_8001EADC:
    v1 = *gBlockmapWidth;
    v0 = (i32(s3) < i32(v1));
    if (v0 != 0) goto loc_8001EAF8;
    s3 = v1 - 1;
loc_8001EAF8:
    v1 = *gBlockmapHeight;
    v0 = (i32(s2) < i32(v1));
    s1 = a3;
    if (v0 != 0) goto loc_8001EB14;
    s2 = v1 - 1;
loc_8001EB14:
    v0 = (i32(s3) < i32(s1));
    if (v0 != 0) goto loc_8001EB5C;
    v0 = (i32(s2) < i32(s4));
loc_8001EB24:
    s0 = s4;
    if (v0 != 0) goto loc_8001EB4C;
    a0 = s1;
loc_8001EB30:
    a1 = s0;
    v0 = PM_BlockThingsIterator(a0, a1);
    s0++;
    if (v0 == 0) goto loc_8001EA5C;
    v0 = (i32(s2) < i32(s0));
    a0 = s1;
    if (v0 == 0) goto loc_8001EB30;
loc_8001EB4C:
    s1++;
    v0 = (i32(s3) < i32(s1));
    {
        const bool bJump = (v0 == 0);
        v0 = (i32(s2) < i32(s4));
        if (bJump) goto loc_8001EB24;
    }
loc_8001EB5C:
    v0 = 0x80090000;                                    // Result = 80090000
    v0 = lw(v0 + 0x7C18);                               // Load from: gtTmbBox[2] (80097C18)
    a0 = *gBlockmapOriginX;
    v1 = 0x80090000;                                    // Result = 80090000
    v1 = lw(v1 + 0x7C1C);                               // Load from: gtTmbBox[3] (80097C1C)
    a1 = *gBlockmapOriginY;
    v0 -= a0;
    a3 = u32(i32(v0) >> 23);
    v1 -= a0;
    s3 = u32(i32(v1) >> 23);
    a0 = 0x80090000;                                    // Result = 80090000
    a0 = lw(a0 + 0x7C14);                               // Load from: gtTmbBox[1] (80097C14)
    v0 = 0x80090000;                                    // Result = 80090000
    v0 = lw(v0 + 0x7C10);                               // Load from: gtTmbBox[0] (80097C10)
    a0 -= a1;
    s4 = u32(i32(a0) >> 23);
    v0 -= a1;
    s2 = u32(i32(v0) >> 23);
    if (i32(a3) >= 0) goto loc_8001EBB4;
    a3 = 0;                                             // Result = 00000000
loc_8001EBB4:
    if (i32(s4) >= 0) goto loc_8001EBC0;
    s4 = 0;                                             // Result = 00000000
loc_8001EBC0:
    v1 = *gBlockmapWidth;
    v0 = (i32(s3) < i32(v1));
    if (v0 != 0) goto loc_8001EBDC;
    s3 = v1 - 1;
loc_8001EBDC:
    v1 = *gBlockmapHeight;
    v0 = (i32(s2) < i32(v1));
    s1 = a3;
    if (v0 != 0) goto loc_8001EBF8;
    s2 = v1 - 1;
loc_8001EBF8:
    v0 = (i32(s3) < i32(s1));
    {
        const bool bJump = (v0 != 0);
        v0 = 1;                                         // Result = 00000001
        if (bJump) goto loc_8001EC40;
    }
loc_8001EC04:
    v0 = (i32(s2) < i32(s4));
    s0 = s4;
    if (v0 != 0) goto loc_8001EC30;
    a0 = s1;
loc_8001EC14:
    a1 = s0;
    v0 = PM_BlockLinesIterator(a0, a1);
    s0++;
    if (v0 == 0) goto loc_8001EA5C;
    v0 = (i32(s2) < i32(s0));
    a0 = s1;
    if (v0 == 0) goto loc_8001EC14;
loc_8001EC30:
    s1++;
    v0 = (i32(s3) < i32(s1));
    {
        const bool bJump = (v0 == 0);
        v0 = 1;                                         // Result = 00000001
        if (bJump) goto loc_8001EC04;
    }
loc_8001EC40:
    sw(v0, gp + 0xB5C);                                 // Store to: gbTryMove2 (8007813C)
loc_8001EC44:
    ra = lw(sp + 0x24);
    s4 = lw(sp + 0x20);
    s3 = lw(sp + 0x1C);
    s2 = lw(sp + 0x18);
    s1 = lw(sp + 0x14);
    s0 = lw(sp + 0x10);
    sp += 0x28;
    return;
}

//------------------------------------------------------------------------------------------------------------------------------------------
// Test if 'gTestTmBBox' intersects the given line: returns 'true' if there is an intersection
//------------------------------------------------------------------------------------------------------------------------------------------
static bool PM_BoxCrossLine(line_t& line) noexcept {
    // Check if the test bounding box is outside the bounding box of the line: if it is then early out
    const bool bTestBBOutsideLineBB = (
        (gTestTmBBox[BOXTOP] <= line.bbox[BOXBOTTOM]) ||
        (gTestTmBBox[BOXBOTTOM] >= line.bbox[BOXTOP]) ||
        (gTestTmBBox[BOXLEFT] >= line.bbox[BOXRIGHT]) ||
        (gTestTmBBox[BOXRIGHT] <= line.bbox[BOXLEFT])
    );

    if (bTestBBOutsideLineBB)
        return false;
    
    // Choose what line diagonal in the test box to test for crossing the line.
    // This code is trying to get a box diagonal that is as perpendicular to the line as possible.
    // Some lines for instance might run at 45 degrees and be parallel to the opposite box diagonal...
    fixed_t x1;
    fixed_t x2;

    if (line.slopetype == ST_POSITIVE) {
        x1 = gTestTmBBox[BOXLEFT];
        x2 = gTestTmBBox[BOXRIGHT];
    } else {
        x1 = gTestTmBBox[BOXRIGHT];
        x2 = gTestTmBBox[BOXLEFT];
    }

    // Use the cross product trick found in many functions such as 'R_PointOnSide' to determine what side of the line
    // both points of the test bounding box diagonal lie on.
    const fixed_t lx = line.vertex1->x;
    const fixed_t ly = line.vertex1->y;
    const int32_t ldx = line.dx >> FRACBITS;
    const int32_t ldy = line.dy >> FRACBITS;

    const int32_t dx1 = (x1 - lx) >> FRACBITS;
    const int32_t dy1 = (gTestTmBBox[BOXTOP] - ly) >> FRACBITS;
    const int32_t dx2 = (x2 - lx) >> FRACBITS;
    const int32_t dy2 = (gTestTmBBox[BOXBOTTOM] - ly) >> FRACBITS;

    const uint32_t side1 = (ldy * dx1 < dy1 * ldx);
    const uint32_t side2 = (ldy * dx2 < dy2 * ldx);

    // If the bounding box diagonal line points are on opposite sides of the line, then the box crosses the line
    return (side1 != side2);
}

//------------------------------------------------------------------------------------------------------------------------------------------
// Assuming a collider intersects the given line, tells if the given line will potentially block - ignoring height differences.
// Returns 'false' if the line is considered blocking ignoring height differences.
//------------------------------------------------------------------------------------------------------------------------------------------
static bool PIT_CheckLine(line_t& line) noexcept {
    // A 1 sided line cannot be crossed: register a collision against this
    if (!line.backsector)
        return false;
    
    // If not a projectile and the line is marked as explicitly blocking then block
    mobj_t& tryMoveThing = *gpTryMoveThing->get();

    if ((tryMoveThing.flags & MF_MISSILE) == 0) {
        // If the line blocks everything then register a collision
        if (line.flags & ML_BLOCKING)
            return false;
        
        // If the line blocks monsters and the thing is not a player then block
        if ((line.flags & ML_BLOCKMONSTERS) && (!tryMoveThing.player))
            return false;
    }

    // Is the line adjoining a sector that is fully closed up (probably a door)? If so then it always blocks:
    sector_t& fsec = *line.frontsector;
    sector_t& bsec = *line.backsector;

    if ((fsec.floorheight == fsec.ceilingheight) || (bsec.floorheight == bsec.ceilingheight)) {
        *gpBlockLine = &line;
        return false;
    }

    // Get the top and bottom height of the opening/gap and the lowest floor
    const fixed_t openTop = std::min(fsec.ceilingheight, bsec.ceilingheight);
    const fixed_t openBottom = std::max(fsec.floorheight, bsec.floorheight);
    const fixed_t lowFloor = std::min(fsec.floorheight, bsec.floorheight);

    // Adjust the global low ceiling, high floor and lowest floor values
    if (openTop < *gTmCeilingZ) {
        *gTmCeilingZ = openTop;
    }

    if (openBottom > *gTmFloorZ) {
        *gTmFloorZ = openBottom;
    }

    if (lowFloor < *gTmDropoffZ) {
        *gTmDropoffZ = lowFloor;
    }

    // PSX new addition: if the line has a special then save it for later testing to determine if the thing has crossed it and thus should trigger it.
    // This was added so that monsters could use teleporters again, since that ability was lost in the Jaguar version of the game.
    if (line.special) {
        if (*gNumCrossCheckLines < MAX_CROSS_LINES) {
            gpCrossCheckLines[*gNumCrossCheckLines] = &line;
            *gNumCrossCheckLines += 1;
        }
    }

    // This line does not block, ignoring height differences.
    // This function does NOT check whether the thing can pass the line due to height differences!
    return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------
// Check for a collision for the current thing being moved (in it's test position) against the input thing to this function.
// Returns 'false' if there was a collision and saves the thing globally, if it is to be interacted with (damage, pickup).
//------------------------------------------------------------------------------------------------------------------------------------------
static bool PIT_CheckThing(mobj_t& mobj) noexcept {
    // If it's not a special, blocking or shootable then we can't collide with it
    if ((mobj.flags & (MF_SPECIAL | MF_SOLID | MF_SHOOTABLE)) == 0)
        return true;
    
    // The thing cannot collide with itself
    mobj_t& tryMoveThing = *gpTryMoveThing->get();

    if (&mobj == &tryMoveThing)
        return true;

    // See if the thing is within range: exit with no collision if it isn't
    const fixed_t totalRadius = mobj.radius + tryMoveThing.radius;
    const fixed_t dx = std::abs(mobj.x - *gTryMoveX);
    const fixed_t dy = std::abs(mobj.y - *gTryMoveY);

    if ((dx >= totalRadius) || (dy >= totalRadius))
        return true;
    
    // Is the thing being moved a skull which is slamming into this thing?
    if (tryMoveThing.flags & MF_SKULLFLY) {
        *gpMoveThing = &mobj;
        return false;
    }

    // Special logic for missiles: unlike most other things, they can pass over/under things
    if (tryMoveThing.flags & MF_MISSILE) {
        // Is the missile flying above the thing? If so then no collision:
        if (tryMoveThing.z > mobj.z + mobj.height)
            return true;
        
        // Is the missile flying below the thing? If so then no collision:
        if (tryMoveThing.z + tryMoveThing.height < mobj.z)
            return true;
        
        // If we are colliding with the same species which fired the missile in most cases explode/collide the missile, but don't damage what was hit.
        // The firing thing is in the 'target' field for missiles.
        mobj_t& firingThing = *tryMoveThing.target.get();

        if (mobj.type == firingThing.type) {
            // Missiles don't collide with the things which fired them
            if (&mobj == &firingThing)
                return true;
            
            // Explode, but do no damage by just returning 'false' and not saving what was hit.
            // The exception to this is if the thing type is a player; players can splash damage other players with rockets...
            if (mobj.type != MT_PLAYER)
                return false;
        }

        // If the thing hit is shootable then save it for damage purposes and return 'false' for a collision
        if (mobj.flags & MF_SHOOTABLE) {
            *gpMoveThing = &mobj;
            return false;
        }

        // Otherwise just explode the missile (but do no damage) if the thing hit was solid
        return ((mobj.flags & MF_SOLID) == 0);
    }
    
    // Are we colliding with an item that can be picked up?
    // If so then save it but return 'true' for no collision (pickups do not block).
    if ((mobj.flags & MF_SPECIAL) && (*gTmFlags & MF_PICKUP)) {
        *gpMoveThing = &mobj;
        return true;
    }

    // In all other cases there is a collision if the item collided with is solid
    return ((mobj.flags & MF_SOLID) == 0);
}

//------------------------------------------------------------------------------------------------------------------------------------------
// Check for potential collisions against all lines in the given blockmap cell, ignoring height differences.
// Returns 'false' if there is a definite collision, 'true' otherwise.
//------------------------------------------------------------------------------------------------------------------------------------------
static bool PM_BlockLinesIterator(const int32_t x, const int32_t y) noexcept {
    // Get the line list for this blockmap cell
    const int16_t* pLineNum = (int16_t*)(gpBlockmapLump->get() + gpBlockmap->get()[y * (*gBlockmapWidth) + x]);

    // Visit all lines in the cell, checking for intersection and potential collision.
    // Stop when there is a definite collision.
    line_t* const pLines = gpLines->get();

    for (; *pLineNum != -1; ++pLineNum) {
        line_t& line = pLines[*pLineNum];

        // Only check the line if not already checked this test
        if (line.validcount != *gValidCount) {
            line.validcount = *gValidCount;
            
            // If it's collided with and definitely blocking then stop
            if (PM_BoxCrossLine(line) && (!PIT_CheckLine(line)))
                return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------
// Check for collisions against all things in the given blockmap cell. Returns 'true' if there were no collisions, 'false' otherwise.
// In some cases the thing collided with is saved in 'gpMoveThing' for futher interactions like pickups and damaging.
//------------------------------------------------------------------------------------------------------------------------------------------
static bool PM_BlockThingsIterator(const int32_t x, const int32_t y) noexcept {
    for (mobj_t* pmobj = gppBlockLinks->get()[x + y * (*gBlockmapWidth)].get(); pmobj; pmobj = pmobj->bnext.get()) {
        if (!PIT_CheckThing(*pmobj))
            return false;
    }

    return true;
}
