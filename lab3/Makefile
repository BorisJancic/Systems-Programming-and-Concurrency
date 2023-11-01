COMPILER = gcc       
COMPILER_FLAGS = -Wall -g -std=c99 -c -D_GNU_SOURCE
LINKER = gcc -pthread


#======== for "make all" =========================================================================
all: paster2.out #all executables should be listed here



#======= for each executable, link together relevant object files and libraries==================
#target: ingredients
	#recipe

paster2.out: paster2.o stack.o decompress.o compress.o crc.o zutil.o header.o
	$(LINKER) -o paster2.out 	paster2.o stack.o decompress.o compress.o crc.o zutil.o header.o -lz -lcurl

#more executables ...
  
  
  
  
#======= for each object file, compile it from source  ===========================================
#target: ingredients
	#recipe

paster2.o: ./starter/src/paster2.c
	$(COMPILER) $(COMPILER_FLAGS) -o paster2.o       ./starter/src/paster2.c 

stack.o: ./starter/src/stack.c ./starter/src/stack.h
	$(COMPILER) $(COMPILER_FLAGS) -o stack.o       ./starter/src/stack.c 

decompress.o: ./starter/src/decompress.c ./starter/src/decompress.h
	$(COMPILER) $(COMPILER_FLAGS) -o decompress.o ./starter/src/decompress.c

compress.o: ./starter/src/compress_and_concat.c ./starter/src/compress_and_concat.h
	$(COMPILER) $(COMPILER_FLAGS) -o compress.o        ./starter/src/compress_and_concat.c

crc.o: ./starter/src/crc.c ./starter/src/crc.h
	$(COMPILER) $(COMPILER_FLAGS) -o crc.o          ./starter/src/crc.c 

zutil.o: ./starter/src/zutil.c ./starter/src/zutil.h 
	$(COMPILER) $(COMPILER_FLAGS) -o zutil.o        ./starter/src/zutil.c 

header.o: ./starter/src/main_write_header_cb.c ./starter/src/main_write_header_cb.h
	$(COMPILER) $(COMPILER_FLAGS) -o header.o       ./starter/src/main_write_header_cb.c 

#======== for "make clean" ========================================================================
.PHONY: clean
clean:
	rm -f *.d *.o *.out



