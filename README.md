# gsctool

Simple GSC loader and dumper for `Call Of Duty: Black Ops 1 (T5) (Microsoft Windows)`.

[https://journal.lunar.sh/2023/gsctool.html](https://journal.lunar.sh/2023/gsctool.html)

To load the demo GSC plugin copy `gsctool` to the games root directory.

This demo will give you a raygun on spawn in zombie mode (solo). 

* GSC dumps are written to `gsctool/cache`

This project is intended to be a starting point for more advanced mods.

# Instructions

Run: `git submodule update --init --recursive` to clone submodules.

Then build project using the provided makefile in Windows Subsystem for Linux
and inject resulting DLL.

The compiler used for this project is [tcc](https://github.com/lunarjournal/tcc) (`Tiny C Compiler`).

Note: You can modify and reload GSC scripts while Black Ops is running by quiting
the level and starting it again.

![Demo](https://raw.githubusercontent.com/lunarjournal/gsctool/main/images/demo.png)

# Dependencies

* [cdl86](https://github.com/lunarjournal/cdl86) - `x86 detours library`
* [miniz](https://github.com/lunarjournal/miniz) - `zlib library`
