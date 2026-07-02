#pragma once

/// address unit in I/O space.
typedef unsigned short port_t;

/// function generator macro for port in.
#define __CSZ(n, sz) \
    static inline unsigned sz in##n \
    (port_t port) \
{ \
    unsigned sz c; \
    __asm__ ("in"#n" %%dx\n" \
            : "=a"(c) : "d"(port)); \
    return c; \
}

/// function generator macro for port out.
#define __CSX(n, sz) \
    static inline void out##n \
    (port_t port, unsigned sz value) \
{ \
    __asm__ ("out"#n" %0,%%dx\n" \
            :: "a"(value), "d"(port)); \
}

__CSZ(b, char);
__CSZ(w, short);
__CSZ(d, int);

__CSX(b, char);
__CSX(w, short);
__CSX(d, int);

#undef __CSZ
#undef __CSX

