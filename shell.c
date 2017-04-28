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

struct buffer *
buffer_alloc(size_t size)
{
	struct buffer *b = malloc(sizeof(struct buffer));
	if (!b)
		return b;
	b->data = malloc(sizeof(uint8_t) * size);
	return b;
}

void
buffer_free(struct buffer *b)
{
	free(b->data);
	free(b);
}

// TODO: Specify rules for `indent`.
// TODO: Implement process spawning with clone().
int
main(int argc, char *argv[])
{
	struct buffer *b = buffer_alloc(13);
	memcpy(b->data, "Hello, world!", 13);
	printf("%s\n", b->data);
	buffer_free(b);

	return 0;
}
