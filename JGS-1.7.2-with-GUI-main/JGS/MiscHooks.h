#pragma once

namespace Misc
{
	__int64 (*OnReload)(UObject* a1, unsigned int a2);
	__int64 __fastcall OnReloadHook(AFortWeapon* Weapon, unsigned int a2)
	{
		if (Weapon)
		{
			auto ItemDefiniton = (UFortWeaponItemDefinition*)Weapon->WeaponData;
			auto AmmoDef = ItemDefiniton->GetAmmoWorldItemDefinition_BP();

			if (AmmoDef == nullptr)
				AmmoDef = ItemDefiniton;

			auto Pawn = (APawn*)Weapon->Owner;
			auto Controller = (AFortPlayerController*)Pawn->Controller;
			auto WorldInventory = Controller->WorldInventory;
			auto QuickBars = Controller->QuickBars;

			for (int i = 0; i < WorldInventory->Inventory.ItemInstances.Num(); i++)
			{
				auto ItemInstance = WorldInventory->Inventory.ItemInstances[i];

				if (ItemInstance->GetItemDefinitionBP() == AmmoDef)
				{
					int newCount = ItemInstance->ItemEntry.Count - a2;

					WorldInventory->Inventory.ItemInstances.Remove(i);

					for (int j = 0; j < WorldInventory->Inventory.ReplicatedEntries.Num(); j++)
					{
						auto Entry = WorldInventory->Inventory.ReplicatedEntries[j];

						if (Entry.ItemDefinition == AmmoDef)
						{
							WorldInventory->Inventory.ReplicatedEntries.Remove(j);
						}
					}

					if (newCount != 0)
					{
						auto NewWorldItem = (UFortWorldItem*)(AmmoDef->CreateTemporaryItemInstanceBP(newCount, 1));

						WorldInventory->Inventory.ReplicatedEntries.Add(NewWorldItem->ItemEntry);
						WorldInventory->Inventory.ItemInstances.Add(NewWorldItem);
					}
					else {
						/*for (int i = 0; i < QuickBars->PrimaryQuickBar.Slots.Num(); i++)
						{
							auto Slot = QuickBars->PrimaryQuickBar.Slots[i];

							if (Slot.Items.IsValidIndex(0))
							{
								if (Util::AreGuidsTheSame(Slot.Items[0], ItemInstance->GetItemGuid()))
								{
									if (i != -1)
										QuickBars->EmptySlot(EFortQuickBars::Primary, i);

									Slot.Items.Remove(0);
								}
							}
						}*/
					}
				}
			}

			FindInventory(Controller)->UpdateInventory();
		}
		return OnReload(Weapon, a2);
	}

	__int64 (*OnBuild)(UObject*, void*);
	__int64 __fastcall OnBuildHook(AFortPlayerController* Controller, void* a2)
	{
		if (Controller)
		{
			UFortResourceItemDefinition* ResourceDef = nullptr;

			if (Controller->CurrentResourceType == EFortResourceType::Wood)
				ResourceDef = FindObjectFast<UFortResourceItemDefinition>("/Game/Items/ResourcePickups/WoodItemData.WoodItemData");

			if (Controller->CurrentResourceType == EFortResourceType::Stone)
				ResourceDef = FindObjectFast<UFortResourceItemDefinition>("/Game/Items/ResourcePickups/StoneItemData.StoneItemData");

			if (Controller->CurrentResourceType == EFortResourceType::Metal)
				ResourceDef = FindObjectFast<UFortResourceItemDefinition>("/Game/Items/ResourcePickups/MetalItemData.MetalItemData");

			if (ResourceDef)
			{
				auto WorldInventory = Controller->WorldInventory;

				for (int i = 0; i < WorldInventory->Inventory.ItemInstances.Num(); i++)
				{
					auto ItemInstance = WorldInventory->Inventory.ItemInstances[i];

					if (ItemInstance->GetItemDefinitionBP() == ResourceDef)
					{
						int newCount = ItemInstance->ItemEntry.Count - 10;

						WorldInventory->Inventory.ItemInstances.Remove(i);

						for (int j = 0; j < WorldInventory->Inventory.ReplicatedEntries.Num(); j++)
						{
							auto Entry = WorldInventory->Inventory.ReplicatedEntries[j];

							if (Entry.ItemDefinition == ResourceDef)
							{
								WorldInventory->Inventory.ReplicatedEntries.Remove(j);
							}
						}

						if (newCount != 0)
						{
							auto NewWorldItem = (UFortWorldItem*)(ResourceDef->CreateTemporaryItemInstanceBP(newCount, 1));

							WorldInventory->Inventory.ReplicatedEntries.Add(NewWorldItem->ItemEntry);
							WorldInventory->Inventory.ItemInstances.Add(NewWorldItem);
						}
					}
				}

				FindInventory(Controller)->UpdateInventory();
			}
		}

		return OnBuild(Controller, a2);
	}

	static void Init()
	{
		auto BaseAddr = Util::BaseAddress();

		MH_CreateHook((void*)(BaseAddr + 0x9239C0), OnReloadHook, (void**)(&OnReload));
		MH_EnableHook((void*)(BaseAddr + 0x9239C0));
		MH_CreateHook((void*)(BaseAddr + 0x81E0B0), OnBuildHook, (void**)(&OnBuild));
		MH_EnableHook((void*)(BaseAddr + 0x81E0B0));
	}
}