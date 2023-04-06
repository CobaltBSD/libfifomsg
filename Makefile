all:
	$(CC) -shared -fPIC fifomsg.c -o libfifomsg.so
	$(CC) -c fifomsg.c -o libfifomsg.o
	ar rcs libfifomsg.a libfifomsg.o
	
	# Utility programs
	$(CC) fifo_listen.c -L$(PWD) -I$(PWD) -lfifomsg -lmsgpack-c -o fifo_listen
	$(CC) fifo_send.c -L$(PWD) -I$(PWD) -lfifomsg -lmsgpack-c -o fifo_send

clean:
	rm -fv libfifomsg.so libfifomsg.a libfifomsg.o fifo_listen fifo_send

install:
	mkdir -pv $(DESTDIR)/usr/include
	mkdir -pv $(DESTDIR)/usr/lib
	mkdir -pv $(DESTDIR)/usr/bin
	cp -fv fifomsg.h $(DESTDIR)/usr/include
	cp -fv libfifomsg.so libfifomsg.a $(DESTDIR)/usr/lib
	cp -fv fifo_listen fifo_send $(DESTDIR)/usr/bin
