#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../png_util/crc.h"

bool check_PNG_header(char* buf, long len);
int get_PNG_width(char* buf, long len);
int get_PNG_height(char* buf, long len);

bool check_CRC_error(char* buf, long len);
int get_chunk_data_length(char* buf, char* chunk, long len);
char* get_chunk_pointer(char*buf, long len, unsigned chunk_type);
unsigned get_CRC(char* buf, char* chunk, long len);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Error: insert file name\n");
        return -1;
    }

    char* file_name = argv[1];
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("No PNG file found\n");
        return -1; 
    }

	bool dbg = false;

	char* buffer;
	long file_length;
	size_t bytes_read;
	
	bool is_PNG;
	int width_PNG;
	int height_PNG;
	
	fseek(file, 0, SEEK_END);
	file_length = ftell(file);
	fseek(file, 0, SEEK_SET);

	buffer = (char *) malloc((file_length + 1)*sizeof(char));
	if (buffer == NULL) { printf("Malloc Error\n"); return -1; }
	buffer[file_length] = '\0';

	bytes_read = fread(buffer, 1, file_length, file);
	if ((long)bytes_read != file_length) { printf("Fread Error\n"); return -1; }

	is_PNG = check_PNG_header(buffer, file_length);
	if (!is_PNG) {
		printf("%s: Not a PNG file\n", file_name);
		free(buffer); return -1;
	}
	

	printf("Bit depth: %x\n", buffer[24]);


	return 0;

	width_PNG = get_PNG_width(buffer, file_length);
	height_PNG = get_PNG_height(buffer, file_length);
	
	if (dbg) {
		printf("File Length: %ld\n", file_length);
	}
	printf("%s: %d x %d\n", file_name, width_PNG, height_PNG);

	char* p_IHDR = get_chunk_pointer(buffer, file_length, 0);
    char* p_IDAT = get_chunk_pointer(buffer, file_length, 1);
    char* p_IEND = get_chunk_pointer(buffer, file_length, 2);
		
	if (dbg) {
		printf("buf  pointer: %p\n", (void*)buffer);
	    printf("IHDR pointer: %p\n", (void*)get_chunk_pointer(buffer, file_length, 0));
	    printf("IDAT pointer: %p\n", (void*)get_chunk_pointer(buffer, file_length, 1));
	    printf("IEND pointer: %p\n", (void*)get_chunk_pointer(buffer, file_length, 2));
	}
	if (p_IHDR == NULL || p_IDAT == NULL || p_IEND == NULL || !(p_IHDR < p_IDAT &&
		p_IDAT < p_IEND && p_IEND < (char*)(buffer + file_length))) {
			printf("Pointers are not aligned properly\n");
			free(buffer); return -1;
	}

    unsigned long IHDR_len = get_chunk_data_length(buffer, p_IHDR, file_length);
    unsigned long IDAT_len = get_chunk_data_length(buffer, p_IDAT, file_length);
    unsigned long IEND_len = get_chunk_data_length(buffer, p_IEND, file_length);
	
	if (dbg) {
		printf("IHDR_len: %lu\n", IHDR_len);
		printf("IDAR_len: %lu\n", IDAT_len);
		printf("IEND_len: %lu\n", IEND_len);
	}
	if (IHDR_len < 0 || IDAT_len < 0 || IEND_len < 0 ||
		IHDR_len >= (unsigned)file_length || IDAT_len >= (unsigned)file_length ||
		IEND_len >= (unsigned)file_length) {
			printf("Chunk lengths are not aligned properly\n");
			free(buffer); return -1;
	}
	
	unsigned read_IHDR_CRC = get_CRC(buffer, p_IHDR, file_length);
	unsigned read_IDAT_CRC = get_CRC(buffer, p_IDAT, file_length);
    unsigned read_IEND_CRC = get_CRC(buffer, p_IEND, file_length);

	if (dbg) {
		printf("read_IHDR_CRC: %u\n", read_IHDR_CRC);
		printf("read_IDAT_CRC: %u\n", read_IDAT_CRC);
		printf("read_IEND_CRC: %u\n", read_IEND_CRC);	
	}

	unsigned calc_IHDR_CRC = crc((unsigned char*)(p_IHDR + 4), IHDR_len + 4);
    unsigned calc_IDAT_CRC = crc((unsigned char*)(p_IDAT + 4), IDAT_len + 4);
	unsigned calc_IEND_CRC = crc((unsigned char*)(p_IEND + 4), IEND_len + 4);
	
	if (dbg) {
		printf("calc_IHDR_CRC: %u\n", calc_IHDR_CRC);
		printf("calc_IDAT_CRC: %u\n", calc_IDAT_CRC);
	    printf("calc_IEND_CRC: %u\n", calc_IEND_CRC);
	}

	bool CRC_Error = false;
	if (read_IHDR_CRC != calc_IHDR_CRC) {
		printf("IHDR chunk CRC error: computed %X, expected %X\n", calc_IHDR_CRC, read_IHDR_CRC);
		CRC_Error = true;
	}
	if (read_IDAT_CRC != calc_IDAT_CRC) {
        printf("IDAT chunk CRC error: computed %X, expected %X\n", calc_IDAT_CRC, read_IDAT_CRC);
		CRC_Error = true;
	}
	if (read_IEND_CRC != calc_IEND_CRC) {
    	printf("IEND chunk CRC error: computed %X, expected %X\n", calc_IEND_CRC, read_IEND_CRC);
		CRC_Error = true;
	}
	if (CRC_Error) { free(buffer); return -1; }
	
	free(buffer);
	return 0;
}

bool check_PNG_header(char* buf, long int len) {
	if (len < 8) { return false; }
	const char png_check[] = "PNG";
	for (int i = 0; i < 3; i++) {
		if (buf[i+1] != png_check[i])
			return false;
	}
	return true;
}


int get_chunk_data_length(char* buf, char* chunk,  long len) {
	if (chunk - buf + 4 >= len || chunk == NULL || buf == NULL) {
		return -1;
	}
	return htonl(*(unsigned*)chunk);
}

int get_PNG_height(char* buf, long len) {
   	int offset = 20;
   	if (len < 24 || buf == NULL) { return -1; }
    return htonl(*(unsigned*)&buf[0 + offset]);
}

int get_PNG_width(char* buf, long len) {
    int offset = 16;
    if (len < 20 || buf == NULL) { return -1; }
	return htonl(*(unsigned*)&buf[0 + offset]);
}

char* get_chunk_pointer(char*buf, long len, unsigned chunk_type){
	if (chunk_type >= 3) { printf("Chunk type 0, 1, 2\n"); return NULL; }
		
	long int current_pos = 0;
	int png_data_len = 0;

	current_pos += 8;

	if (chunk_type == 0) {   // reach IHDR
		if (current_pos >= len) { printf("IHDR Exceeds len\n"); return NULL; }
		return &buf[current_pos];
	}
    current_pos += 4 + 4 + 13 + 4;

	if (chunk_type == 1) {  //reach IDAT
        if (current_pos >= len) { printf("IDAT Exceeds len\n"); return NULL; }
        return (char*)buf + current_pos;
    }

    png_data_len = get_chunk_data_length(buf, (char*)(buf + current_pos), len);
	if (png_data_len < 0) { printf("Chunk Exceeds len\n"); return NULL; }

	current_pos += 4 + 4 + png_data_len + 4;
		
	if (current_pos >= len) { printf("IEND Exceeds len\n"); return NULL; }
    return &buf[current_pos]; //reach IEND
}

unsigned get_CRC(char* buf, char* chunk, long len) {
	long offset = (long)( (char*)chunk - (char*)buf );
	if (len < offset || chunk < buf || buf == NULL || chunk == NULL)
		return -1;

	unsigned length = get_chunk_data_length(buf, chunk, len);
	char* crc_pointer = &chunk[4 + 4 + length];
	if ((crc_pointer + 3) > &buf[len-1]) { return -1; }

	return htonl(*(unsigned*)crc_pointer);
}





