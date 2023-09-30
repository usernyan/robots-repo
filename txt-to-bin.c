#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Wrong number of args\n");
		exit(EXIT_FAILURE);
	}
	FILE *txt_file = fopen(argv[1], "r");
	FILE *bin_file = fopen(argv[2], "wb");
	if (txt_file == NULL || bin_file == NULL) {
		fprintf(stderr, "can't open files\n");
		exit(EXIT_FAILURE);
	}

	bool done_reading = false;
	char s[2];
	uint16_t to_write = 0U;
	int bit_num = 0;
	bool comment_mode = false;
	uint16_t cur_mask = 0U;
	while( fscanf(txt_file, "%c", s) != EOF ) {
		bool is_1 = s[0] == '1';
		bool is_0 = s[0] == '0';
		bool is_hash = s[0] == '#';
		bool is_nl = s[0] == '\n';
		if (is_nl)
			comment_mode = false;
		if (is_hash)
			comment_mode = true;

		if (comment_mode)
			continue;
		//
		
		if (is_1)
			cur_mask = 1U;
		else if (is_0)
			cur_mask = 0U;

		if (is_1 || is_0) {
			to_write = to_write << 1;
			to_write = to_write | cur_mask;
			bit_num++;
		}

		if (bit_num == 16) {
			fwrite(&to_write, sizeof(to_write), 1, bin_file);
			bit_num = 0;
		}
	}

	fclose(txt_file);
	fclose(bin_file);

}
