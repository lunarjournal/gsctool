CC=tcc.exe
CFLAGS=-m32 -shared -lmsvcrt
CFILES_GSC=gsctool.c lib/cdl86/cdl.c lib/miniz/miniz.c

all: gsctool

gsctool: gsctool.c
	$(CC) $(CFLAGS) $(CFILES_GSC) -o gsctool.dll

.PHONY: clean
clean:
	rm -f gsctool.dll

