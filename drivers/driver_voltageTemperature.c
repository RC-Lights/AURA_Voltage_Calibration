#include "driver_voltageTemperature.h"
#include "config.h"
#include <avr/io.h>
#include <stdint.h>

driver_voltageTemperature_t voltageTemperature = {0};

static void driverVoltageTemperature_measure(void);

void driverVoltageTemperature_init(void) {

    VREF.CTRLA |= VREF_ADC0REFSEL_1V1_gc;
    VREF.CTRLB |= VREF_ADC0REFEN_bm;
    ADC0.CTRLA = ADC_RESSEL_10BIT_gc;
    ADC0.CTRLB = ADC_SAMPNUM_ACC1_gc;
    ADC0.CTRLC = ADC_SAMPCAP_bm;
    ADC0.CTRLD = ADC_INITDLY_DLY32_gc;
    ADC0.SAMPCTRL = 2;

    voltageTemperature.state = DRIVER_VOLTAGE_TEMPERATURE_STATE_IDLE;
    voltageTemperature.voltage = 0;
    voltageTemperature.ready = 0;

    driverVoltageTemperature_forceMeasure();
}

void driverVoltageTemperature_spin(void) {

    static uint8_t idleTicks = 0;

    if (idleTicks) {
        idleTicks--;
        return;
    }

    driverVoltageTemperature_measure();
    voltageTemperature.ready = 1;
    idleTicks = (VOLTAGE_MEASURE_IDLE_TICKS > 0) ? (VOLTAGE_MEASURE_IDLE_TICKS - 1) : 0;
}

void driverVoltageTemperature_forceMeasure(void) {

    driverVoltageTemperature_measure();
    voltageTemperature.ready = 1;
}

static void driverVoltageTemperature_measure(void) {

    uint16_t adcReading;
    int32_t voltage;

    ADC0.CTRLC &= ~ADC_REFSEL_INTREF_gc;
    ADC0.CTRLC |= ADC_REFSEL_VDDREF_gc;
    ADC0.CTRLA |= ADC_ENABLE_bm;
    ADC0.MUXPOS = ADC_MUXPOS_INTREF_gc;

    ADC0.COMMAND = ADC_STCONV_bm;
    while (!(ADC0.INTFLAGS & ADC_RESRDY_bm));
    adcReading = ADC0.RES;

    ADC0.COMMAND = ADC_STCONV_bm;
    while (!(ADC0.INTFLAGS & ADC_RESRDY_bm));
    adcReading = ADC0.RES;

    ADC0.CTRLA &= ~ADC_ENABLE_bm;

    if (adcReading == 0) {
        voltageTemperature.voltage = 0;
        return;
    }

    voltage = (int32_t)(VOLTAGE_SCALE_NUMERATOR / (uint32_t)adcReading);
    voltage += VOLTAGE_OFFSET;

    if (voltage < 0) {
        voltage = 0;
    }

    voltageTemperature.voltage = (uint16_t)voltage;
}
