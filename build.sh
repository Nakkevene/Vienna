#!/bin/bash
CXX=/home/schnakkevene/opt/cross/bin/i686-elf-g++
AS=/home/schnakkevene/opt/cross/bin/i686-elf-as
LD=/home/schnakkevene/opt/cross/bin/i686-elf-g++

init () {
	echo Manipulating directories...
    rm -rf build
    mkdir -p build/bin
    mkdir build/crt
}

assembly() {
	echo Assembling...
    $AS -o build/boot.o src/boot.s
    $AS -o build/crt/crti.o src/crt/crti.s
    $AS -o build/crt/crtn.o src/crt/crtn.s
    cp /home/schnakkevene/opt/cross/lib/gcc/i686-elf/12.2.0/crtbegin.o build/crt/
    cp /home/schnakkevene/opt/cross/lib/gcc/i686-elf/12.2.0/crtend.o build/crt/
}

cxx() {
	echo Compiling...
    Files=$(find src/ -type f -name '*.cxx')
    for File in $Files
    do
        $CXX -c "$File" -o build/"${File##*/}".o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
    done
}

link() {
	echo Linking...
    Files=$(find build/ -name '*.o' -not -path 'build/crt/*')
	Files2=
	for File in $Files
	do
		Files2+="$File "
	done
    $LD -T src/linker.ld -o build/bin/vienna.bin -lgcc -ffreestanding -O2 -nostdlib build/crt/crti.o build/crt/crtbegin.o $Files build/crt/crtend.o build/crt/crtn.o
}

qemuwindows() {
	qemu-system-i386.exe -kernel build/bin/vienna.bin
}

echo "-- Init --"
init
echo "-- Assembly --"
assembly
echo "-- CXX --"
cxx
echo "-- Link --"
link
echo "-- Qemu --"
qemuwindows