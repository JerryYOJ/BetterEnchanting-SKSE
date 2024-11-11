class EnchantingPatch {

public:
    static void Install();

protected:
    static void PostMessage_(RE::MessageBoxData* data);

private:
    EnchantingPatch() = delete;
    EnchantingPatch(const EnchantingPatch&) = delete;
    EnchantingPatch(EnchantingPatch&&) = delete;
    ~EnchantingPatch() = delete;

    EnchantingPatch& operator=(const EnchantingPatch&) = delete;
    EnchantingPatch& operator=(EnchantingPatch&&) = delete;

    inline static REL::Relocation<decltype(PostMessage_)> _PostMessage;
};