#include "EnchantingPatch.h"

void EnchantingPatch::Install()
{
    int offset = 0x23A;
    if (REL::Module::GetRuntime() == REL::Module::Runtime::SE) offset = 0x1F8;
    REL::Relocation<std::uintptr_t> hookPoint{ RELOCATION_ID(50487, 51379), offset };
    auto& trampoline = SKSE::GetTrampoline();
    _PostMessage = trampoline.write_call<5>(hookPoint.address(), PostMessage_);

    logger::info("Installed hooks for class {}", typeid(EnchantingPatch).name());
}

void EnchantingPatch::PostMessage_(RE::MessageBoxData* data)
{
    auto&& menu = ((RE::CraftingSubMenus::EnchantConstructMenu::EnchantMenuCraftCallback*)data->callback.get())->subMenu;

    RE::BSTArray<RE::BSTSmartPointer<RE::CraftingSubMenus::EnchantConstructMenu::EnchantmentEntry>> savedEffects = menu->selected.effects;

    RE::TESBoundObject* soulGemObj = nullptr;
    if (menu->selected.soulGem->data->countDelta > 1) {
        soulGemObj = menu->selected.soulGem->data->GetObject();
    }

    data->callback->Run(RE::IMessageBoxCallback::Message::kUnk0);

    for (auto&& eff : savedEffects) {
        eff->selected = true;
    }

    menu->selected.effects = savedEffects;
    for (auto&& it : menu->listEntries) {
        if (it && it->filterFlag.all(RE::CraftingSubMenus::EnchantConstructMenu::FilterFlag::SoulGem) && (*(RE::BSTSmartPointer<RE::CraftingSubMenus::EnchantConstructMenu::ItemChangeEntry>*) & it)->data->GetObject() == soulGemObj)
            menu->selected.soulGem = *(RE::BSTSmartPointer<RE::CraftingSubMenus::EnchantConstructMenu::ItemChangeEntry>*)&it;
    }

    return;
}
