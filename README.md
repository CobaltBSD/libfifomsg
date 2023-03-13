# libfifomsg

A simple interprocess communication library using FIFOs and msgpack-c. Used by Cobalt's userspace programs as a replacement for the overthought and insecure D-Bus.

## `fifo_send` and `fifo_listen`
This library also contains the supplementary utilities `fifo_send` and `fifo_listen`. Together, these utilities enable command-line usage of this library.

An example of using these together is as follows:
### Terminal 1:
`fifo_listen NotificationListener /usr/bin/echo`

### Terminal 2:
`fifo_send NotificationListener -s "My notification" -s "Sample text"

This library is feature-complete, but still an early work-in-progress. Expect bugs.
