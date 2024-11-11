#include <spdlog/sinks/basic_file_sink.h>

#include "hooks/hooks.h"

void InitializeLog() {
    auto logsFolder = SKSE::log::log_directory();
    if (!logsFolder) SKSE::stl::report_and_fail("SKSE log_directory not provided, logs disabled.");
    auto pluginName = SKSE::PluginDeclaration::GetSingleton()->GetName();
    auto logFilePath = *logsFolder / std::format("{}.log", pluginName);
    auto fileLoggerPtr = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.string(), true);
    auto loggerPtr = std::make_shared<spdlog::logger>("log", std::move(fileLoggerPtr));
    spdlog::set_default_logger(std::move(loggerPtr));
    spdlog::set_level(spdlog::level::debug);
    spdlog::flush_on(spdlog::level::trace);
}

SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    SKSE::Init(skse);
    
    // This example prints "Hello, world!" to the Skyrim ~ console.
    // To view it, open the ~ console from the Skyrim Main Menu.
    //SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message *message) {
    //    if (message->type == SKSE::MessagingInterface::kDataLoaded)
    //    else if (message->type == SKSE::MessagingInterface::kPostLoad)
    //    else if (message->type == SKSE::MessagingInterface::kPostLoadGame) {
    //    }
    //});

    InitializeLog();
    SKSE::AllocTrampoline(1 << 7);

    Hooks::Install();

    return true;
}