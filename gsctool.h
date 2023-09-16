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
