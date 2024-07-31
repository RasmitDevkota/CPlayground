CC = clang
LINKER = clang -fuse-ld=lld
LIBS = -lm -lgsl -lgslcblas -lnlopt
LDFLAGS = -shared -fPIC
CFLAGS  = -Wall -Wno-unused-command-line-argument -fPIC -O3 -g
ENZLLD = ${HOME}/Enzyme/enzyme/build/Enzyme/LLDEnzyme-18.so
ENZCLANG = ${HOME}/Enzyme/enzyme/build/Enzyme/ClangEnzyme-18.so
OUTPUT_NAME = playground.exe

enzyme: main.o helper.o
	$(LINKER) -flto $(LDFLAGS) main.o helper.o $(STATICS) -o $(OUTPUT_NAME) $(LIBS) -v -Wl,--load-pass-plugin=$(ENZLLD)

main.o : main.c
	$(CC) $(CFLAGS) -c main.c

helper.o : helper.c
	$(CC) $(CFLAGS) -c helper.c -fpass-plugin=$(ENZCLANG) -Xclang -load -Xclang $(ENZCLANG)

.PHONY : clean
clean :
	rm -f *.o $(SHARED_LIB_NAME)

