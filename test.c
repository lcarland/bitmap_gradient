#include <stdio.h>

// testing spliting strings by delimiters

int main(void) {
	char list[3][20] = {
		"name=Pearl",
		"type=cat",
		"status=crazy"
	};

	for (int i = 0; i < 3; i++) {
		char key[16];
		char value[24];
		int value_start;

		for (int c = 0; c < sizeof(list[i]); c++) {
			char ch = list[i][c];
			if (ch == 0) {
				continue;
			}
			else if (ch == '=') {
				value_start = c + 1;
				break;
			}
			key[c] = ch;
		}
		int val_index = 0;
		for (int c = value_start; c < sizeof(list[i]); c++){
			value[val_index] = list[i][c];
			val_index++;
		}

		printf("Key: %s\n",key);
		printf("Value: %s\n", value);
	}
}