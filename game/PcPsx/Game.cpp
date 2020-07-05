//------------------------------------------------------------------------------------------------------------------------------------------
// Various game logic, mostly related to handling different game types and variants
//------------------------------------------------------------------------------------------------------------------------------------------
#include "Game.h"

#include "Doom/Renderer/r_data.h"
#include "Doom/UI/in_main.h"
#include "FatalErrors.h"
#include "IsoFileSys.h"
#include "PsxVm.h"

BEGIN_NAMESPACE(Game)

// Total number of maps in the game for Doom and Final Doom
static constexpr uint32_t NUM_MAPS_DOOM = 59;
static constexpr uint32_t NUM_MAPS_FINAL_DOOM = 30;

// Number of regular (non secret) maps in the game for Doom and Final Doom
static constexpr uint32_t NUM_REGULAR_MAPS_DOOM = 54;
static constexpr uint32_t NUM_REGULAR_MAPS_FINAL_DOOM = 30;

GameType        gGameType;
GameVariant     gGameVariant;

//------------------------------------------------------------------------------------------------------------------------------------------
// Determine which game type we are playing and from what region
//------------------------------------------------------------------------------------------------------------------------------------------
void determineGameTypeAndVariant() noexcept {
    if (PsxVm::gIsoFileSys.getEntry("SLUS_000.77")) {
        gGameType = GameType::Doom;
        gGameVariant = GameVariant::NTSC_U;
    } else if (PsxVm::gIsoFileSys.getEntry("SLPS_003.08")) {
        gGameType = GameType::Doom;
        gGameVariant = GameVariant::NTSC_J;
    } else if (PsxVm::gIsoFileSys.getEntry("SLES_001.32")) {
        gGameType = GameType::Doom;
        gGameVariant = GameVariant::PAL;
    } else if (PsxVm::gIsoFileSys.getEntry("SLUS_003.31")) {
        gGameType = GameType::FinalDoom;
        gGameVariant = GameVariant::NTSC_U;
    } else if (PsxVm::gIsoFileSys.getEntry("SLPS_007.27")) {
        gGameType = GameType::FinalDoom;
        gGameVariant = GameVariant::NTSC_J;
    } else if (PsxVm::gIsoFileSys.getEntry("SLES_004.87")) {
        gGameType = GameType::FinalDoom;
        gGameVariant = GameVariant::PAL;
    } else {
        FatalErrors::raise(
            "Unknown/unrecognized PSX Doom game disc provided!\n"
            "The disc given must be either 'Doom' or 'Final Doom' (NTSC-U, NTSC-J or PAL version)."
        );
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------
// Get the total number of maps for this game
//------------------------------------------------------------------------------------------------------------------------------------------
int32_t getNumMaps() noexcept {
    return (gGameType == GameType::FinalDoom) ? NUM_MAPS_FINAL_DOOM : NUM_MAPS_DOOM;
}

//------------------------------------------------------------------------------------------------------------------------------------------
// Get the total number of maps excluding secret levels for this game
//------------------------------------------------------------------------------------------------------------------------------------------
int32_t getNumRegularMaps() noexcept {
    return (gGameType == GameType::FinalDoom) ? NUM_REGULAR_MAPS_FINAL_DOOM : NUM_REGULAR_MAPS_DOOM;
}

//------------------------------------------------------------------------------------------------------------------------------------------
// Get the name of the specified map number
//------------------------------------------------------------------------------------------------------------------------------------------
const char* getMapName(const int32_t mapNum) noexcept {
    if (gGameType == GameType::Doom) {
        if (mapNum >= 1 && mapNum <= 59) {
            return gMapNames_Doom[mapNum - 1];
        }
    } else if (gGameType == GameType::FinalDoom) {
        if (mapNum >= 1 && mapNum <= 30) {
            return gMapNames_FinalDoom[mapNum - 1];
        }
    }

    return "Unknown Mapname";
}

//------------------------------------------------------------------------------------------------------------------------------------------
// Get the palette to use for various textures
//------------------------------------------------------------------------------------------------------------------------------------------
uint16_t getTexPalette_BACK() noexcept {
    return gPaletteClutIds[(gGameType == GameType::FinalDoom) ? TITLEPAL : MAINPAL];
}

END_NAMESPACE(Game)