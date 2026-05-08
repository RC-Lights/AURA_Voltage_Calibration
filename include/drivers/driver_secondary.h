#ifndef DRIVER_SECONDARY_H
#define DRIVER_SECONDARY_H

#include <stdint.h>

typedef struct {
    uint8_t on;
    uint8_t raw;
    uint8_t gear;
} driver_secondary_t;

extern driver_secondary_t secondary;

void driverSecondary_init(void);
void driverSecondary_setRaw(uint8_t raw, uint8_t gear);
void driverSecondary_turnOff(void);

#endif
