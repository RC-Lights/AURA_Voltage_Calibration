#include "driver_system.h"
#include "driver_boost.h"
#include "driver_voltageTemperature.h"
#include "config.h"
#include <avr/interrupt.h>
#include <stdint.h>

typedef struct {
    uint8_t bandIndex;
    uint8_t blinkCount;
    uint8_t blinkIndex;
    uint8_t flashOn;
    uint8_t ticksDelay;
} calibration_state_t;

static const uint16_t VOLTAGE_VALUES[VOLTAGE_PATTERN_COUNT] = VOLTAGE_PATTERN_VALUES;
static const uint8_t VOLTAGE_BLINKS[VOLTAGE_PATTERN_COUNT] = VOLTAGE_PATTERN_BLINKS;

static calibration_state_t calibration = {0};

static uint8_t calibrationBandIndexFromVoltage(uint16_t voltage);
static void calibrationSetBaseOutput(void);
static void calibrationSetFlashOutput(void);
static void calibrationStartBurst(void);
static void calibrationSpinPattern(void);

int main(void) {

    cli();
    driverSystem_init();
    driverBoost_init();
    driverVoltageTemperature_init();
    sei();

    calibration.bandIndex = calibrationBandIndexFromVoltage(voltageTemperature.voltage);
    calibration.blinkCount = VOLTAGE_BLINKS[calibration.bandIndex];
    calibrationStartBurst();

    while (1) {
        if (system.spin) {
            driverVoltageTemperature_spin();

            if (voltageTemperature.ready) {
                uint8_t newBandIndex;

                voltageTemperature.ready = 0;
                newBandIndex = calibrationBandIndexFromVoltage(voltageTemperature.voltage);
                if (newBandIndex != calibration.bandIndex) {
                    calibration.bandIndex = newBandIndex;
                    calibration.blinkCount = VOLTAGE_BLINKS[newBandIndex];
                    calibrationStartBurst();
                }
            }

            calibrationSpinPattern();
            system.spin = 0;
        }
    }
}

static uint8_t calibrationBandIndexFromVoltage(uint16_t voltage) {

    for (uint8_t index = 0; index < VOLTAGE_PATTERN_COUNT; index++) {
        uint16_t lowerBound = (VOLTAGE_VALUES[index] > VOLTAGE_RANGE_WINDOW)
                            ? (VOLTAGE_VALUES[index] - VOLTAGE_RANGE_WINDOW)
                            : 0;
        if (voltage >= lowerBound) {
            return index;
        }
    }

    return (VOLTAGE_PATTERN_COUNT - 1);
}

static void calibrationSetBaseOutput(void) {

    driverBoost_setGear0Raw(VOLTAGE_BASE_DAC);
}

static void calibrationSetFlashOutput(void) {

    driverBoost_setGear0Raw(VOLTAGE_FLASH_DAC);
}

static void calibrationStartBurst(void) {

    calibration.blinkIndex = 0;
    calibration.flashOn = 1;
    calibration.ticksDelay = VOLTAGE_FLASH_ON_TICKS;
    calibrationSetFlashOutput();
}

static void calibrationSpinPattern(void) {

    if (calibration.ticksDelay) {
        calibration.ticksDelay--;
        return;
    }

    if (calibration.flashOn) {
        calibration.flashOn = 0;
        calibration.blinkIndex++;
        calibrationSetBaseOutput();

        if (calibration.blinkIndex < calibration.blinkCount) {
            calibration.ticksDelay = VOLTAGE_FLASH_OFF_TICKS;
        }
        else {
            calibration.ticksDelay = VOLTAGE_BURST_PAUSE_TICKS;
        }
        return;
    }

    calibration.flashOn = 1;
    calibration.ticksDelay = VOLTAGE_FLASH_ON_TICKS;
    calibrationSetFlashOutput();
}
