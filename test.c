#include <stdio.h>
#include <string.h>

// testing spliting strings by delimiters

int keyValParse(char* input, char delimeter, char *keyBuffer, char *valBuffer) {

	int delim_pos;
	for (int i = 0; i < strlen(input); i++) {
		if (input[i] == delimeter) {
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

typedef struct
{
	char name[16];
	char type[16];
	char status[16];
} Animal;


int main(void) {
	char list[3][20] = {
		"name=Pearl",
		"type=cat",
		"status=crazy"
	};

	Animal _animal = {
		.name = "",
		.type = "",
		.status = "",
	};

	char key[16];
	char value[24];
	for (int i = 0; i < 3; i++) {	

		keyValParse(list[i], '=', key, value);

		if (strcmp(key, "name") == 0) {
			strcpy(_animal.name, value);
		}
		else if (strcmp(key, "type") == 0) {
			strcpy(_animal.type, value);
		}
		else if (strcmp(key, "status") == 0) {
			strcpy(_animal.status, value);
		}

		memset(key, 0, sizeof(key));
		memset(value, 0, sizeof(value));
	}

	printf("Name: %s; Type: %s; Status: %s\n", _animal.name, _animal.type, _animal.status);

	return 0;
}
