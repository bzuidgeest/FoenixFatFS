@echo off
cls

rem for hardware
wdc816as -DUSING_816 -DLARGE -V -L fxloader.asm -O fxloader.obj

rem wdc816cc -ML ata.c
rem wdc816cc -ML sd.c
rem wdc816cc -ML diskio.c
rem wdc816cc -ML ff.c
wdc816cc -ML test.c
rem wdcln -HIE -T  -P00 ata.obj sd.obj diskio.obj ff.obj test.obj fxloader.obj -L../foenixLibrary/FMX -LML -LCL -O test.hex -C10000  -D20000
wdcln -HIE -T  -P00 test.obj fxloader.obj -L../foenixLibrary/FMX -LML -LCL -O test.hex -C10000  -D20000


