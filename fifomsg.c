#include"fifomsg.h"
#include<sys/stat.h>
#include<poll.h>
#include<unistd.h>
#include<fcntl.h>

void fifomsg_listen(char *service, fifo_handler handler) {
	char path[strlen(service) + 15];
	(void) sprintf(path, "/tmp/%s.service", service);

	(void) mkfifo(path, 0666); // Create FIFO first

	while(true) {
		int fd = open(path, O_RDONLY | O_NONBLOCK);

		struct pollfd ufds[1];
		ufds[0].fd = fd;
		ufds[0].events = POLLIN;

		(void) poll(ufds, 1, 3500);
		if(ufds[0].revents) {

			// Always assume the first size_t bits is the object length
			size_t *sz = malloc(sizeof(size_t));
			(void) read(fd, sz, sizeof(size_t));

			// Unpack
			msgpack_unpacked msg;
			msgpack_unpacked_init(&msg);

			uint8_t *buf = malloc(*sz);
			(void) read(fd, buf, *sz);

			// Also always assume that the root element is an array and that there are no nested arrays
			if(msgpack_unpack_next(&msg, buf, *sz, NULL)) {
				handler(msg);
			}

			msgpack_unpacked_destroy(&msg);
			free(buf);
			free(sz);
		}

		(void) close(fd);
	}
}

// Sends serialized data to a FIFO. The buffer represents a typical msgpack object
void fifomsg_send(char *service, void *buf, size_t buf_sz) {
	char path[strlen(service) + 15];
	(void) sprintf(path, "/tmp/%s.service", service);

	int fd = open(path, O_WRONLY);

	size_t newsize = buf_sz + sizeof(size_t);

	// A size_t length space is prefixed at the beginning of the buffer for storing buffer size, to ease parsing later
	unsigned char *b = malloc(buf_sz + sizeof(size_t));
	(void) memcpy(b, &newsize, sizeof(size_t));
	(void) memcpy(b + sizeof(size_t), buf, buf_sz);

	(void) write(fd, b, newsize);

	(void) close(fd);
}
