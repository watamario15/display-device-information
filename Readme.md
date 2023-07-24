# Display Device Information Readme

この文書には[日本語版](Readme_JA.md)もあります。

![](image.png)

This software tells you your every active display device's information. Multi-display environments are also supported. I made this software based on [this website](http://jag5.dreamlog.jp/archives/7949249.html).

## System Requirements

**Windows 2000 SP4 or later** (IA-32/AMD64/AArch32/AArch64)

## How to run

You can get executable files at "**Releases**". Then, please use one that matches your computer. The results will be displayed on a message box.

|   File Name   | Target Computer |
| :-----------: | :-------------: |
|  ddi-32.exe   |   x86 32-bit    |
|  ddi-64.exe   |   x86 64-bit    |
| ddi-arm32.exe |   Arm 32-bit    |
| ddi-arm64.exe |   Arm 64-bit    |

## How to install / uninstall

Just run an executable file directly. No installation needed. You can also uninstall it by just deleting the executable file. This software doesn't use registry or such.

## About source codes

This project is for **Visual Studio 2022**.

This project supports **ARM and ARM64**, and you must install build sets for them to build for them. Visual Studio will prompt you to install them if you haven't. You can still build for x64 and Win32 without ARM or ARM64 build sets.

You can also use MinGW_w64 instead. To do so, run `win.sh` or `win.bat` with the PATH correctly set.

The source code is "**main.cpp**", the resource script is "**resource.rc**", and the icon is "**app.ico**". You can edit and build by opening "**PrimeFactorization.sln**" in Visual Studio.

Executable binary files will be generated in the "**Release / Debug**" folder in "**Win32**" for x86 32-bit, "**x64**" for x86 64-bit, "**ARM**" for Arm 32-bit, and "**ARM64**" for Arm 64-bit. The "**Release**" folder is for release builds, and "**Debug**" folder is for debug builds.

## Notes

**THE AUTHOR OF THIS SOFTWARE WILL NOT TAKE ANY RESPONSIBILITY FOR ANY DAMAGES CAUSED BY THIS SOFTWARE.**

## Rights

I made this software by adding some new content to a program provided on [this website](http://jag5.dreamlog.jp/archives/7949249.html). I waive copyright on the parts I added, but please be noted that **the original author still holds copyrights for what they developed**.
