#include<fifomsg.h>
#include<unistd.h>

#define USAGE "Usage: fifo_send service_name [-i int] [-b bool] [-f float] [-s string]"

int main(int argc, char *argv[]) {
	if(argc <= 2) {
		(void) puts(USAGE);
	}

	msgpack_sbuffer sbuf;
	msgpack_packer pk;

	msgpack_sbuffer_init(&sbuf);
	msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

	(void) msgpack_pack_array(&pk, (argc - 2) / 2); // Normally this might be bad practice, but it can be guaranteed that every parameter is associated with an argument

	int opt;
	while((opt = getopt(argc - 1, argv + 1, "i:b:f:s:")) != -1) {
		switch(opt) {
			case 'i':
				(void) msgpack_pack_int(&pk, atoi(optarg));
				break;
			case 'b':
				if(atoi(optarg) == 1) (void) msgpack_pack_true(&pk);
				else (void) msgpack_pack_false(&pk);
				break;
			case 'f':
				(void) msgpack_pack_float(&pk, atof(optarg));
				break;
			case 's':
				(void) msgpack_pack_str(&pk, strlen(optarg) + 1);
				(void) msgpack_pack_str_body(&pk, optarg, strlen(optarg) + 1);
				break;
			default:
				(void) fputs(USAGE "\n", stderr);
				return 1;
		}
	}

	void *b = malloc(sbuf.size);
	(void) memcpy(b, sbuf.data, sbuf.size);

	fifomsg_send(argv[1], b, sbuf.size);

	return 0;
}
