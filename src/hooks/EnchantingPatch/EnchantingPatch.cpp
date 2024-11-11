#include "EnchantingPatch.h"

void EnchantingPatch::Install()
{
    REL::Relocation<std::uintptr_t> hookPoint{ RELOCATION_ID(51379, 51379), 0x23A };
    auto& trampoline = SKSE::GetTrampoline();
    _PostMessage = trampoline.write_call<5>(hookPoint.address(), PostMessage_);

    logger::info("Installed hooks for class {}", typeid(EnchantingPatch).name());
}

void EnchantingPatch::PostMessage_(RE::MessageBoxData* data)
{
    auto&& menu = ((RE::CraftingSubMenus::EnchantConstructMenu::EnchantMenuCraftCallback*)data->callback.get())->subMenu;

    RE::BSTArray<RE::BSTSmartPointer<RE::CraftingSubMenus::EnchantConstructMenu::EnchantmentEntry>> savedEffects = menu->selected.effects;

    RE::CraftingSubMenus::EnchantConstructMenu::ItemChangeEntry* soulmem = nullptr;
    if (menu->selected.soulGem->data->countDelta > 1) {
        soulmem = menu->selected.soulGem.get();
        soulmem->IncRef();
    }

    data->callback->Run(RE::IMessageBoxCallback::Message::kUnk0);

    for (auto&& eff : savedEffects) {
        eff->selected = true;
    }

    if (soulmem) {
        soulmem->selected = true;
    }

    menu->selected.effects = savedEffects;
    menu->selected.soulGem = RE::BSTSmartPointer<RE::CraftingSubMenus::EnchantConstructMenu::ItemChangeEntry>(soulmem);

    return;
}
