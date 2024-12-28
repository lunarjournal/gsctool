/**
 * @file offsets.h
 * @brief Call of Duty: Black Ops GSC Loader - offsets.h
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

#ifndef _OFFSETS_H
#define _OFFSETS_H

/* Offsets for Call of Duty: Black Ops 1 (Win32) */
/* Found using IDA */
#define T5_Scr_LoadScript         0x00661AF0
#define T5_Scr_GetFunctionHandle  0x004E3470
#define T5_DB_LinkXAssetEntry     0x007A2F10
#define T5_Scr_ExecThread         0x005598E0
#define T5_Scr_FreeThread         0x005DE2C0
#define T5_Scr_LoadGameType       0x004B7F80
#define T5_Dvar_FindVar           0x0057FF80
#define T5_Assign_Hotfix          0x007A4800
#define T5_init_trigger           0x00C793B0
#define T5_Thread_Timer           0x004C06E0

#endif
