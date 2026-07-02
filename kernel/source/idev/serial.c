#include <pio.h>
#include <offsetof.h>
#include <idev/serial.h>

constexpr port_t rtb = offsetof(struct serialLayout, recieve_buffer);
constexpr port_t dlsb = offsetof(struct serialLayout, divisor_lsb);

constexpr port_t ier = offsetof(struct serialLayout, ier);
constexpr port_t dmsb = offsetof(struct serialLayout, divisor_msb);

constexpr port_t fctl = offsetof(struct serialLayout, fifo_ctl);

constexpr port_t lcr = offsetof(struct serialLayout, lcr);
constexpr port_t mcr = offsetof(struct serialLayout, mcr);
constexpr port_t lsr = offsetof(struct serialLayout, lsr);

static void enable_dlab(port_t base) {
    unsigned char v = inb(base + lcr);
    v |= 0x80;
    outb(base + lcr, v);
}

static void disable_dlab(port_t base) {
    unsigned char v = inb(base + lcr);
    v &= ~0x80;
    outb(base + lcr, v);
}

static void program_baud(port_t base,
        int baud)
{
    unsigned short divisor = 115200 / baud;
    enable_dlab(base);
    outb(base + dlsb, (unsigned char)divisor);
    outb(base + dmsb, (unsigned char)(divisor >> 8));
    disable_dlab(base); 
}

static void program_stop(port_t base,
        bool more_stop_bits)
{
    unsigned char v = inb(base + lcr);
    if (more_stop_bits)
        v |= 0x4;
    else
        v &= ~0x4;
    outb(base + lcr, v);
}

static void program_data(port_t base,
        unsigned char data_bits)
{
    unsigned char v = inb(base + lcr);
    data_bits -= 5;
    v &= ~0x3;
    v |= data_bits;
    outb(base + lcr, v);
}

static void program_parity(port_t base,
        enum serialParity parity)
{
    unsigned char v = inb(base + lcr);
    unsigned char p = ((unsigned char)parity) << 3;
    v &= ~0x38;
    v |= p;
    outb(base + lcr, v);
}

static void program_fifo(port_t base) {
    unsigned char v = inb(base + fctl);
    v |= 0x1; /// enable FIFO.
    v |= 0x6; /// clear recieve+transmit FIFO.
    v |= 0xc0; /// interrupt level 14B.
    outb(base + fctl, v);
}

bool serial_init(port_t base, 
        int baud, 
        enum serialParity parity,
        bool more_stop_bits,
        unsigned char data_bits)
{
    if (data_bits > 8 || data_bits < 5)
        return false;
    if (baud == 0)
        return false;

    unsigned char v = inb(base + ier);
    v &= ~0xf;
    outb(base + ier, v);

    program_baud(base, baud);
    program_data(base, data_bits);
    program_stop(base, more_stop_bits);
    program_parity(base, parity);
    program_fifo(base);
    outb(base + mcr, 0xb); // DTR+RTS+OUT2.
    
    return true;
}

static bool can_tx(port_t base) {
    return inb(base + lsr) & 0x20;
}

void serial_put(port_t base, char data) {
    serial_putu(base, data);
}

void serial_putu(port_t base, unsigned char data)
{
    while (!can_tx(base))
        ;

    outb(base + rtb, data);
}

void serial_write(port_t base, const char* data)
{
    while (*data++)
        serial_put(base, *data);
}

