@echo off
cls

rem for hardware
wdc816as -DUSING_816 -DLARGE -V -L fxloader.asm -O fxloader.obj

wdc816cc -ML ata.c
wdc816cc -ML sd.c
wdc816cc -ML diskio.c
wdc816cc -ML ff.c
wdc816cc -ML test.c
wdcln -HIE -T  -P00 ata.obj sd.obj diskio.obj ff.obj test.obj fxloader.obj -L../foenixLibrary/FMX -LML -LCL -O test.hex -C10000  -D20000
rem wdcln -HIE -T  -P00 test.obj fxloader.obj -L../foenixLibrary/FMX -LML -LCL -O test.hex -C10000  -D20000

rem for simulator
rem wdc816cc -ML printf.c -bs
rem wdcln -HZ -G -V -T -P00 printf.obj c0l.obj -LCL -O printf.bin

rem output assembly
wdc816cc -ML diskio.c -AT
