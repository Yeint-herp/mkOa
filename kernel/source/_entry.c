#include <idev/serial.h>

[[noreturn]] void _start() {
    serial_init(serial_combase1, 115200,
            kParityNone, false, 7);

    serial_write(serial_combase1, "Hello World\n");

    for (;;) ;
}
