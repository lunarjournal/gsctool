/**
 * @file gsctool.h
 * @brief Call of Duty: Black Ops GSC Loader - gsctool.h
 *
 * Author: Dylan Müller
 *
 * +---------------------------------------+
 * |   .-.         .-.         .-.         |
 * |  /   \       /   \       /   \        |
 * | /     \     /     \     /     \     / |
 * |        \   /       \   /       \   /  |
 * |         "_"         "_"         "_"   |
 * |                                       |
 * |  _   _   _ _  _   _   ___   ___ _  _  |
 * | | | | | | | \| | /_\ | _ \ / __| || | |
 * | | |_| |_| | .` |/ _ \|   /_\__ \ __ | |
 * | |____\___/|_|\_/_/ \_\_|_(_)___/_||_| |
 * |                                       |
 * |                                       |
 * | Lunar RF Labs                         |
 * | https://lunar.sh                      |
 * |                                       |
 * | RF Research Laboratories              |
 * | Copyright (C) 2022-2024               |
 * |                                       |
 * +---------------------------------------+
 *
 * Copyright (c) 2022 Lunar RF Labs
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice,
 *       this list of conditions and the following disclaimer in the documentation
 *       and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _MAIN_H
#define _MAIN_H

#include <stdint.h>
#include <stdbool.h>

#define PATH_ROOT "gsctool"
#define PATH_DUMP PATH_ROOT "/cache"
#define FILE_OFFSET 0x8

typedef struct RawFileData
{
	int32_t deflatedSize;
	int32_t compressedSize;
} RawFileData;

typedef struct RawFile
{
	uint8_t* name;
	int32_t len;
	uint8_t* buffer;
} RawFile;

typedef enum XAssetType
{
	ASSET_TYPE_RAWFILE = 0x24
} XAssetType;

typedef union XAssetHeader
{
	struct RawFile* rawFile;
	void* data;
} XAssetHeader;

typedef struct XAsset
{
	enum XAssetType type;
	union XAssetHeader header;
} XAsset;

typedef struct XAssetEntry
{
	XAsset asset;
} XAssetEntry;

typedef union XAssetEntryPoolEntry
{
	struct XAssetEntry entry;
	union XAssetEntryPoolEntry* next;
} XAssetEntryPoolEntry;

typedef int32_t (__cdecl* Scr_LoadScript_t)(
	int32_t scriptInstance,
	const uint8_t* scriptName
);

typedef int32_t (__cdecl* Scr_GetFunctionHandle_t)(
	int32_t scriptInstance,
	const uint8_t* scriptName,
	const uint8_t* functioName
);

typedef uint16_t (__cdecl* Scr_ExecThread_t)(
	int32_t scriptInstance,
	int32_t handle,
	int32_t paramCount
);

typedef uint16_t (__cdecl* Scr_FreeThread_t)(
	uint16_t handle,
	int scriptInstance
);

typedef XAssetEntryPoolEntry* (__cdecl* DB_LinkXAssetEntry_t)(
	XAsset* newEntry,
	int32_t allowOverride
);

typedef uint8_t* (__cdecl* Dvar_FindVar_t)(
	uint8_t* variable
);

typedef int* (__cdecl* Assign_Hotfix_t)(
	int* a1, int* a2
);

typedef void (__cdecl* Thread_Timer_t)(
	uint8_t a1, int a2,
	int a3, int a4
);

typedef void (__cdecl* Scr_LoadGameType_t)(
	void
);

#endif
