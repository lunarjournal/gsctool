/**
 * Written by: Dylan Muller
 * Copyright (c) 2023
 */

/* Global includes */
#include <Windows.h>
#include <direct.h>

/* Local includes */
#include "offsets.h"
#include "gsctool.h"
#include "lib/miniz/miniz.h"
#include "lib/cdl86/cdl.h"

/* Local definitions */
#define SIZE_BUF 1024 * 1024
#define SIZE_PATH 256

/* Assign function pointers */
/* Load GSC script */
Scr_LoadScript_t Scr_LoadScript = (Scr_LoadScript_t)T5_Scr_LoadScript;
/* Get VM function handle */
Scr_GetFunctionHandle_t Scr_GetFunctionHandle = (Scr_GetFunctionHandle_t)T5_Scr_GetFunctionHandle;
/* Create new execution thread */
Scr_ExecThread_t Scr_ExecThread = (Scr_ExecThread_t)T5_Scr_ExecThread;
/* Free execution thread */
Scr_FreeThread_t Scr_FreeThread = (Scr_FreeThread_t)T5_Scr_FreeThread;
/* Execute on level startup */
Scr_LoadGameType_t Scr_LoadGameType = (Scr_LoadGameType_t)T5_Scr_LoadGameType;
/* Dvar lookup */
Dvar_FindVar_t Dvar_FindVar = (Dvar_FindVar_t)T5_Dvar_FindVar;
/* Link asset/rawfile. Should be called before Scr_LoadScript */
DB_LinkXAssetEntry_t DB_LinkXAssetEntry = (DB_LinkXAssetEntry_t)T5_DB_LinkXAssetEntry;
/* Hotfix to prevent crash */
Assign_Hotfix_t Assign_Hotfix = (Assign_Hotfix_t)T5_Assign_Hotfix;
/* Remove anti-debug timer */
Thread_Timer_t Thread_Timer = (Thread_Timer_t)T5_Thread_Timer;

/* Global variables */
int* init_trigger = (int*)T5_init_trigger;
uint8_t mod_dir[SIZE_PATH];
uint8_t mod_dir_relative[SIZE_PATH];
uint8_t entry_file[SIZE_PATH];
int func_handle = 0;

/* Strip extension from name */
void strip_ext(uint8_t* fname)
{
	uint8_t* end = fname + strlen(fname);

	while (end > fname && *end != '.')
	{
		--end;
	}

	if (end > fname)
	{
		*end = '\0';
	}
}

/* Print banner */
void print_banner()
{
	printf("[info] gsctool init\n");
	printf("[info] gsc dumper and loader\n");
}

/* Print mod info (config.ini) */
void print_mod_info(uint8_t* dir, uint8_t* entry)
{
	printf("mod.dir=%s\n", dir);
	printf("mod.entry=%s\n", entry);
}

/* Print example config.ini */
void print_mod_example()
{
	printf("example config.ini:\n");
	printf("mod.dir=example\n");
	printf("mod.entry=main.gsc\n");
}

/* Check if file exists */
BOOL FileExists(LPCSTR szPath)
{
	DWORD attrib = GetFileAttributesA(szPath);
	return (attrib != INVALID_FILE_ATTRIBUTES
		&& !(attrib & FILE_ATTRIBUTE_DIRECTORY));
}

/* Check if directory */
BOOL DirectoryExists(LPCSTR szPath)
{
	DWORD attrib = GetFileAttributesA(szPath);
	return (attrib != INVALID_FILE_ATTRIBUTES
		&& (attrib & FILE_ATTRIBUTE_DIRECTORY));
}

/* Hotfix to prevent crash when exiting gamemode. */
int* __cdecl Assign_Hotfix_hk(
	int* a1, int* a2
)
{
	if (a1 != NULL && a2 != NULL)
	{
		return Assign_Hotfix(a1, a2);
	}
	else
	{
		return 0;
	}
}

/* AC bypass to allow detouring. */
void __cdecl Thread_Timer_hk(
	uint8_t a1, int a2,
	int a3, int a4
)
{
	return;
}

/* Load our custom GSC script */
int32_t __cdecl Scr_LoadScript_hk(
	int32_t scriptInstance,
	const uint8_t* scriptName
)
{
	uint8_t script_file[SIZE_PATH];
	uint8_t mapname_buffer[SIZE_PATH];
	uint8_t* mapname = NULL;
	FILE* gsc_script = NULL;;
	uint8_t* source_buffer = 0x0;
	uint8_t* data_buffer = 0x0;
	uint8_t* compress_buffer = 0x0;
	uint32_t gsc_size = 0x0;
	int cmp_status = 0x0;
	RawFileData* file_data = 0x0;
	mz_ulong gsc_compress_size = 0x0;
	mz_ulong data_size = 0;
	XAsset entry = { 0 };
	int ret = 0x0;
	int result = 0x0;

	result = Scr_LoadScript(scriptInstance, scriptName);

	/* Get current map name */
	mapname = Dvar_FindVar("mapname");
	sprintf(mapname_buffer, "maps/%s", mapname);

	/* Are we loading script for current map? */
	if (strcmp(mapname_buffer, scriptName) == 0 
	     && strcmp(mapname_buffer, "maps/frontend") != 0)
	{
		/* Load GSC script to inject */
		FILE* gsc_script = fopen(entry_file, "rb");
		if (gsc_script)
		{
			/* Get GSC script size in bytes */
			fseek(gsc_script, 0, SEEK_END);
			gsc_size = ftell(gsc_script);
			fseek(gsc_script, 0, SEEK_SET);

			/* Allocate GSC script text buffer */
			source_buffer = (uint8_t*)malloc((sizeof(uint8_t) * gsc_size) + 1);

			/* Return if we cannot allocate memory */
			if (!source_buffer)
			{
				printf("[info] failed allocating memory\n");
				fclose(gsc_script);
				return result;
			}
			/* Read GSC script into buffer */
			fread(source_buffer, sizeof(uint8_t), gsc_size, gsc_script);

			/* Don't forget null terminator! */
			source_buffer[gsc_size] = 0;

			/* Estimate compression bounds for allocator */
			gsc_compress_size = mz_compressBound(gsc_size + 1);
			/* Allocate data buffer for compressed data */
			data_buffer = (uint8_t*)malloc((sizeof(uint8_t) * gsc_compress_size)
				                           + sizeof(RawFileData));

			/* Return if we cannot allocate memory */
			if (!data_buffer)
			{
				printf("[info] failed allocating memory\n");
				fclose(gsc_script);
				free(source_buffer);
				return result;
			}
			compress_buffer = (uint8_t*)(data_buffer + sizeof(RawFileData));
			/* Apply zlib compression */
			cmp_status = mz_compress((uint8_t*)compress_buffer, &gsc_compress_size, 
				                     (uint8_t*)source_buffer, gsc_size + 1);
			/* Now free source buffer */
			free(source_buffer);

			if (cmp_status == MZ_OK){
				/* If compression succeeded set compression and deflate lengths */
				if (data_buffer) {
					file_data = (RawFileData*)data_buffer;
					file_data->compressedSize = gsc_compress_size;
					file_data->deflatedSize = gsc_size + 1;
				}

				/* Calculate total payload size */
				data_size = gsc_compress_size + sizeof(RawFileData);
				/* Set entry type */
				entry.type = ASSET_TYPE_RAWFILE;
				entry.header.rawFile = (RawFile*)malloc(sizeof(RawFile));

				/* Return if we cannot allocate memory */
				if (!entry.header.rawFile)
				{
					printf("[info] failed allocating memory\n");
					fclose(gsc_script);
					free(data_buffer);
					return result;
				}
				entry.header.rawFile->buffer = data_buffer;
				entry.header.rawFile->len = data_size;
				entry.header.rawFile->name = (uint8_t*)entry_file;
	
				printf("[info] linking asset %s\n", entry_file);
				/* Link script asset before compilation */
				DB_LinkXAssetEntry(&entry, 0);
	
				sprintf(script_file, "%s", entry_file);
				strip_ext(script_file);
				/* Compile GSC script */
				ret = Scr_LoadScript(0, script_file);

				/* Free asset */
				free(data_buffer);
				free(entry.header.rawFile);
	
				printf("[info] retreiving function handle\n");
				/* Get main VM entry point */
				func_handle = Scr_GetFunctionHandle(0, script_file, "main");
				if (func_handle)
				{
					printf("[info] main @ %p\n", (uint8_t*)func_handle);
				}
				else
				{
					printf("[error] failed to retreive main handle\n");
				}
				fclose(gsc_script);
			}
		}
	}

	return result;
}

/* Execute function handle at correct timing */
void __cdecl Scr_LoadGameType_hk()
{
	Scr_LoadGameType();
	uint8_t* mapname = Dvar_FindVar("mapname");

	if (strcmp(mapname, "frontend") == 0)
	{
		printf("[info] loaded main menu\n");
	}
	else if (func_handle > 0)
	{
		printf("[info] executing main @ %p\n", (uint8_t*)func_handle);
		int16_t handle = Scr_ExecThread(0, func_handle, 0);
		Scr_FreeThread(handle, 0);
	}
	return;
}

/* Recursively make directory */
void __cdecl mkdir_recursive(uint8_t* path)
{
	int i = 0x0;
	uint8_t* buffer = NULL;

	if (path != NULL) {
		buffer = (uint8_t*)malloc(SIZE_PATH);
		if (!buffer)
		{
			printf("[info] failed allocating memory\n");
			return;
		}
		int len_path = strlen(path);
		for (i = 0; i < len_path; i++)
		{
			if (path[i] == '/')
			{
				strncpy(buffer, path, i);
				buffer[i] = 0;
				mkdir(buffer);
			}
		}
		free(buffer);
	}


}

/* Dump GSC and CSC scripts as they are loaded */
XAssetEntryPoolEntry* __cdecl DB_LinkXAssetEntry_hk(
	XAsset* newEntry,
	int32_t allowOverride
)
{
	struct RawFile* rawfile = newEntry->header.rawFile;
	const uint8_t* data = 0x0;
	uint8_t* buffer = (uint8_t*)malloc(SIZE_BUF);
	uint8_t* path = (uint8_t*)malloc(SIZE_PATH);
	uint8_t* script_name = 0x0;
	z_stream stream;
	int status = 0x0;
	int inflate_len = 0x0;
	FILE* inflated_script = 0x0;

	if (!buffer || !path)
	{
		printf("[info] failed allocating memory\n");
		return DB_LinkXAssetEntry(newEntry, allowOverride);
	}

	if (newEntry->type == ASSET_TYPE_RAWFILE && rawfile!= NULL)
	{
		script_name = rawfile->name;


		/* Check for GSC or CSC script */
		if (strstr(script_name, ".gsc") != NULL
			|| strstr(script_name, ".csc") != NULL)
		{
			memset(&stream, 0, sizeof(stream));
			/* Apply file data offset */
			data = (const uint8_t*)(rawfile->buffer + FILE_OFFSET);

			/* Setup decompression stream */
			stream.next_in = data;
			stream.avail_in = rawfile->len - FILE_OFFSET;
			stream.next_out = buffer;
			stream.avail_out = SIZE_BUF;

			/* Initialize inflate with stream */
			inflateInit(&stream);
			/* Decompress script... */
			status = inflate(&stream, Z_SYNC_FLUSH);
			inflateEnd(&stream);
			inflate_len = stream.total_out;

			/* On decompression success create dir and write file */
			if (status == Z_STREAM_END || status == Z_OK)
			{
				sprintf(path, "%s/%s", PATH_DUMP, script_name);
				printf("[cache] %s\n", path);
				mkdir_recursive(path);
				inflated_script = fopen(path, "wb");
				if (inflated_script)
				{
					fwrite(buffer, sizeof(uint8_t), inflate_len, inflated_script);
					fclose(inflated_script);
				}
			}

		}
	}
	free(buffer);
	free(path);

	return DB_LinkXAssetEntry(newEntry, allowOverride);
}

/* Main thread to execute once injected. */
DWORD WINAPI init_thread(LPVOID lpParam)
{
	FILE* dependency_list = NULL;
	FILE* config_file = NULL;
	uint8_t* mod_dir_rel = (uint8_t*)malloc(SIZE_PATH);
	uint8_t* entry_file_rel = (uint8_t*)malloc(SIZE_PATH);

	/* Wait until all threads have started. */
	while (! *init_trigger) { Sleep(10); }

	/* Allocate console and redirect std io. */
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);

	if (!mod_dir_rel || !entry_file_rel)
	{
		printf("[info] failed allocating memory\n");
		return EXIT_FAILURE;
	}
	/* Print startup banner */
	print_banner();

	mkdir(PATH_ROOT);

	config_file = fopen(PATH_ROOT "/config.ini", "r");
	if (!config_file)
	{
		printf("[error] %s", "Error opening config.ini\n");
		return EXIT_SUCCESS;
	}

	printf("[info] loaded config.ini\n");

	if (fscanf(config_file, "mod.dir=%s\n", mod_dir_rel) != 1)
	{
		printf("[error] %s\n", "failed reading 'mod.dir' key in config file\n");
		print_mod_example();
		return EXIT_SUCCESS;
	}

	if (fscanf(config_file, "mod.entry=%s\n", entry_file_rel) != 1)
	{
		printf("[error] %s\n", "failed reading 'mod.entry' from config file\n");
		print_mod_example();
		return EXIT_SUCCESS;
	}

	fclose(config_file);

	printf("[info] mod info\n");
	print_mod_info(mod_dir_rel, entry_file_rel);

	sprintf(mod_dir, "%s/%s", PATH_ROOT, mod_dir_rel);
	sprintf(mod_dir_relative, "%s", mod_dir_rel);

	printf("[info] searching for mod directory...\n");
	printf("%s\n", mod_dir);
	if (!DirectoryExists(mod_dir))
	{
		printf("[error] %s\n", "mod directory does not exist");
		return EXIT_SUCCESS;
	}

	printf("[info] mod directory found\n");
	sprintf(entry_file, "%s/%s", mod_dir, entry_file_rel);

	free(entry_file_rel);
	free(mod_dir_rel);

	printf("[info] searching for mod entry...\n");

	if (!FileExists(entry_file))
	{
		printf("[error] %s\n", "mod entry file does not exist, check config!");
		return EXIT_SUCCESS;
	}

	printf("[info] mod entry found\n");
	printf("[info] applying detours...\n");

	/* Subroutine patches. */
	struct cdl_jmp_patch thread_timer =
		cdl_jmp_attach((void**)&Thread_Timer, Thread_Timer_hk);
	struct cdl_jmp_patch assign_hotfix =
		cdl_jmp_attach((void**)&Assign_Hotfix, Assign_Hotfix_hk);
	struct cdl_jmp_patch link_asset =
		cdl_jmp_attach((void**)&DB_LinkXAssetEntry, DB_LinkXAssetEntry_hk);
	struct cdl_jmp_patch load_script =
		cdl_jmp_attach((void**)&Scr_LoadScript, Scr_LoadScript_hk);
	struct cdl_jmp_patch load_game_type =
		cdl_jmp_attach((void**)&Scr_LoadGameType, Scr_LoadGameType_hk);

	/* Print debug info */
	printf("[info] Thread_Timer() detour info\n");
	cdl_jmp_dbg(&thread_timer);
	printf("[info] Assign_Hotfix() detour info\n");
	cdl_jmp_dbg(&assign_hotfix);
	printf("[info] DB_LinkXAssetEntry() detour info\n");
	cdl_jmp_dbg(&link_asset);
	printf("[info] Scr_LoadScript() detour info\n");
	cdl_jmp_dbg(&load_script);
	printf("[info] Scr_LoadGameType() detour info\n");
	cdl_jmp_dbg(&load_game_type);

	/* Flush std io */
	while (1) 
	{ 
		flushall();
		Sleep(200); 
	}

	return EXIT_SUCCESS;

}

/* Main entry point */
BOOL APIENTRY DllMain(
	HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread((LPSECURITY_ATTRIBUTES)0, 0, (LPTHREAD_START_ROUTINE)init_thread,
		             (LPVOID)hModule, 0, (LPDWORD)NULL);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

