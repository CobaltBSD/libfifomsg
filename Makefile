all:
	gcc -shared -fPIC fifomsg.c -o libfifomsg.so
	gcc -c fifomsg.c -o libfifomsg.o
	ar rcs libfifomsg.a libfifomsg.o
	# Utility programs
	gcc fifo_listen.c -L$(PWD) -I$(PWD) -lfifomsg -lmsgpack-c -o fifo_listen
	gcc fifo_send.c -L$(PWD) -I$(PWD) -lfifomsg -lmsgpack-c -o fifo_send

clean:
	rm -f libfifomsg.so libfifomsg.a libfifomsg.o fifo_listen fifo_send

install:
	mkdir -pv $(DESTDIR)/usr/include
	mkdir -pv $(DESTDIR)/usr/lib
	mkdir -pv $(DESTDIR)/usr/bin
	cp -f fifomsg.h $(DESTDIR)/usr/include
	cp -f libfifomsg.so libfifomsg.a $(DESTDIR)/usr/lib
	cp -f fifo_listen fifo_send $(DESTDIR)/usr/bin
