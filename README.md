# gsctool

Simple `GSC` script loader and dumper for the `PC` version of `Call Of Duty: Black Ops 1 (T5)`.

[`https://journal.lunar.sh/2023/gsctool.html`](https://journal.lunar.sh/2023/gsctool.html)

To load the demo `GSC` plugin, copy `gsctool` to the game's root directory.

This demo will give you a `raygun` on spawn in `SP` zombie mode. 

* `GSC` dumps are written to `gsctool/cache`.

> Note: This project is intended to be a starting point for more advanced `GSC` modding.

# Instructions

Run: `git submodule update --init --recursive` to clone submodules.

Then build project using the provided `makefile` in `WSL` (`Windows`)
and inject resulting `DLL`.

The compiler used for this project is [`tcc`](https://github.com/lunarjournal/tcc) (`Tiny C Compiler`).

> Note: You can modify and reload `GSC` scripts while `Black Ops` is running by quiting
the level and starting it again.

![Demo](https://raw.githubusercontent.com/lunarjournal/gsctool/main/images/demo.png)

`Demo Video`:

[![](https://img.youtube.com/vi/Gs9mHXyiGNg/0.jpg)](https://www.youtube.com/watch?v=Gs9mHXyiGNg)

Files:
<br/>
* `gsctool.h` - `GSC` loader definitions.
* `gsctool.c` - `core` implementation.
* `offsets.h` - `T5` function `offsets`.

# Dependencies

* [`cdl86`](https://github.com/lunarjournal/cdl86) - `x86_64` `detours` library.
* [`miniz`](https://github.com/lunarjournal/miniz) - `zlib` library.

# Signature

```
+---------------------------------------+
|   .-.         .-.         .-.         |
|  /   \       /   \       /   \        |
| /     \     /     \     /     \     / |
|        \   /       \   /       \   /  |
|         "_"         "_"         "_"   |
|                                       |
|  _   _   _ _  _   _   ___   ___ _  _  |
| | | | | | | \| | /_\ | _ \ / __| || | |
| | |_| |_| | .` |/ _ \|   /_\__ \ __ | |
| |____\___/|_|\_/_/ \_\_|_(_)___/_||_| |
|                                       |
|                                       |
| Lunar RF Labs                         |
| https://lunar.sh                      |
|                                       |
| RF Research Laboratories              |
| Copyright (C) 2022-2024               |
|                                       |
+---------------------------------------+
```
