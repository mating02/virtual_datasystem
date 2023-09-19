#include "../lib/operations.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int find_parent_list(file_system* fs , char *path){
	if(path[0] != '/'){
		return -1;
	}
	int z = -1;
	int k = 0;
    const char s[] = "/";
    char* token;
	char* name;
    char* pathCopy = strdup(path);
    token = strtok(pathCopy, s);
    while(token != NULL) {
		z = -1;
		name = token;
        token = strtok(NULL, s);
		if(token != NULL){
		for(int j = k+1; j<fs->s_block->num_blocks; j++){
			if(strcmp(fs->inodes[j].name,name) == 0 && fs->inodes[j].n_type == 2){
				k = j;
				z = j;
				break;
			}
		}
		}
		else{
			for(int j = k+1; j<fs->s_block->num_blocks; j++){
				if(strcmp(fs->inodes[j].name,name) == 0){
					k = j;
					z = j;
					break;
				}
			}
		}
		if(z==-1 && token != NULL){
			free(pathCopy);
			return -1;
		}
    }
	free(pathCopy);
	return k;
}
int is_path_valid_and_find_parent(file_system* fs, char* path, int type){
	if(strcmp(path, "/") == 0){
		return -1;
	}
	int z = -1;
	int k = 0;
    const char s[] = "/";
    char* token;
	char* name;
    char* pathCopy = strdup(path);
    token = strtok(pathCopy, s);
	if(type == 2){
    while(token != NULL) {
		z = -1;
		name = token;
		for(int j = k+1; j<fs->s_block->num_blocks; j++){
			if(strcmp(fs->inodes[j].name,name) == 0 && fs->inodes[j].n_type == 2){
				k = j;
				z = j;
				break;
			}
		}
        token = strtok(NULL, s);
		if(z==-1 && token != NULL){
			free(pathCopy);
			return -1;
		}
		if(z != -1 && token == NULL){
			free(pathCopy);
			return -1;
		}
    }
	}
	if(type == 1){
		while(token != NULL) {
		z = -1;
		name = token;
        token = strtok(NULL, s);
		if(token != NULL){
		for(int j = k+1; j<fs->s_block->num_blocks; j++){
			if(strcmp(fs->inodes[j].name,name) == 0 && fs->inodes[j].n_type == 2){
				k = j;
				z = j;
				break;
			}
		}
		}
		if(z==-1 && token != NULL){
			free(pathCopy);
			return -1;
		}
		if(z != -1 && token == NULL){
			free(pathCopy);
			return -1;
		}
    }
	}
	if(type == 1){
		for(int h = 0; h < DIRECT_BLOCKS_COUNT; h++){
			if(fs->inodes[k].direct_blocks[h] != -1){
				if(strcmp(fs->inodes[fs->inodes[k].direct_blocks[h]].name, name) == 0 && fs->inodes[fs->inodes[k].direct_blocks[h]].n_type == type){
					free(pathCopy);
					return -2;
				}
			}
		}
	}
	if(k == 0){
		for(int h = 0; h < DIRECT_BLOCKS_COUNT; h++){
		if(fs->inodes[k].direct_blocks[h] != -1){
			if(strcmp(fs->inodes[fs->inodes[k].direct_blocks[h]].name, name) == 0 && fs->inodes[fs->inodes[k].direct_blocks[h]].n_type == type){
				free(pathCopy);
				return -1;
			}
		}
	}
	}
	free(pathCopy);
	return k;
}

int initiate_inodedata(int i, int type, file_system *fs, char *path){
	fs->inodes[i].n_type = type; // nType = dir
	int k = is_path_valid_and_find_parent(fs, path, type);

	if(k == -1 || k ==-2){
		fs->inodes[i].n_type = free_block;
		return k;
	}
	fs->s_block->free_blocks--;
	fs->inodes[i].parent = k;
	const char s[] = "/";
    char* token;
	char* name;
    char* pathCopy = strdup(path);
    token = strtok(pathCopy, s);
    while(token != NULL) {
		name = token;
		token = strtok(NULL, s);
	}
	strncpy(fs->inodes[i].name,name,NAME_MAX_LENGTH); // copy name
		for(int h = 0; h < 12; h++){ //kleinsmöglichin Direct_block finden und Index zuweisen
        if(fs->inodes[k].direct_blocks[h] == -1){
            fs->inodes[k].direct_blocks[h] = i;
			break;
        }
		}
	free(pathCopy);
	return 0;
}
int checkfile(file_system* fs, char* path){
	if(strcmp(path, "/") == 0){
		return -1;
	}
	if(path[0] != '/' || path == NULL){
		return -1;
	}
	int z = -1;
	int k = 0;
    const char s[] = "/";
    char* token;
	char* name;
    char* pathCopy = strdup(path);
    token = strtok(pathCopy, s);
    while(token != NULL) {
		z = -1;
		name = token;
        token = strtok(NULL, s);
		if(token != NULL){
		for(int j = k+1; j<fs->s_block->num_blocks; j++){
			if(strcmp(fs->inodes[j].name,name) == 0 && fs->inodes[j].n_type == 2){
				k = j;
				z = j;
				break;
			}
		}
		}
		else{
			for(int j = k+1; j<fs->s_block->num_blocks; j++){
				if(strcmp(fs->inodes[j].name,name) == 0){
					k = j;
					z = j;
					break;
				}
			}
		}
		if(z==-1 && token != NULL){
			free(pathCopy);
			return -1;
		}
    }
	if(k == 0){
		int f = -1;
		for(int h = 0; h < DIRECT_BLOCKS_COUNT; h++){
		if(fs->inodes[k].direct_blocks[h] != -1){
			if(strcmp(fs->inodes[fs->inodes[k].direct_blocks[h]].name, name) == 0 ){
				f = 0;
				break;
			}
		}
	}
	if( f == -1){
		return -1;
	}
	}
	free(pathCopy);
	return k;
}

int
fs_mkdir(file_system *fs, char *path)
{
	if(fs == NULL){
		return -1;
	}
	if(path == NULL || path[0] != '/'){
		return -1;
	}
	int i = find_free_inode(fs); // i = kleinster INode-Index
    if(i == -1){
        return -1;
    }
    return initiate_inodedata(i, 2, fs, path);
	
}



int
fs_mkfile(file_system *fs, char *path_and_name)
{
	if(fs == NULL){
		return -1;
	}
	if(path_and_name == NULL || path_and_name[0] != '/'){
		return -1;
	}
	int i = find_free_inode(fs); // i = kleinster INode-Index
    if(i == -1){
        return -1;
    }
	return initiate_inodedata(i, 1, fs, path_and_name);
}

char *
fs_list(file_system *fs, char *path)
{
	if(path == NULL){
		return NULL;
	}
	if(fs == NULL){
		return NULL;
	}
	int parent = find_parent_list(fs, path);
	if(parent == -1){
		return NULL;
	}
	
	char* list = malloc(1+NAME_MAX_LENGTH*12 * sizeof(char));
	list[0] = '\0';
	for (int i=0; i<fs->s_block->num_blocks; i++){
		if(fs->inodes[i].n_type==3){
			continue;
		}
		else if(fs->inodes[i].n_type==2 && fs->inodes[i].parent == parent){
			strcat(list, "DIR ");
			strcat(list, fs->inodes[i].name);
			strcat(list, "\n");
		}
		else if(fs->inodes[i].n_type==1 &&  fs->inodes[i].parent == parent){
			strcat(list, "FIL ");
			strcat(list, fs->inodes[i].name);
			strcat(list, "\n");
		}
	}
	// Check n_type 
	// if type == 1 -> FILE , else DIR
	//for first inode to last -> printf(name)
	return list;
}
void writebigdata(file_system *fs, char* text, int textsize, int k){
			int size = textsize;
			int b = 0;
		for(int h = 0; h < DIRECT_BLOCKS_COUNT; h++){
			if(size <= 0){
				return;
			}
			if(fs->inodes[k].direct_blocks[h] != -1){
				int j = fs->inodes[k].direct_blocks[h];
					if(fs->data_blocks[j].size < 1024){
						char substring[size + 1]; 
    					strncpy(substring, text + b, size);
    					substring[size] = '\0'; 
						int restsize = fs->data_blocks[j].size + size;
						strncat((char*)fs->data_blocks[j].block, substring, 1024 - fs->data_blocks[j].size);
						if(restsize <= 1024){
							fs->data_blocks[j].block[restsize] = '\0';
							fs->data_blocks[j].size += size;
							return;
						}
						else{
							fs->data_blocks[j].size = 1024;
							/*char rest[textsize];
        					strcpy(rest, text + 1024 - fs->data_blocks[j].size);
							rest[textsize] = '\0';
							*/
							b += 1024 - fs->data_blocks[j].size;
							size = textsize-(1024 - fs->data_blocks[j].size);
						}
					}
			}
			

		}
		if(size > 0){
			for(int h = 0; h < DIRECT_BLOCKS_COUNT; h++){
			if(size <= 0){
				return;
			}
		char substring[size + 1]; 
    			strncpy(substring, text + b, size);
    			substring[size] = '\0'; 
			for(int j = 0; j <fs->s_block->num_blocks; j++){
				if(fs->free_list[j] == 1){
					memcpy(fs->data_blocks[j].block, substring, size);
					if(size > 1024){
					fs->data_blocks[j].size = 1024;
					}
					else{
					fs->data_blocks[j].size += size;
					}
			    	fs->free_list[j] = 0;
					for(int h = 0; h < DIRECT_BLOCKS_COUNT; h++){
						if(fs->inodes[k].direct_blocks[h] == -1){
            				fs->inodes[k].direct_blocks[h] = j;
							break;
        				}
					}
					b += 1024;
					size = size-1024;
					fs->s_block->free_blocks -= 1;
					if(size <= 0){
						break;
					}
				}
			}
		}
		}
}

int
fs_writef(file_system *fs, char *filename, char *text)
{
	// FALLUNTERSCHEIDUNG:
	// Eingabe ist kleiner als 1024 -> 1 Data Block reicht
	// Eingabe ist größer -> mehrere Data Blöcke verwenden
	// Datei wurde schon beschrieben -> Text anhängen (strcat())
	if(filename == NULL || fs == NULL || text == NULL){
		return -1;
	}
	int k = checkfile(fs, filename);
	if(k == -1){
		return -1;
	}
	int textsize = strlen(text);
	if(textsize == 0){
		return 0;
	}
	if(fs->inodes[k].size == 0){
		int b = 0;
		int size = textsize;

	for(int j = 0; j <fs->s_block->num_blocks; j++){
			if(fs->free_list[j] == 1){
					memcpy(fs->data_blocks[j].block, text +b, size);
					if(size > 1024){
					fs->data_blocks[j].size = 1024;
					}
					else{
					fs->data_blocks[j].size += size;
					}
			    	fs->free_list[j] = 0;
					for(int h = 0; h < DIRECT_BLOCKS_COUNT; h++){
						if(fs->inodes[k].direct_blocks[h] == -1){
            				fs->inodes[k].direct_blocks[h] = j;
							break;
        				}
					}
					b += 1024;
					size = size-1024;
					fs->s_block->free_blocks -= 1;
					if(size <= 0){
						break;
					}
				}
			}
	}
	else if(fs->inodes[k].size == 12*1024 || fs->inodes[k].size + textsize >= 12*1024){
		return -2;
	}
	// Im Fall, dass man eine Datei anhängen will, die größer ist als der block -> anhängen und Rest in einen anderen Block schreiben ???
	else{
		writebigdata(fs,text,textsize,k);
	}
	fs->inodes[k].size += textsize;
	return textsize;
}

uint8_t *
fs_readf(file_system *fs, char *filename, int *file_size)
{
	if(fs == NULL || filename == NULL || file_size == NULL){
		return NULL;
	}
	int k = checkfile(fs, filename);
	if(k == -1){
		return NULL;
	}
	if(fs->inodes[k].size == 0){
		*file_size = 0;
		return NULL;
	}
	*file_size = fs->inodes[k].size;
	uint8_t* buffer = (uint8_t*)malloc(fs->inodes[k].size+1 * sizeof(uint8_t));
	int f = 0;
	for(int h = 0; h < DIRECT_BLOCKS_COUNT; h++){
		if(fs->inodes[k].direct_blocks[h] != -1){
            memcpy(buffer+f, fs->data_blocks[fs->inodes[k].direct_blocks[h]].block, fs->data_blocks[fs->inodes[k].direct_blocks[h]].size+1);
			f += 1024;
        }

	}
	return buffer;
}

void delete_inode(file_system* fs, int k){
	if(fs->inodes[k].parent != -1){
			for(int h = 0; h < DIRECT_BLOCKS_COUNT; h++){
				if(fs->inodes[fs->inodes[k].parent].direct_blocks[h] == k){
            		fs->inodes[fs->inodes[k].parent].direct_blocks[h] = -1;
        			}
				}
			}
	inode_init(&fs->inodes[k]);
}
void file_delete(file_system* fs, int k){
	for(int h = 0; h < DIRECT_BLOCKS_COUNT; h++){
			if(fs->inodes[k].direct_blocks[h] != -1){
            	size_t length = sizeof(fs->data_blocks[fs->inodes[k].direct_blocks[h]].block) / sizeof(fs->data_blocks[fs->inodes[k].direct_blocks[h]].block[0]);
    			memset(fs->data_blocks[fs->inodes[k].direct_blocks[h]].block, 0, length * sizeof(uint8_t));
				fs->data_blocks[fs->inodes[k].direct_blocks[h]].size = 0;
				fs->free_list[fs->inodes[k].direct_blocks[h]] = 1;
			}
		}
		fs->s_block->free_blocks++;
		delete_inode(fs, k);
}
void delete_recursive(file_system* fs, int k){
	for(int h = 0; h < DIRECT_BLOCKS_COUNT; h++){
			if(fs->inodes[k].direct_blocks[h] != -1){
				if(fs->inodes[fs->inodes[k].direct_blocks[h]].n_type == 1){
					file_delete(fs, fs->inodes[k].direct_blocks[h]);
				}
				else if(fs->inodes[fs->inodes[k].direct_blocks[h]].n_type == 2){
					//name der datei an path anhängen -> fs_rm(newpath), path wieder vom letzen stück befreien
					delete_recursive(fs, fs->inodes[k].direct_blocks[h]);
				}
			}
		}
		fs->s_block->free_blocks++;
		delete_inode(fs, k);
}
int
fs_rm(file_system *fs, char *path)
{
	if(path == NULL || fs == NULL){
		return -1;
	}
	int k = checkfile(fs, path);
	if(k == -1){
		return -1;
	}
	// 1.Fall : Inode ist Datei -> Directblocks und data_blocks freigeben, size anpassen, fertig
	if(fs->inodes[k].n_type == 1){
		file_delete(fs, k);
		return 0;
	}
	// 2. Fall: Inode ist Ordner-> Kinder suchen (parent = inode)->wenn keine Kinder mehr: Ordner freigeben-> Wenn Kinder: Schritt 2
	else if(fs->inodes[k].n_type == 2){
		delete_recursive(fs, k);
		return 0;
	}
	return -1;

}
int
fs_import(file_system *fs, char *int_path, char *ext_path)
{
	if(int_path == NULL || fs == NULL || ext_path== NULL){
		return -1;
	}
	FILE *ext_file = fopen(ext_path, "rb");
    if (ext_file == NULL) {
        return -1;
    }
	fseek(ext_file, 0, SEEK_END);
    long file_size = ftell(ext_file);
    fseek(ext_file, 0, SEEK_SET);

    // Allocate memory for the buffer to store the picture data
    uint8_t* buffer = (uint8_t*)malloc((file_size+1) * sizeof(uint8_t));
    if (buffer == NULL) {
        fclose(ext_file);
        return -1;
    }
      // Buffer to store the read data
	buffer[file_size] = '\0';
    // Read and print the data line by line
    size_t read_size = fread(buffer, sizeof(uint8_t), file_size, ext_file);
    if (read_size != file_size) {
        fclose(ext_file);
        free(buffer);
        return -1;
    }
	int k = checkfile(fs, int_path);
	if (k == -1){
        fclose(ext_file); // Schließe die externe Datei
        return -1;
    }
	int b = 0;
	int size = read_size;
	fs->inodes[k].size += size;
	for(int j = 0; j <fs->s_block->num_blocks; j++){
			if(fs->free_list[j] == 1){
					memcpy(fs->data_blocks[j].block, buffer +b, size);
					if(size > 1024){
					fs->data_blocks[j].size = 1024;
					}
					else{
					fs->data_blocks[j].size += size;
					}
			    	fs->free_list[j] = 0;
					for(int h = 0; h < DIRECT_BLOCKS_COUNT; h++){
						if(fs->inodes[k].direct_blocks[h] == -1){
            				fs->inodes[k].direct_blocks[h] = j;
							break;
        				}
					}
					b += 1024;
					size = size-1024;
					fs->s_block->free_blocks -= 1;
					if(size <= 0){
						break;
					}
				}
			}
	fclose(ext_file);
	free(buffer);
    return 0;
}
int
fs_export(file_system *fs, char *int_path, char *ext_path)
{
	if(int_path == NULL || fs == NULL || ext_path== NULL){
		return -1;
	}
	int x = checkfile(fs, int_path);
	if(x == -1){
		return -1;
	}
	FILE* source_file = fopen(fs->inodes[x].name, "rb");  // Open the source file in read mode
    if (source_file == NULL) {
        return -1;
    }

    FILE* destination_file = fopen(ext_path, "wb");  // Open the destination file in write mode
    if (destination_file == NULL) {
        fclose(source_file);
        return -1;
    }
	int file_size = -1;
	uint8_t* buffer = fs_readf(fs, int_path, &file_size);
	if (buffer == NULL && file_size != 0) {
        fclose(source_file);
		fclose(destination_file);
        return -1;
    }
    size_t bytes_read = fread(buffer, sizeof(uint8_t), file_size, source_file);
	if(bytes_read > 0){
        size_t bytes_written = fwrite(buffer, sizeof(uint8_t), bytes_read, destination_file);
        if (bytes_written != bytes_read) {
			fclose(source_file);
            fclose(destination_file);
			free(buffer);
            return -1;
        }
	}
    fclose(source_file);  // Close the source file
    fclose(destination_file);  // Close the destination file
	free(buffer);
    return 0;
}
