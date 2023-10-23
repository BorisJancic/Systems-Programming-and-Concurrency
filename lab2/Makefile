COMPILER = gcc       
COMPILER_FLAGS = -Wall -g -std=c99 -c -D_GNU_SOURCE
LINKER = gcc       


#======== for "make all" =========================================================================
all: paster.out #all executables should be listed here



#======= for each executable, link together relevant object files and libraries==================
#target: ingredients
	#recipe

paster.out: paster.o catpng.o crc.o zutil.o header.o
	$(LINKER) -o paster.out 	paster.o catpng.o crc.o zutil.o header.o -lz -lcurl -pthread

#more executables ...
  
  
  
  
#======= for each object file, compile it from source  ===========================================
#target: ingredients
	#recipe

paster.o: ./starter/Paster/paster.c
	$(COMPILER) $(COMPILER_FLAGS) -o paster.o       ./starter/Paster/paster.c 

catpng.o: ./starter/Paster/catpng.c ./starter/Paster/catpng.h
	$(COMPILER) $(COMPILER_FLAGS) -o catpng.o        ./starter/Paster/catpng.c

crc.o: ./starter/Paster/crc.c ./starter/Paster/crc.h
	$(COMPILER) $(COMPILER_FLAGS) -o crc.o          ./starter/Paster/crc.c 

zutil.o: ./starter/Paster/zutil.c ./starter/Paster/zutil.h 
	$(COMPILER) $(COMPILER_FLAGS) -o zutil.o        ./starter/Paster/zutil.c 

header.o: ./starter/Paster/main_write_header_cb.c ./starter/Paster/main_write_header_cb.h
	$(COMPILER) $(COMPILER_FLAGS) -o header.o       ./starter/Paster/main_write_header_cb.c 

#======== for "make clean" ========================================================================
.PHONY: clean
clean:
	rm -f *.d *.o *.out



