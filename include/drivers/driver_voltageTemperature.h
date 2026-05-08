#ifndef DRIVER_VOLTAGE_TEMPERATURE_H
#define DRIVER_VOLTAGE_TEMPERATURE_H

#include <stdint.h>

#define DRIVER_VOLTAGE_TEMPERATURE_STATE_IDLE           ((uint8_t) 0)
#define DRIVER_VOLTAGE_TEMPERATURE_STATE_BUSY           ((uint8_t) 1)

typedef struct {
    uint8_t state;
    uint16_t voltage;
    uint8_t ready;
} driver_voltageTemperature_t;

extern driver_voltageTemperature_t voltageTemperature;

void driverVoltageTemperature_init(void);
void driverVoltageTemperature_spin(void);
void driverVoltageTemperature_forceMeasure(void);

#endif
