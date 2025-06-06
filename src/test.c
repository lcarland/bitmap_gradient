#include <stdio.h>
#include <string.h>

// testing spliting strings by delimiters

/*
 Parses key value pairs by either = or :
*/
int keyValParse(char* input, char *keyBuffer, char *valBuffer) {

	int delim_pos;
	for (int i = 0; i < strlen(input); i++) {
		if (input[i] == '=' || input[i] == ':') {
			delim_pos = i;
		}
	}

	for (int i = 0; i < delim_pos; i++) {
		keyBuffer[i] = input[i];
	}

	int val_index = 0;
	for (int i = delim_pos + 1; i < strlen(input); i++) {
		valBuffer[val_index] = input[i];
		val_index++;
	}

	return 0;
}


typedef struct {
	char name[16];
	char type[16];
	char status[16];
} Animal;

// Animal method, set animal properties one at a time
int setAnimal(Animal *_animal, char *key, char *value) {
	if (strcmp(key, "name") == 0) {
		strcpy(_animal->name, value);
	}
	else if (strcmp(key, "type") == 0) {
		strcpy(_animal->type, value);
	}
	else if (strcmp(key, "status") == 0) {
		strcpy(_animal->status, value);
	}
	return 0;
}


int main(int argc, char *argv[]) {
	char list[3][20] = {
		"name=Pearl",
		"type=cat",
		"status=crazy"
	};

	Animal animal;

	char key[16];
	char value[24];
	memset(key, 0, sizeof(key));
	memset(value, 0, sizeof(value));

	for (int i = 0; i < 3; i++) {	
		keyValParse(list[i], key, value);

		setAnimal(&animal, key, value);

		memset(key, 0, sizeof(key));
		memset(value, 0, sizeof(value));
	}

	// overwrite with args
	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			keyValParse(argv[i], key, value);
			setAnimal(&animal, key, value);

			memset(key, 0, sizeof(key));
			memset(value, 0, sizeof(value));
		}
	}


	printf("Name: %s; Type: %s; Status: %s\n", animal.name, animal.type, animal.status);

	return 0;
}
