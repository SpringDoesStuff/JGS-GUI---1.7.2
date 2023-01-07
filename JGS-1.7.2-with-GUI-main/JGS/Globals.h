#pragma once

#include <iostream>

static auto StaticLoadObjectInternal = reinterpret_cast<UObject* (*)(UClass* InClass, UObject* InOuter, const TCHAR* Name, const TCHAR* FileName, uint32_t LoadFlags, UPackageMap* Sandbox, bool bAllowObjectReconciliation)>(uintptr_t(GetModuleHandle(NULL)) + 0x13E0180);

template <typename T>
static T* StaticLoadObject(const TCHAR* InPath)
{
    return (T*)StaticLoadObjectInternal(T::StaticClass(), nullptr, InPath, nullptr, 0, nullptr, false);
}

template <typename T>
static T* FindObjectFast(std::string ObjectName, UClass* ObjectClass = UObject::StaticClass())
{
    auto OrigInName = std::wstring(ObjectName.begin(), ObjectName.end()).c_str();

    auto StaticFindObject = (T * (*)(UClass*, UObject * Package, const wchar_t* OrigInName, bool ExactClass))((uintptr_t)GetModuleHandleA(0) + 0x13DE630);
    return StaticFindObject(ObjectClass, nullptr, OrigInName, false);
}

//#define CHEATS
//#define DBNO_ENABLED
//#define SAME_TEAM
//#define LOG_RPCS
//#define DUOS

struct CharPart
{
    UCustomCharacterPart* HeadPart;
    UCustomCharacterPart* BodyPart;
    UCustomCharacterPart* HatPart;
};

namespace Globals
{
	UWorld* World;
	UFortEngine* FortEngine;
	AFortPlayerController* PC;
	APlayerPawn_Generic_C* Pawn;
	UGameplayStatics* GPS;
	UKismetMathLibrary* MathLib;
    UKismetSystemLibrary* SystemLib;
    UCustomCharacterPart* HeadPart;
    UCustomCharacterPart* BodyPart;
    std::vector<UFortItemDefinition*> Traps;
    std::vector<UFortItemDefinition*> Consumables;
    std::vector<UFortItemDefinition*> SupplyDrop;

    std::vector<UFortItemDefinition*> CommonWeapons;
    std::vector<UFortItemDefinition*> UnCommonWeapons;
    std::vector<UFortItemDefinition*> RareWeapons;
    std::vector<UFortItemDefinition*> EpicWeapons;
    std::vector<UFortItemDefinition*> LegendaryWeapons;
    std::vector<UFortItemDefinition*> Ammo;

    std::vector <std::string> WeaponArrays
    {
        "Common",
        "UnCommon",
        "Rare",
        "Epic",
        "Legendary"
    };

    std::vector<CharPart> CharacterParts;
    bool bSTWMode = false;
}

static void LoadLootPools()
{
    //Ranged Weapons
    Globals::CommonWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_C_Ore_T02.WID_Assault_Auto_Athena_C_Ore_T02"));
    Globals::CommonWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_C_Ore_T02.WID_Assault_SemiAuto_Athena_C_Ore_T02"));
    Globals::CommonWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavy_Athena_C_Ore_T02.WID_Pistol_AutoHeavy_Athena_C_Ore_T02"));
    Globals::CommonWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_C_Ore_T02.WID_Pistol_SemiAuto_Athena_C_Ore_T02"));
    Globals::CommonWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_SixShooter_Athena_C_Ore_T02.WID_Pistol_SixShooter_Athena_C_Ore_T02"));
    Globals::CommonWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_C_Ore_T03.WID_Shotgun_Standard_Athena_C_Ore_T03"));

    Globals::UnCommonWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_UC_Ore_T03.WID_Assault_Auto_Athena_UC_Ore_T03"));
    Globals::UnCommonWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_UC_Ore_T03.WID_Assault_SemiAuto_Athena_UC_Ore_T03"));
    Globals::UnCommonWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavy_Athena_UC_Ore_T03.WID_Pistol_AutoHeavy_Athena_UC_Ore_T03"));
    Globals::UnCommonWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_Scavenger_Athena_UC_Ore_T03.WID_Pistol_Scavenger_Athena_UC_Ore_T03"));
    Globals::UnCommonWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_UC_Ore_T03.WID_Pistol_SemiAuto_Athena_UC_Ore_T03"));
    Globals::UnCommonWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_SixShooter_Athena_UC_Ore_T03.WID_Pistol_SixShooter_Athena_UC_Ore_T03"));
    Globals::UnCommonWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_UC_Ore_T03.WID_Shotgun_Standard_Athena_UC_Ore_T03"));

    Globals::RareWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_R_Ore_T03.WID_Assault_Auto_Athena_R_Ore_T03"));
    Globals::RareWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_R_Ore_T03.WID_Assault_SemiAuto_Athena_R_Ore_T03"));
    Globals::RareWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Surgical_Athena_R_Ore_T03.WID_Assault_Surgical_Athena_R_Ore_T03"));
    Globals::RareWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Launcher_Grenade_Athena_R_Ore_T03.WID_Launcher_Grenade_Athena_R_Ore_T03"));
    Globals::RareWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Launcher_Rocket_Athena_R_Ore_T03.WID_Launcher_Rocket_Athena_R_Ore_T03"));
    Globals::RareWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavy_Athena_R_Ore_T03.WID_Pistol_AutoHeavy_Athena_R_Ore_T03"));
    Globals::RareWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_Scavenger_Athena_R_Ore_T03.WID_Pistol_Scavenger_Athena_R_Ore_T03"));
    Globals::RareWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_R_Ore_T03.WID_Pistol_SemiAuto_Athena_R_Ore_T03"));
    Globals::RareWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_SixShooter_Athena_R_Ore_T03.WID_Pistol_SixShooter_Athena_R_Ore_T03"));
    Globals::RareWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_R_Ore_T03.WID_Shotgun_SemiAuto_Athena_R_Ore_T03"));
    Globals::RareWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_R_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_R_Ore_T03"));

    Globals::EpicWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_VR_Ore_T03.WID_Assault_AutoHigh_Athena_VR_Ore_T03"));
    Globals::EpicWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Surgical_Athena_VR_Ore_T03.WID_Assault_AutoHigh_Athena_VR_Ore_T03"));
    Globals::EpicWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Launcher_Grenade_Athena_VR_Ore_T03.WID_Assault_AutoHigh_Athena_VR_Ore_T03"));
    Globals::EpicWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Launcher_Rocket_Athena_VR_Ore_T03.WID_Assault_AutoHigh_Athena_VR_Ore_T03"));
    Globals::EpicWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_Scavenger_Athena_VR_Ore_T03.WID_Assault_AutoHigh_Athena_VR_Ore_T03"));
    Globals::EpicWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_VR_Ore_T03.WID_Assault_AutoHigh_Athena_VR_Ore_T03"));
    Globals::EpicWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_VR_Ore_T03.WID_Assault_AutoHigh_Athena_VR_Ore_T03"));
    Globals::EpicWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_Standard_Scope_Athena_VR_Ore_T03.WID_Assault_AutoHigh_Athena_VR_Ore_T03"));

    Globals::LegendaryWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_SR_Ore_T03.WID_Assault_AutoHigh_Athena_SR_Ore_T03"));
    Globals::LegendaryWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Launcher_Grenade_Athena_SR_Ore_T03.WID_Launcher_Grenade_Athena_SR_Ore_T03"));
    Globals::LegendaryWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Launcher_Rocket_Athena_SR_Ore_T03.WID_Launcher_Rocket_Athena_SR_Ore_T03"));
    Globals::LegendaryWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03"));
    Globals::LegendaryWeapons.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_Standard_Scope_Athena_SR_Ore_T03.WID_Sniper_Standard_Scope_Athena_SR_Ore_T03"));


    //Traps / Femboy traps :hot:
    Globals::Traps.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Traps/TID_Ceiling_Electric_Single_Athena_R_T03.TID_Ceiling_Electric_Single_Athena_R_T03"));
    Globals::Traps.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Traps/TID_Floor_Player_Jump_Pad_Athena.TID_Floor_Player_Jump_Pad_Athena"));
    Globals::Traps.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Traps/TID_Floor_Player_Jump_Pad_Free_Direction_Athena.TID_Floor_Player_Jump_Pad_Free_Direction_Athena"));
    Globals::Traps.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Traps/TID_Floor_Spikes_Athena_R_T03.TID_Floor_Spikes_Athena_R_T03"));
    Globals::Traps.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Traps/TID_Wall_Electric_Athena_R_T03.TID_Wall_Electric_Athena_R_T03"));

    //Consumables
    Globals::Consumables.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Consumables/Bandage/Athena_Bandage.Athena_Bandage"));
    Globals::Consumables.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Consumables/Grenade/Athena_Grenade.Athena_Grenade"));
    Globals::Consumables.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Consumables/Medkit/Athena_Medkit.Athena_Medkit"));
    Globals::Consumables.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Consumables/Shields/Athena_Shields.Athena_Shields"));

    // loot pool for supply drop (weapons only)
    Globals::SupplyDrop.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_AMR_Athena_SR_Ore_T03.WID_Sniper_AMR_Athena_SR_Ore_T03"));
    Globals::SupplyDrop.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_Standard_Scope_Athena_SR_Ore_T03.WID_Sniper_Standard_Scope_Athena_SR_Ore_T03"));
    Globals::SupplyDrop.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Launcher_Grenade_Athena_VR_Ore_T03.WID_Launcher_Grenade_Athena_VR_Ore_T03"));
    Globals::SupplyDrop.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Launcher_Grenade_Athena_SR_Ore_T03.WID_Launcher_Grenade_Athena_SR_Ore_T03"));
    Globals::SupplyDrop.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Surgical_Athena_VR_Ore_T03.WID_Assault_Surgical_Athena_VR_Ore_T03"));
    Globals::SupplyDrop.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_VR_Ore_T03.WID_Assault_AutoHigh_Athena_VR_Ore_T03"));
    Globals::SupplyDrop.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_SR_Ore_T03.WID_Assault_AutoHigh_Athena_SR_Ore_T03"));
    Globals::SupplyDrop.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_Standard_Scope_Athena_VR_Ore_T03.WID_Sniper_Standard_Scope_Athena_VR_Ore_T03"));
    Globals::SupplyDrop.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_VR_Ore_T03.WID_Shotgun_SemiAuto_Athena_VR_Ore_T03"));

    Globals::Ammo.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Ammo/AmmoDataRockets.AmmoDataRockets"));
    Globals::Ammo.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataShells.AthenaAmmoDataShells"));
    Globals::Ammo.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsMedium.AthenaAmmoDataBulletsMedium"));
    Globals::Ammo.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsLight.AthenaAmmoDataBulletsLight"));
    Globals::Ammo.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsHeavy.AthenaAmmoDataBulletsHeavy"));
    Globals::Ammo.push_back(FindObjectFast<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataEnergyCell.AthenaAmmoDataEnergyCell"));
}

// janky but whatever
static bool IsPickupStackable(UFortItemDefinition* PickupDef)
{
    std::string pickupDefName = PickupDef->GetFullName();
    if (pickupDefName.contains("Athena_Bandage") ||
        pickupDefName.contains("Athena_Grenade") ||
        pickupDefName.contains("Athena_Medkit") ||
        pickupDefName.contains("Athena_Shields")) {
        return true;
    }
    return !PickupDef->IsA(UFortWeaponItemDefinition::StaticClass());
}

static void LoadCharacterParts()
{
    //HID_Commando_Athena_05
    //HID_Commando_Athena_06
    //HID_Commando_Athena_12
    //HID_Commando_Athena_14
    //HID_Commando_Athena_22
    //HID_Commando_Athena_25
    //HID_Commando_Athena_26

    StaticLoadObject<UCustomCharacterPart>(L"/Game/Athena/Heroes/Meshes/Heads/M_MED_ASN_Jae_Head_01_ATH.M_MED_ASN_Jae_Head_01_ATH"); //05
    StaticLoadObject<UCustomCharacterPart>(L"/Game/Athena/Heroes/Meshes/Bodies/M_Med_Soldier_01_CV05_ATH.M_Med_Soldier_01_CV05_ATH"); //05
    StaticLoadObject<UCustomCharacterPart>(L"/Game/Athena/Heroes/Meshes/Heads/M_MED_HIS_Sparks_Head_01_ATH.M_MED_HIS_Sparks_Head_01_ATH"); //06
    StaticLoadObject<UCustomCharacterPart>(L"/Game/Athena/Heroes/Meshes/Bodies/M_Med_Soldier_01_TV14_ATH.M_Med_Soldier_01_TV14_ATH"); //06
    StaticLoadObject<UCustomCharacterPart>(L"/Game/Athena/Heroes/Meshes/Heads/F_MED_BLK_Red_Head_01_ATH.F_MED_BLK_Red_Head_01_ATH"); //12
    StaticLoadObject<UCustomCharacterPart>(L"/Game/Athena/Heroes/Meshes/Bodies/F_Med_Soldier_TV12_ATH.F_Med_Soldier_TV12_ATH"); //12
    StaticLoadObject<UCustomCharacterPart>(L"/Game/Athena/Heroes/Meshes/Heads/F_MED_CAU_Armstrong_Head_01_ATH.F_MED_CAU_Armstrong_Head_01_ATH"); //14
    StaticLoadObject<UCustomCharacterPart>(L"/Game/Athena/Heroes/Meshes/Bodies/F_Med_Soldier_TV17_ATH.F_Med_Soldier_TV17_ATH"); //14
    StaticLoadObject<UCustomCharacterPart>(L"/Game/Athena/Heroes/Meshes/Bodies/F_Med_Soldier_CV02_ATH.F_Med_Soldier_CV02_ATH"); //22
    StaticLoadObject<UCustomCharacterPart>(L"/Game/Athena/Heroes/Meshes/Heads/F_Med_Head1_ATH.F_Med_Head1_ATH"); //22
    StaticLoadObject<UCustomCharacterPart>(L"/Game/Characters/CharacterParts/Hats/Ramirez_Glasses.Ramirez_Glasses"); //22
    StaticLoadObject<UCustomCharacterPart>(L"/Game/Athena/Heroes/Meshes/Heads/F_MED_ASN_Sarah_Head_01_ATH.F_MED_ASN_Sarah_Head_01_ATH"); //25
    StaticLoadObject<UCustomCharacterPart>(L"/Game/Athena/Heroes/Meshes/Bodies/F_Med_Soldier_CV04_ATH.F_Med_Soldier_CV04_ATH"); //25
    StaticLoadObject<UCustomCharacterPart>(L"/Game/Athena/Heroes/Meshes/Heads/M_Med_Soldier_Head_01_ATH.M_Med_Soldier_Head_01_ATH"); //26
    StaticLoadObject<UCustomCharacterPart>(L"/Game/Athena/Heroes/Meshes/Bodies/M_Med_Soldier_01_Base_ATH.M_Med_Soldier_01_Base_ATH"); //26

    //05
    Globals::CharacterParts.push_back({
        FindObjectFast<UCustomCharacterPart>("/Game/Athena/Heroes/Meshes/Heads/M_MED_ASN_Jae_Head_01_ATH.M_MED_ASN_Jae_Head_01_ATH"),
        FindObjectFast<UCustomCharacterPart>("/Game/Athena/Heroes/Meshes/Bodies/M_Med_Soldier_01_CV05_ATH.M_Med_Soldier_01_CV05_ATH"),
        nullptr
    });

    //06
    Globals::CharacterParts.push_back({
        FindObjectFast<UCustomCharacterPart>("/Game/Athena/Heroes/Meshes/Heads/M_MED_HIS_Sparks_Head_01_ATH.M_MED_HIS_Sparks_Head_01_ATH"),
        FindObjectFast<UCustomCharacterPart>("/Game/Athena/Heroes/Meshes/Bodies/M_Med_Soldier_01_TV14_ATH.M_Med_Soldier_01_TV14_ATH"),
        nullptr
    });

    //12
    Globals::CharacterParts.push_back({
        FindObjectFast<UCustomCharacterPart>("/Game/Athena/Heroes/Meshes/Heads/F_MED_BLK_Red_Head_01_ATH.F_MED_BLK_Red_Head_01_ATH"),
        FindObjectFast<UCustomCharacterPart>("/Game/Athena/Heroes/Meshes/Bodies/F_Med_Soldier_TV12_ATH.F_Med_Soldier_TV12_ATH"),
        nullptr
    });

    //14
    Globals::CharacterParts.push_back({
        FindObjectFast<UCustomCharacterPart>("/Game/Athena/Heroes/Meshes/Heads/F_MED_CAU_Armstrong_Head_01_ATH.F_MED_CAU_Armstrong_Head_01_ATH"),
        FindObjectFast<UCustomCharacterPart>("/Game/Athena/Heroes/Meshes/Bodies/F_Med_Soldier_TV17_ATH.F_Med_Soldier_TV17_ATH"),
        nullptr
    });

    //22
    Globals::CharacterParts.push_back({
        FindObjectFast<UCustomCharacterPart>("/Game/Athena/Heroes/Meshes/Heads/F_Med_Head1_ATH.F_Med_Head1_ATH"),
        FindObjectFast<UCustomCharacterPart>("/Game/Athena/Heroes/Meshes/Bodies/F_Med_Soldier_CV02_ATH.F_Med_Soldier_CV02_ATH"),
        FindObjectFast<UCustomCharacterPart>("/Game/Characters/CharacterParts/Hats/Ramirez_Glasses.Ramirez_Glasses")
    });

    //25
    Globals::CharacterParts.push_back({
        FindObjectFast<UCustomCharacterPart>("/Game/Athena/Heroes/Meshes/Heads/F_MED_ASN_Sarah_Head_01_ATH.F_MED_ASN_Sarah_Head_01_ATH"),
        FindObjectFast<UCustomCharacterPart>("/Game/Athena/Heroes/Meshes/Bodies/F_Med_Soldier_CV04_ATH.F_Med_Soldier_CV04_ATH"),
        nullptr
    });

    //26
    Globals::CharacterParts.push_back({
        FindObjectFast<UCustomCharacterPart>("/Game/Athena/Heroes/Meshes/Heads/M_Med_Soldier_Head_01_ATH.M_Med_Soldier_Head_01_ATH"),
        FindObjectFast<UCustomCharacterPart>("/Game/Athena/Heroes/Meshes/Bodies/M_Med_Soldier_01_Base_ATH.M_Med_Soldier_01_Base_ATH"),
        nullptr
    });
}
