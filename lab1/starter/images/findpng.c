//#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>

unsigned find_PNG(const char* dir_path);
bool check_PNG_header(char* buf, long int len);
bool valid_directory(const char* dir_path);

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Total of 2 args required (1 dir name)\n");
		return -1;
	}
	if (!valid_directory(argv[1])) { return -1; }
	
	unsigned PNG_count = 0;

	unsigned i = 0;
	while (argv[1][i])
			i++;
	if (i == 0) { printf("Empty arg\n"); return -1; }

	if (argv[1][i-1] != '/') {
		char* path = (char*)malloc((i + 2)*sizeof(char));
		if (!path) { printf("Malloc Err\n"); return -1; }

		i = 0;
		while (argv[1][i]) {
				path[i] = argv[1][i];
				++i;
		}
		path[i] = '/';
		path[i + 1] = '\0';
		
		PNG_count = find_PNG(path);
		free(path);
	} else {
		PNG_count = find_PNG(argv[1]);
	}

	if (!PNG_count)
			printf("findpng: No PNG file found\n");

	return 0;
}

unsigned find_PNG(const char* dir_path) {
    DIR* directory;
    struct dirent *dp;
    char* cur_entry;
	unsigned count = 0;

	directory = opendir(dir_path);
	if (directory == NULL) { printf("%s: NULL\n", dir_path); return count; }

    while((dp = readdir(directory)) != NULL){
        cur_entry = dp->d_name;
	
		char* entry_path;
        unsigned string_length = 0;
        unsigned i = 0;
        unsigned j = 0;

        while (cur_entry[i]) { ++string_length; ++i; }
        if (i == 1 && cur_entry[0] == '.')
      		continue;
        if (i == 2 && cur_entry[0] == '.' && cur_entry[1] == '.')
        	continue;
        i = 0;
        while (dir_path[i]) { ++string_length; ++i; }
		i = 0;

        entry_path = (char*)malloc((string_length + 2)*sizeof(char));
        if (!entry_path) { printf("Malloc Err\n"); exit(-1); }
        entry_path[string_length] = '/';
		entry_path[string_length + 1] = '\0';

		while (dir_path[i]) {
            entry_path[j] = dir_path[i];
            ++i; ++j;
        } i = 0;
        while (cur_entry[i]) {
            entry_path[j] = cur_entry[i];
            ++i;++j;
        }
		
		if(dp->d_type == DT_REG) { 				// if File
			entry_path[string_length] = '\0';	// get rid of last /
			FILE* file = fopen(entry_path, "r");
      		if (file == NULL) {
					printf("NULL file: %s\n", entry_path);
					free(entry_path);		
					continue;
			}

			char* buffer;
      		long file_length;
 			size_t bytes_read;

      		fseek(file, 0, SEEK_END);
      		file_length = ftell(file);
      		fseek(file, 0, SEEK_SET);
  
      		buffer = (char *) malloc((file_length + 1)*sizeof(char));
       	 	if (!buffer) {
					printf("Malloc Err\n"); free(entry_path); exit(-1);
			}
			buffer[file_length] = '\0';
      		bytes_read = fread(buffer, 1, file_length, file);
			if ((long)bytes_read > file_length) {
					printf("File read length Err\n");
					free(buffer);
					free(entry_path);
					continue;
			}

			if (check_PNG_header(buffer, file_length)) {
					printf("file: %s%s\n",dir_path, cur_entry);
					++count;
			}
			free(buffer);
			fclose(file);
		} else if(dp->d_type == DT_DIR) {		// if Dir
			count += find_PNG(entry_path);
		}
		free(entry_path);
    }
    closedir(directory);
	return count;
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

bool valid_directory(const char* dir_path) {
	struct stat sb;
	if (stat(dir_path, &sb) || !S_ISDIR(sb.st_mode)) {
    	printf("%s: not a valid directory\n", dir_path);
    	return false;
	} else { return true; }
}











