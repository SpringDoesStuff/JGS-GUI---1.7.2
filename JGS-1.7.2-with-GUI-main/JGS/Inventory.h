#pragma once

#include <map>

class Inventory
{
public:
    static inline std::map<AFortPlayerController*, Inventory*> InventoryMap;

    AFortPlayerController* PC;

    void Init(AFortPlayerController* InPC)
    {
        PC = InPC;
    }

    void AddDefaultQuickBarItems()
    {
        auto QuickBars = PC->QuickBars;
        auto FortInventory = PC->WorldInventory;

        static auto PickaxeDef = FindObjectFast<UFortWeaponItemDefinition>("/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Athena_C_T01.WID_Harvest_Pickaxe_Athena_C_T01");
        static auto EditToolDef = FindObjectFast<UFortEditToolItemDefinition>("/Game/Items/Weapons/BuildingTools/EditTool.EditTool");
        static auto WallBuildDef = FindObjectFast<UFortBuildingItemDefinition>("/Game/Items/Weapons/BuildingTools/BuildingItemData_Wall.BuildingItemData_Wall");
        static auto FloorBuildDef = FindObjectFast<UFortBuildingItemDefinition>("/Game/Items/Weapons/BuildingTools/BuildingItemData_Floor.BuildingItemData_Floor");
        static auto StairBuildDef = FindObjectFast<UFortBuildingItemDefinition>("/Game/Items/Weapons/BuildingTools/BuildingItemData_Stair_W.BuildingItemData_Stair_W");
        static auto RoofBuildDef = FindObjectFast<UFortBuildingItemDefinition>("/Game/Items/Weapons/BuildingTools/BuildingItemData_RoofS.BuildingItemData_RoofS");

        auto EditToolItem = EditToolDef->CreateTemporaryItemInstanceBP(1, 0);
        auto WorldEditToolItem = (UFortWorldItem*)EditToolItem;
        WorldEditToolItem->ItemEntry.Count = 1;
        FortInventory->Inventory.ReplicatedEntries.Add(WorldEditToolItem->ItemEntry);
        FortInventory->Inventory.ItemInstances.Add(WorldEditToolItem);

        auto PickaxeItem = PickaxeDef->CreateTemporaryItemInstanceBP(1, 0);
        auto WorldPickaxeItem = reinterpret_cast<UFortWorldItem*>(PickaxeItem);
        WorldPickaxeItem->ItemEntry.Count = 1;
        FortInventory->Inventory.ReplicatedEntries.Add(WorldPickaxeItem->ItemEntry);
        FortInventory->Inventory.ItemInstances.Add(WorldPickaxeItem);
        QuickBars->ServerAddItemInternal(WorldPickaxeItem->GetItemGuid(), EFortQuickBars::Primary, 0);

        auto WallBuildItem = WallBuildDef->CreateTemporaryItemInstanceBP(1, 0);
        auto WallWorldBuildItem = reinterpret_cast<UFortWorldItem*>(WallBuildItem);
        FortInventory->Inventory.ReplicatedEntries.Add(WallWorldBuildItem->ItemEntry);
        FortInventory->Inventory.ItemInstances.Add(WallWorldBuildItem);
        QuickBars->ServerAddItemInternal(WallWorldBuildItem->GetItemGuid(), EFortQuickBars::Secondary, 0);

        auto FloorBuildItem = FloorBuildDef->CreateTemporaryItemInstanceBP(1, 0);
        auto FloorWorldBuildItem = reinterpret_cast<UFortWorldItem*>(FloorBuildItem);
        FortInventory->Inventory.ReplicatedEntries.Add(FloorWorldBuildItem->ItemEntry);
        FortInventory->Inventory.ItemInstances.Add(FloorWorldBuildItem);
        QuickBars->ServerAddItemInternal(FloorWorldBuildItem->GetItemGuid(), EFortQuickBars::Secondary, 1);

        auto StairBuildItem = StairBuildDef->CreateTemporaryItemInstanceBP(1, 0);
        auto StairWorldBuildItem = reinterpret_cast<UFortWorldItem*>(StairBuildItem);
        FortInventory->Inventory.ReplicatedEntries.Add(StairWorldBuildItem->ItemEntry);
        FortInventory->Inventory.ItemInstances.Add(StairWorldBuildItem);
        QuickBars->ServerAddItemInternal(StairWorldBuildItem->GetItemGuid(), EFortQuickBars::Secondary, 2);

        auto RoofBuildItem = RoofBuildDef->CreateTemporaryItemInstanceBP(1, 0);
        auto RoofWorldBuildItem = reinterpret_cast<UFortWorldItem*>(RoofBuildItem);
        FortInventory->Inventory.ReplicatedEntries.Add(RoofWorldBuildItem->ItemEntry);
        FortInventory->Inventory.ItemInstances.Add(RoofWorldBuildItem);
        QuickBars->ServerAddItemInternal(RoofWorldBuildItem->GetItemGuid(), EFortQuickBars::Secondary, 3);
    }

    void SetupInventory()
    {
        std::map<UFortItemDefinition*, int> ItemsToAddMap;

        auto NewQuickBars = (AFortQuickBars*)Util::SpawnActor(AFortQuickBars::StaticClass(), {}, {});
        NewQuickBars->SetOwner(PC);
        PC->QuickBars = NewQuickBars;
        PC->OnRep_QuickBar();

        auto FortInventory = PC->WorldInventory;
        auto QuickBars = PC->QuickBars;
        
        QuickBars->EnableSlot(EFortQuickBars::Primary, 0);
        QuickBars->EnableSlot(EFortQuickBars::Primary, 1);
        QuickBars->EnableSlot(EFortQuickBars::Primary, 2);
        QuickBars->EnableSlot(EFortQuickBars::Primary, 3);
        QuickBars->EnableSlot(EFortQuickBars::Primary, 4);
        QuickBars->EnableSlot(EFortQuickBars::Secondary, 0);
        QuickBars->EnableSlot(EFortQuickBars::Secondary, 1);
        QuickBars->EnableSlot(EFortQuickBars::Secondary, 2);
        QuickBars->EnableSlot(EFortQuickBars::Secondary, 3);
        QuickBars->EnableSlot(EFortQuickBars::Secondary, 4);

        AddDefaultQuickBarItems();

        for (auto Item : ItemsToAddMap)
        {
            auto ItemDef = Item.first;
            auto ItemCount = Item.second;

            if (ItemDef)
            {
                auto NewWorldItem = (UFortWorldItem*)(ItemDef->CreateTemporaryItemInstanceBP(ItemCount, 0));
                FortInventory->Inventory.ReplicatedEntries.Add(NewWorldItem->ItemEntry);
                FortInventory->Inventory.ItemInstances.Add(NewWorldItem);
            }
        }

        ItemsToAddMap.empty();
        ItemsToAddMap.clear();

        PC->bHasInitializedWorldInventory = true;
	}

	void UpdateInventory()
	{
        PC->HandleWorldInventoryLocalUpdate();

        PC->WorldInventory->HandleInventoryLocalUpdate();
        PC->WorldInventory->bRequiresLocalUpdate = true;
        PC->QuickBars->OnRep_PrimaryQuickBar();
        PC->QuickBars->OnRep_SecondaryQuickBar();
        PC->QuickBars->ForceNetUpdate();
        PC->WorldInventory->ForceNetUpdate();

        PC->WorldInventory->Inventory.MarkArrayDirty();
	}

    void SpawnAllLootInInventory()
    {
        if (PC) {
            for (int i = 0; i < PC->WorldInventory->Inventory.ItemInstances.Num(); i++)
            {
                auto ItemInstance = PC->WorldInventory->Inventory.ItemInstances[i];
                PC->WorldInventory->Inventory.ItemInstances.Remove(i);

                if (ItemInstance->GetItemDefinitionBP()->GetFullName().contains("FortBuildingItemDefinition"))
                    continue;

                if (ItemInstance->GetItemDefinitionBP()->GetFullName().contains("FortEditToolItemDefinition"))
                    continue;

                if (ItemInstance->GetItemDefinitionBP()->GetFullName().contains("FortWeaponMeleeItemDefinition"))
                    continue;

                if (ItemInstance)
                {
                    auto NewFortPickup = (AFortPickupAthena*)(Util::SpawnActor(AFortPickupAthena::StaticClass(), PC->Pawn->K2_GetActorLocation(), {}));
                    NewFortPickup->PrimaryPickupItemEntry = ItemInstance->ItemEntry;
                    NewFortPickup->OnRep_PrimaryPickupItemEntry();
                    NewFortPickup->TossPickup(PC->Pawn->K2_GetActorLocation(), nullptr, 999);
                }
            }

            for (int i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
            {
                PC->WorldInventory->Inventory.ReplicatedEntries.Remove(i);
            }
        }
    }

    void ExecuteInventoryItem(FGuid InGuid)
    {
        if (PC)
        {
            if (PC->WorldInventory)
            {
                auto ItemInstances = PC->WorldInventory->Inventory.ItemInstances;

                for (int i = 0; i < ItemInstances.Num(); i++)
                {
                    auto ItemInstance = ItemInstances[i];

                    if (Util::AreGuidsTheSame(ItemInstance->GetItemGuid(), InGuid))
                    {
                        if (PC->Pawn)
                            ((AFortPlayerPawn*)PC->Pawn)->EquipWeaponDefinition((UFortWeaponItemDefinition*)ItemInstance->GetItemDefinitionBP(), InGuid);

                        if (ItemInstance->GetItemDefinitionBP()->IsA(UFortTrapItemDefinition::StaticClass()))
                        {
                            ((AFortPlayerPawn*)PC->Pawn)->PickUpActor(nullptr, (UFortDecoItemDefinition*)ItemInstance->GetItemDefinitionBP());
                        }
                    }
                }
            }
        }
    }
};

static Inventory* FindInventory(AFortPlayerController* PC)
{
    for (auto Inv : Inventory::InventoryMap)
    {
        if (Inv.first == PC)
            return Inv.second;
    }

    return nullptr;
}

static Inventory* CreateInventoryForPlayerController(AFortPlayerController* PC)
{
    Inventory* NewInv = new Inventory;
    NewInv->Init(PC);
    Inventory::InventoryMap.insert_or_assign(PC, NewInv);
    return NewInv;
}