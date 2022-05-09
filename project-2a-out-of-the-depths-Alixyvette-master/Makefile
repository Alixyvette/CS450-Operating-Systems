CC:=gcc
AS:=as
LD:=ld
QEMU:=qemu-system-i386

QEMUOPTS:= 

KERN_LINK_SCRIPT:=kernel.ld

KERN_CFLAGS:= -ffreestanding -nostdlib -m32
KERN_LDFLAGS:= -T $(KERN_LINK_SCRIPT) -m elf_i386

bootloader.bin: bootloader.o
	$(LD) -m elf_i386 -Ttext 0x7c00 --oformat binary -o $@ $<

lowkern.o: lowkern.S
	$(CC) $(KERN_CFLAGS) -o $@ -c $<

lowkern.bin: lowkern.o
	$(LD) $(KERN_LDFLAGS) -o $@ $<

boot.o: boot.S
	$(CC) $(KERN_CFLAGS) -o $@ -c $<

kernel.o: kernel.c
	$(CC) $(KERN_CFLAGS) -o $@ -c $<

ckern.bin: boot.o kernel.o
	$(LD) $(KERN_LDFLAGS) -o $@ kernel.o boot.o

iso: lowkern.bin ckern.bin
	@cp lowkern.bin ckern.bin cdrom/boot/
	@grub-mkrescue -o p2kern.iso cdrom

bootloader.o: bootloader.S
	$(AS) --32 -o $@ $<

dump_bl: bootloader.bin
	@objdump -D -b binary -m i8086 $<

run-bl: bootloader.bin
	$(QEMU) -nographic -drive file=bootloader.bin,index=0,media=disk,format=raw $(QEMUOPTS) 

dump_lowkern: lowkern.bin
	@objdump -D -m i386 lowkern.bin

test: bootloader.bin
	@tests/runtests


handin: handin-check
	@echo "Handing in with git (this may ask for your GitHub username/password)..."
	@git push

handin-check:
	@if ! test -d .git; then \
		echo No .git directory, is this a git repository?; \
		false; \
	fi
	@if test "$$(git symbolic-ref HEAD)" != refs/heads/master; then \
		git branch; \
		read -p "You are not on the master branch.  Hand-in the current branch? [y/N] " r; \
		test "$$r" = y; \
	fi
	@if ! test -e info.txt; then \
		echo "You haven't created an info.txt file!"; \
		echo "Please create one with your name, email, etc."; \
		echo "then add and commit it to continue."; \
		false; \
	fi
	@if ! test -e bootloader.S; then \
		echo "No bootloader.S file found. Make sure to add and commit it";\
		false; \
	fi
	@if ! test -e lowkern.S; then \
		echo "No lowkern.S file found. Make sure to add and commit it";\
		false; \
	fi
	@if ! git diff-files --quiet || ! git diff-index --quiet --cached HEAD; then \
		git status -s; \
		echo; \
		echo "You have uncomitted changes.  Please commit or stash them."; \
		false; \
	fi
	@if test -n "`git status -s`"; then \
		git status -s; \
		read -p "Untracked files will not be handed in.  Continue? [y/N] " r; \
		test "$$r" = y; \
	fi


clean:
	@rm -f *.bin *.o *.log *.iso
