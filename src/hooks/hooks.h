#include <MinHook.h>

#include "EnchantingPatch/EnchantingPatch.h"

namespace Hooks {
    void Install() { 
        MH_Initialize();

        EnchantingPatch::Install();

        return;
    }
}