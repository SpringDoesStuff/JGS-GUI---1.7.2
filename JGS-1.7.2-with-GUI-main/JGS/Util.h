#pragma once

#include <cstdint>
#include <winnt.h>
#include <cstdio>
#include <consoleapi.h>
#include <libloaderapi.h>
#include <vector>
#include <iostream>
#include <Windows.h>
#include <Psapi.h>
#include "SDK.hpp"

using namespace SDK;

#define LOG(str) std::cout << "LogJGS: " << str << std::endl;

class Util
{
public:
	static __forceinline VOID InitConsole()
	{
		FILE* fDummy;
		AllocConsole();
		freopen_s(&fDummy, "CONIN$", "r", stdin);
		freopen_s(&fDummy, "CONOUT$", "w", stdout);
	}

	static __forceinline uintptr_t BaseAddress()
	{
		return reinterpret_cast<uintptr_t>(GetModuleHandle(0));
	}

	static __forceinline uintptr_t FindPattern(const char* signature, bool bRelative = false, uint32_t offset = 0)
	{
		uintptr_t base_address = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));
		static auto patternToByte = [](const char* pattern)
		{
			auto bytes = std::vector<int>{};
			const auto start = const_cast<char*>(pattern);
			const auto end = const_cast<char*>(pattern) + strlen(pattern);

			for (auto current = start; current < end; ++current)
			{
				if (*current == '?')
				{
					++current;
					if (*current == '?') ++current;
					bytes.push_back(-1);
				}
				else { bytes.push_back(strtoul(current, &current, 16)); }
			}
			return bytes;
		};

		const auto dosHeader = (PIMAGE_DOS_HEADER)base_address;
		const auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)base_address + dosHeader->e_lfanew);

		const auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
		auto patternBytes = patternToByte(signature);
		const auto scanBytes = reinterpret_cast<std::uint8_t*>(base_address);

		const auto s = patternBytes.size();
		const auto d = patternBytes.data();

		for (auto i = 0ul; i < sizeOfImage - s; ++i)
		{
			bool found = true;
			for (auto j = 0ul; j < s; ++j)
			{
				if (scanBytes[i + j] != d[j] && d[j] != -1)
				{
					found = false;
					break;
				}
			}
			if (found)
			{
				uintptr_t address = reinterpret_cast<uintptr_t>(&scanBytes[i]);
				if (bRelative)
				{
					address = ((address + offset + 4) + *(int32_t*)(address + offset));
					return address;
				}
				return address;
			}
		}
		return NULL;
	}

	static BOOL MaskCompare(PVOID pBuffer, LPCSTR lpPattern, LPCSTR lpMask)
	{
		for (auto value = static_cast<PBYTE>(pBuffer); *lpMask; ++lpPattern, ++lpMask, ++value)
		{
			if (*lpMask == 'x' && *reinterpret_cast<LPCBYTE>(lpPattern) != *value)
				return false;
		}

		return true;
	}

	static PBYTE FindPattern(PVOID pBase, DWORD dwSize, LPCSTR lpPattern, LPCSTR lpMask)
	{
		dwSize -= static_cast<DWORD>(strlen(lpMask));

		for (auto i = 0UL; i < dwSize; ++i)
		{
			auto pAddress = static_cast<PBYTE>(pBase) + i;

			if (MaskCompare(pAddress, lpPattern, lpMask))
				return pAddress;
		}

		return NULL;
	}

	static PBYTE FindPattern(LPCSTR lpPattern, LPCSTR lpMask)
	{
		MODULEINFO info = { 0 };

		GetModuleInformation(GetCurrentProcess(), GetModuleHandle(0), &info, sizeof(info));

		return Util::FindPattern(info.lpBaseOfDll, info.SizeOfImage, lpPattern, lpMask);
	}

	static AActor* SpawnActor(UClass* ActorClass, FVector Location, FRotator Rotation)
	{
		FQuat Quat;
		FTransform Transform;

		auto DEG_TO_RAD = 3.14159 / 180;
		auto DIVIDE_BY_2 = DEG_TO_RAD / 2;

		auto SP = sin(Rotation.Pitch * DIVIDE_BY_2);
		auto CP = cos(Rotation.Pitch * DIVIDE_BY_2);

		auto SY = sin(Rotation.Yaw * DIVIDE_BY_2);
		auto CY = cos(Rotation.Yaw * DIVIDE_BY_2);

		auto SR = sin(Rotation.Roll * DIVIDE_BY_2);
		auto CR = cos(Rotation.Roll * DIVIDE_BY_2);

		Quat.X = CR * SP * SY - SR * CP * CY;
		Quat.Y = -CR * SP * CY - SR * CP * SY;
		Quat.Z = CR * CP * SY - SR * SP * CY;
		Quat.W = CR * CP * CY + SR * SP * SY;

		Transform.Rotation = Quat;
		Transform.Scale3D = FVector{ 1,1,1 };
		Transform.Translation = Location;

		auto Actor = Globals::GPS->STATIC_BeginSpawningActorFromClass(Globals::FortEngine->GameViewport->World, ActorClass, Transform, false, nullptr);
		Globals::GPS->STATIC_FinishSpawningActor(Actor, Transform);
		return Actor;
	}

	static bool AreGuidsTheSame(FGuid guidA, FGuid guidB)
	{
		if (guidA.A == guidB.A && guidA.B == guidB.B && guidA.C == guidB.C && guidA.D == guidB.D)
			return true;
		else
			return false;
	}
};