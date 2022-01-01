#include "main.hpp"

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

MAKE_HOOK_FIND_CLASS_UNSAFE_INSTANCE(PlayerHandStart, "SG.Claymore.Interaction","PlayerHand", "Start", void, Il2CppObject* self)
{
    
}

// Loads the config from disk using our modInfo, then returns it for use
Configuration &getConfig()
{
    static Configuration config(modInfo);
    config.Load();
    return config;
}

// Returns a logger, useful for printing debug messages
Logger &getLogger()
{
    static Logger *logger = new Logger(modInfo);
    return *logger;
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo &info)
{
    info.id = ID;
    info.version = VERSION;
    modInfo = info;

    getConfig().Load(); // Load the config file
    getLogger().log(Logging::INFO,"Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load()
{
    il2cpp_functions::Init();
    getLogger().log(Logging::INFO,"Installing hooks...");
    // Install our hooks (none defined yet)
    INSTALL_HOOK(getLogger(), PlayerHandStart)
    getLogger().log(Logging::INFO,"Installed all hooks!");
}