#pragma once

#include<msgpack.h>

typedef void (*fifo_handler)(msgpack_unpacked msg);

/* fifomsg_listen
 * Creates a FIFO and listens on it, returning received data to a specified file descriptor
 *
 * @service - Arbitrary name of the service to listen as
 * @fd - File descriptor to send output to
 */
void fifomsg_listen(char *service, fifo_handler handler);

/* fifomsg_send
 * Sends a message to the identified FIFO
 *
 * @service - Arbitrary name of the service to send data to
 * @buf - Buffer containing msgpack-serialized data to send over FIFO
 * @buf_sz - Size of the buffer
 */
void fifomsg_send(char *service, void *buf, size_t buf_sz);
