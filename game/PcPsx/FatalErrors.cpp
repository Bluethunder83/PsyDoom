//------------------------------------------------------------------------------------------------------------------------------------------
// Functions that can be called to handle fatal errors like out of memory.
// The program will terminate after each fatal error type.
//------------------------------------------------------------------------------------------------------------------------------------------
#include "FatalErrors.h"

#include "ProgArgs.h"
#include "Video.h"

#include <cstdio>
#include <SDL.h>
#include <vector>

BEGIN_NAMESPACE(FatalErrors)

// Fallback string to use if null pointers are given for some reason.
// Normally most code does not tolerate nulls, but error reporting should be more robust.
static constexpr const char* const UNSPECIFIED_ERROR_STR = "An unspecified/unknown error has occurred!";

//------------------------------------------------------------------------------------------------------------------------------------------
// Actual implementation of raising a fatal error
//------------------------------------------------------------------------------------------------------------------------------------------
[[noreturn]] static void raiseImpl(const char* const errorMsg) noexcept {
    // Always print to the console (standard out) and in debug builds
    if (errorMsg) {
        std::printf("[FATAL ERROR] %s\n", errorMsg);
    } else {
        std::printf("[FATAL ERROR] %s\n", UNSPECIFIED_ERROR_STR);
    }

    // Show a GUI error box (except if in headless mode)
    if (!ProgArgs::gbHeadlessMode) {
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "A fatal error has occurred!",
            (errorMsg != nullptr) ? errorMsg : UNSPECIFIED_ERROR_STR,
            Video::getWindow()
        );
    }
    
    // Finish up!
    std::terminate();
}

//------------------------------------------------------------------------------------------------------------------------------------------
// Issue an out of memory fatal error
//------------------------------------------------------------------------------------------------------------------------------------------
[[noreturn]] void outOfMemory() noexcept {
    raiseImpl("A memory allocation has failed - out of memory!");
}

//------------------------------------------------------------------------------------------------------------------------------------------
// Issue a generic fatal error message with any message.
// This is useful for very specific error messages for a certain part of the application.
//------------------------------------------------------------------------------------------------------------------------------------------
[[noreturn]] void raise(const char* const pMsgStr) noexcept {
    if (pMsgStr == nullptr) {
        raiseImpl(UNSPECIFIED_ERROR_STR);
    } else {
        raiseImpl(pMsgStr);
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------
// Raise a fatal error with 'printf' style formatting
//------------------------------------------------------------------------------------------------------------------------------------------
[[noreturn]] void raiseF(const char* const pMsgFormatStr, ...) noexcept {
    if (pMsgFormatStr == nullptr) {
        raiseImpl(UNSPECIFIED_ERROR_STR);
    }
    else {
        const size_t guessBufferLength = (std::strlen(pMsgFormatStr) + 1) * 2;
        std::vector<char> buffer;
        buffer.resize(guessBufferLength);

        va_list va_args;
        va_start(va_args, pMsgFormatStr);
        int numCharsWrittenOrRequired = vsnprintf(buffer.data(), buffer.size(), pMsgFormatStr, va_args);
        va_end(va_args);

        if (numCharsWrittenOrRequired < 0) {
            // A return value of < 0 from 'vsnprintf' indicates an error!
            raiseImpl(UNSPECIFIED_ERROR_STR);
        }

        if ((unsigned int) numCharsWrittenOrRequired >= buffer.size()) {
            buffer.resize((size_t) numCharsWrittenOrRequired + 1);

            va_start(va_args, pMsgFormatStr);
            numCharsWrittenOrRequired = vsnprintf(buffer.data(), buffer.size(), pMsgFormatStr, va_args);
            va_end(va_args);

            if (numCharsWrittenOrRequired < 0) {
                // A return value of < 0 from 'vsnprintf' indicates an error!
                raiseImpl(UNSPECIFIED_ERROR_STR);
            }
        }

        va_end(va_args);
        raiseImpl(buffer.data());
    }
}

END_NAMESPACE(FatalErrors)
