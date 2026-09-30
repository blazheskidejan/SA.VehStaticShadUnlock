#include <plugin.h>
#include <cstdint>
#include <windows.h>

using namespace plugin;

struct Main
{
    Main()
    {
        // These addresses belong to GTA SA 1.0 US (Compact / Hoodlum).
        if (!IsGameVersion10us())
        {
            OutputDebugStringA("VehStaticShadUnlock: GTA SA 1.0 US is required.\n");
            return;
        }

        constexpr std::uintptr_t storeShadowForVehicle = 0x70BDA0;
        constexpr std::uintptr_t graphicsHighQuality = 0x70F9B0;

        // Search the first 64 bytes of StoreShadowForVehicle for a CALL whose
        // destination is GraphicsHighQuality, then redirect that call only.
        // This removes the quality-based early exit; the rest of the
        // original routine (strength, distance, models, etc.) still executes.
        for (auto address = storeShadowForVehicle;
             address + 5 <= storeShadowForVehicle + 0x40; ++address)
        {
            const auto target = patch::TranslateCallOffset<void*>(address);
            if (target != reinterpret_cast<void*>(GetGlobalAddress(graphicsHighQuality)))
                continue;

            patch::RedirectCall(address, IgnoreVehicleShadowQualityGate);
            return;
        }

        OutputDebugStringA("VehStaticShadUnlock: vehicle-shadow quality check not found; patch skipped.\n");
    }

    // Replaces only StoreShadowForVehicle's GraphicsHighQuality call.
    // Returning false bypasses its quality-based early exit.
    static bool __cdecl IgnoreVehicleShadowQualityGate()
    {
        return false;
    }
} gInstance;
