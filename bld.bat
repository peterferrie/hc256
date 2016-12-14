@echo off
del *.obj *.bin *.exe
yasm -fbin -DBIN hx.asm -ohx.bin
yasm -fwin32 hx.asm -ohx.obj
cl /nologo /DUSE_ASM /O2 /Os /GS- test.c hx.obj
del *.obj