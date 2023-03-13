#include<fifomsg.h>

#include<unistd.h>

char *prog;

void handler(msgpack_unpacked msg) {
	char **args = malloc(sizeof(char*));
	args[0] = prog;

	msgpack_object root = msg.data;
	int i;
	for(i = 0; i < root.via.array.size; ++i) {
		args = realloc(args, sizeof(char*) * (i + 3));
		int sz;
		char *z = NULL;

		switch(root.via.array.ptr[i].type) {
			case MSGPACK_OBJECT_POSITIVE_INTEGER:
				sz = snprintf(NULL, 0, "%d", root.via.array.ptr[i].via.u64);
				args[i + 1] = malloc(sz + 1);
				(void) sprintf(args[i + 1], "%d", root.via.array.ptr[i].via.u64);
				break;
			case MSGPACK_OBJECT_NEGATIVE_INTEGER:
				sz = snprintf(NULL, 0, "%d", root.via.array.ptr[i].via.i64);
				args[i + 1] = malloc(sz + 1);
				(void) sprintf(args[i + 1], "%d", root.via.array.ptr[i].via.i64);
				break;
			case MSGPACK_OBJECT_FLOAT32:
			case MSGPACK_OBJECT_FLOAT64:
				sz = snprintf(NULL, 0, "%f", root.via.array.ptr[i].via.f64);
				args[i + 1] = malloc(sz + 1);
				(void) sprintf(args[i + 1], "%f", root.via.array.ptr[i].via.f64);
				break;
			case MSGPACK_OBJECT_BOOLEAN:
				sz = snprintf(NULL, 0, "%b", root.via.array.ptr[i].via.boolean);
				args[i + 1] = malloc(sz + 1);
				(void) sprintf(args[i + 1], "%b", root.via.array.ptr[i].via.boolean);
				break;
			case MSGPACK_OBJECT_STR:
				sz = snprintf(NULL, 0, "%s", root.via.array.ptr[i].via.str.ptr);
				args[i + 1] = malloc(sz + 1);
				(void) sprintf(args[i + 1], "%s", root.via.array.ptr[i].via.str.ptr);
				break;
			case MSGPACK_OBJECT_NIL:
			case MSGPACK_OBJECT_ARRAY:
			case MSGPACK_OBJECT_MAP:
			case MSGPACK_OBJECT_BIN:
			case MSGPACK_OBJECT_EXT:
			default:
				(void) fputs("unsupported type\n", stderr);
		}
	}
	args[i + 1] = NULL;

	if(fork() == 0) {
		execv(prog, args);
	}
}

int main(int argc, char *argv[]) {
	if(argc != 3) {
		(void) fputs("Usage: fifo_listen service_name prog", stderr);
	}

	prog = argv[2];

	fifomsg_listen(argv[1], &handler);

	return 0;
}
