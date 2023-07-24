#!/bin/bash
set -e

echo "*** [IA-32] Compiling C sources..."
i686-w64-mingw32-gcc -Wall -Wextra -O3 -std=c99 -c ./*.c

echo "*** [IA-32] Compiling resource.rc..."
i686-w64-mingw32-windres resource.rc resource.o

echo "*** [IA-32] Linking..."
i686-w64-mingw32-gcc ./*.o -static -s -lm -mwindows -municode -o ddi-32.exe

echo "*** [AMD64] Compiling C sources..."
x86_64-w64-mingw32-gcc -Wall -Wextra -O3 -std=c99 -c ./*.c

echo "*** [AMD64] Compiling resource.rc..."
x86_64-w64-mingw32-windres resource.rc resource.o

echo "*** [AMD64] Linking..."
x86_64-w64-mingw32-gcc ./*.o -static -s -lm -mwindows -municode -o ddi-64.exe

echo "OK"
