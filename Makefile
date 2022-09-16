CXX=/home/schnakkevene/opt/cross/bin/i686-elf-g++
AS=/home/schnakkevene/opt/cross/bin/i686-elf-as
LD=/home/schnakkevene/opt/cross/bin/i686-elf-g++

CRTIS=src/crt/crti.s
CRTI=build/crt/crti.o
CRTBEGIN=/home/schnakkevene/opt/cross/lib/gcc/i686-elf/12.2.0/crtbegin.o
CRTEND=/home/schnakkevene/opt/cross/lib/gcc/i686-elf/12.2.0/crtend.o
CRTN=build/crt/crtn.o
CRTNS=src/crt/crtn.s

CXXSRC=$(shell find src/ -type f -name '*.cxx')
ASSRC=$(shell find src -path src/crt -prune -o -name '*.s' -print)

all: buildinit buildas buildascrt buildcxx link

buildinit:
	@mkdir build/bin -p
buildas: $(ASSRC)
	@echo $^
	$(AS) -o build/$@ $^
buildascrt:
	$(AS) -o $(CRTI) $(CRTIS)
	$(AS) -o $(CRTN) $(CRTNS)
buildcxx: $(CXXSRC)
	$(CXX) -c $^ -o $@ -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
link:
	OBJ=$(shell find build/ -type f -name '*.o' -prune)
	$(LD) -T src/linker.ld -o build/bin/vienna.bin -ffreestanding -O2 -nostdlib $(CRTI) $(CRTBEGIN) $(OBJ) $(CRTEND) $(CRTN) -lgcc

.PHONY: clean
clean:
	@rm -rfv build

test:
	@echo CXX=$(CXX)
	@echo AS=$(AS)
	@echo LD=$(LD)
	@echo
	@echo CRTIS=$(CRTIS)
	@echo CRTI=$(CRTI)
	@echo CRTBEGIN=$(CRTBEGIN)
	@echo CRTEND=$(CRTEND)
	@echo CRTN=$(CRTN)
	@echo CRTNS=$(CRTNS)
	@echo
	@echo CXXSRC=$(CXXSRC)
	@echo ASSRC=$(ASSRC)