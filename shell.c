#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

struct buffer
{
	size_t size;
	uint8_t *data;
};

void
buffer_free(struct buffer *b)
{
	free(b->data);
	free(b);
}

struct buffer *
buffer_alloc(size_t size)
{
	struct buffer *b = malloc(sizeof(struct buffer));
	if (!b)	return NULL;
	if (!(b->data = malloc(sizeof(uint8_t) * size))) {
		free(b);
		return NULL;
	}
	b->size = size;
	return b;
}

// Return NULL if a string is valid UTF-8 data, otherwise, return a pointer to
// the first malformed sequence.
uint8_t *
utf_validate(uint8_t *data)
{
	while (*data) {
		if (*data < 0x80) data++;
		else if ((data[0] & 0xE0) == 0xC0) {
			if ((data[1] & 0xC0) != 0x80 ||
				(data[0] & 0xFE) == 0xC)
				return data;
			else data += 2;
		}
		else if ((data[0] & 0xF0) == 0xE0) {
			if ((data[1] & 0xC0) != 0x80 ||
				(data[2] & 0xC0) != 0x80 ||
				(data[0] == 0xE0 && (data[1] & 0xE0) == 0x80) ||
				(data[0] == 0xED && (data[1] & 0xE0) == 0xA0) ||
				(data[0] == 0xEF && data[1] == 0xBF &&
				 (data[2] & 0xFE) == 0xBE))
				return data;
			else data += 3;
		}
		else if ((data[0] & 0xF8) == 0xF0) {
			if ((data[1] & 0xC0) != 0x80 ||
				(data[2] & 0xC0) != 0x80 ||
				(data[3] & 0xC0) != 0x80 ||
				(data[0] == 0xF0 && (data[1] & 0xF0) == 0x80) ||
				(data[0] == 0xF4 && data[1] > 0x8F) || data[0] > 0xF4)
				return data;
			else data += 4;
		}
		else return data;
	}
	return NULL;
}

// Expand all strings to their largest representation.
uint8_t *
utf_decompose(uint8_t *data)
{
	return NULL;
}

// Return the next grapheme cluster.
uint8_t *
utf_next(uint8_t *data)
{
	return NULL;
}

// Return the number of vertical columns that the given string occupies.
size_t
utf_width(uint8_t *data)
{
	return 0;
}

// TODO: Specify rules for `indent`.
// TODO: Implement process spawning with clone().
int
main(int argc, char *argv[])
{
	if (utf_validate((uint8_t *)"κόσμε"))
		printf("Invalid.\n");
	else
		printf("Valid.\n");
	return 0;
}
