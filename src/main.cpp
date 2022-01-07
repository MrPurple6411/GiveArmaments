#include "main.hpp"

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

static Il2CppObject* PlayerHand;
static Il2CppObject* Inventory;

MAKE_HOOK_FIND_CLASS_UNSAFE_INSTANCE(PlayerHandStart, "SG.Claymore.Interaction","PlayerHand", "Start", void, Il2CppObject* self)
{
    //calls the original start method to set everything up to be used.
    PlayerHandStart(self);

    if(PlayerHand)
        return;

    //Tries to get the PlayerHand.inventory value
    ::std::optional<Il2CppObject *> inventory = ::il2cpp_utils::GetFieldValue(self, "inventory");

    //Validates it has actually returned something.
    if(!inventory.has_value()){
        getLogger().log(Logging::ERROR,"Inventory Not found!");
        return;
    }

    PlayerHand = self;
    Inventory = inventory.value();

    //Tries to get the MethodInfo* for SG::Claymore::SaveSystem::ArmamentInventory.GiveAll()
    const MethodInfo* GiveAll = ::il2cpp_utils::FindMethod(Inventory, "GiveAll");

    //Validates it has actually returned something.
    if(!GiveAll){
        getLogger().log(Logging::ERROR,"GiveAll Method Not found!");
        return;
    }

    //Calls the GiveAll which unlocks all weapons to be able to be purchased.
    ::il2cpp_utils::RunMethod(Inventory, GiveAll);
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
    static auto *logger = new Logger(modInfo);
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
    INSTALL_HOOK(getLogger(), PlayerHandStart);
    getLogger().log(Logging::INFO,"Installed all hooks!");
}