COMPILER = gcc       
COMPILER_FLAGS = -Wall -g -std=c99 -c -D_GNU_SOURCE
LINKER = gcc       


#======== for "make all" =========================================================================
all: findpng.out catpng.out #all executables should be listed here



#======= for each executable, link together relevant object files and libraries==================
#target: ingredients
	#recipe

findpng.out: findpng.o crc.o zutil.o
	$(LINKER) -o findpng.out 	findpng.o

catpng.out: catpng.o crc.o zutil.o
	$(LINKER) -o catpng.out     catpng.o crc.o zutil.o -lz  #-lz is the libz library


#more executables ...
  
  
  
  
#======= for each object file, compile it from source  ===========================================
#target: ingredients
	#recipe

findpng.o: ./starter/images/findpng.c
	$(COMPILER) $(COMPILER_FLAGS) -o findpng.o		./starter/images/findpng.c

catpng.o: ./starter/images/catpng.c
	$(COMPILER) $(COMPILER_FLAGS) -o catpng.o       ./starter/images/catpng.c 

crc.o: ./starter/png_util/crc.c ./starter/png_util/crc.h
	$(COMPILER) $(COMPILER_FLAGS) -o crc.o          ./starter/png_util/crc.c 

zutil.o: ./starter/png_util/zutil.c ./starter/png_util/zutil.h 
	$(COMPILER) $(COMPILER_FLAGS) -o zutil.o        ./starter/png_util/zutil.c 



#======== for "make clean" ========================================================================
.PHONY: clean
clean:
	rm -f *.d *.o *.out



