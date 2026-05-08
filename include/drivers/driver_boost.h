#ifndef DRIVER_BOOST_H
#define DRIVER_BOOST_H

#include <stdint.h>

typedef struct {
    uint8_t on;
    uint8_t raw;
} driverBoost_boost_t;

extern driverBoost_boost_t boost;

void driverBoost_init(void);
void driverBoost_setGear0Raw(uint8_t raw);
void driverBoost_setGear1Raw(uint8_t raw);
void driverBoost_turnOff(void);

#endif
