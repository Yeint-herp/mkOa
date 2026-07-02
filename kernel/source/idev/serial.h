#pragma once

#include <pio.h>

constexpr port_t serial_combase1 = 0x3F8;
constexpr port_t serial_combase2 = 0x2F8;

struct serialLayout {
    union {
        char recieve_buffer;
        char transmit_buffer;
        char divisor_lsb;
    };

    union {
       char ier;
       char divisor_msb;
    };

    union {
        char iid;
        char fifo_ctl;
    };

    char lcr;
    char mcr;
    char lsr;
    char msr;
    char scratch;
};

enum serialParity {
    kParityNone = 0x0,
    kParityOdd = 0x1,
    kParityEven = 0x3,
    kParityMark = 0x5,
    kParitySpace = 0x7
};

bool serial_init(port_t base, 
        int baud, 
        enum serialParity parity,
        bool more_stop_bits,
        unsigned char data_bits);

/// truncating for data_bits < 7.
void serial_put(port_t base, char data);

/// unsigned for data_bits = 8.
void serial_putu(port_t base, unsigned char data);

/// blocking write into the serial.
void serial_write(port_t base, const char* data);

