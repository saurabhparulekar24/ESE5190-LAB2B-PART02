#ifndef SIO_REGISTERS_H
#define SIO_REGISTERS_H

typedef          uint32_t   VALUE;
typedef volatile uint32_t* ADDRESS;

VALUE register_read(ADDRESS  address);
void register_write(ADDRESS address, VALUE value);

#endif