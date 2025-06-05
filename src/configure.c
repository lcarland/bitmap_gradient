#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "main.h"

// Parse Keyvalues by either '=' or ':'
int keyValParse(char *input, char *keyBuffer, char *valBuffer) {
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

// configure image generator with file params
int configure(FILE *file, Configuration *config) {
    char line[64];
    char key[24];
    char value[24];

    memset(key, 0, sizeof(key));
    memset(value, 0, sizeof(value));

    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0 || line[0] == '#') {
            continue;
        }
        keyValParse(line, key, value);

        if (strcmp(key, "gradient_direction") == 0) {
            strcpy(config->gradient_direction, value);
        }
        else if (strcmp(key, "fileName") == 0) {
            strcpy(config->fileName, value);
        }
        else if (strcmp(key, "height") == 0) {
            config->height = atoi(value);
        }
        else if (strcmp(key, "width") == 0) {
            config->width = atoi(value);
        }
        else if (strcmp(key, "redStart") == 0) {
            config->redStart = (uint8_t) atoi(value);
        }
        else if (strcmp(key, "greenStart") == 0) {
            config->greenStart = (uint8_t) atoi(value);
        }
        else if (strcmp(key, "blueStart") == 0) {
            config->blueStart = (uint8_t) atoi(value);
        }
        else if (strcmp(key, "redTarget") == 0) {
            config->redTarget = (uint8_t) atoi(value);
        }
        else if (strcmp(key, "greenTarget") == 0) {
            config->greenTarget = (uint8_t) atoi(value);
        }
        else if (strcmp(key, "blueTarget") == 0) {
            config->blueTarget = (uint8_t) atoi(value);
        }

        memset(key, 0, sizeof(key));
        memset(value, 0, sizeof(value));
    }

    return 0;
}

