#include "driver_system.h"
#include "driver_boost.h"
#include "driver_voltageTemperature.h"
#include "config.h"
#include <avr/interrupt.h>
#include <stdint.h>

typedef struct {
    uint8_t blinkCount;
    uint8_t blinkIndex;
    uint8_t flashOn;
    uint8_t ticksDelay;
} calibration_state_t;

static const uint16_t VOLTAGE_VALUES[VOLTAGE_PATTERN_COUNT] = VOLTAGE_PATTERN_VALUES;
static const uint8_t VOLTAGE_BLINKS[VOLTAGE_PATTERN_COUNT] = VOLTAGE_PATTERN_BLINKS;

static calibration_state_t calibration = {0};

static uint8_t calibrationBlinkCountFromVoltage(uint16_t voltage);
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

    calibration.blinkCount = calibrationBlinkCountFromVoltage(voltageTemperature.voltage);
    calibrationStartBurst();

    while (1) {
        if (system.spin) {
            driverVoltageTemperature_spin();

            if (voltageTemperature.ready) {
                uint8_t newBlinkCount;

                voltageTemperature.ready = 0;
                newBlinkCount = calibrationBlinkCountFromVoltage(voltageTemperature.voltage);
                if (newBlinkCount != calibration.blinkCount) {
                    calibration.blinkCount = newBlinkCount;
                    calibrationStartBurst();
                }
            }

            calibrationSpinPattern();
            system.spin = 0;
        }
    }
}

static uint8_t calibrationBlinkCountFromVoltage(uint16_t voltage) {

    for (uint8_t index = 0; index < VOLTAGE_PATTERN_COUNT; index++) {
        uint16_t lowerBound = (VOLTAGE_VALUES[index] > VOLTAGE_RANGE_WINDOW)
                            ? (VOLTAGE_VALUES[index] - VOLTAGE_RANGE_WINDOW)
                            : 0;

        /* Blink only inside the calibration window below each threshold. */
        if ((voltage <= VOLTAGE_VALUES[index]) && (voltage >= lowerBound)) {
            return VOLTAGE_BLINKS[index];
        }
    }

    return 0;
}

static void calibrationSetBaseOutput(void) {

    driverBoost_setGear0Raw(VOLTAGE_BASE_DAC);
}

static void calibrationSetFlashOutput(void) {

    driverBoost_setGear0Raw(VOLTAGE_FLASH_DAC);
}

static void calibrationStartBurst(void) {

    calibration.blinkIndex = 0;
    if (calibration.blinkCount == 0) {
        calibration.flashOn = 0;
        calibration.ticksDelay = 0;
        calibrationSetBaseOutput();
        return;
    }

    calibration.flashOn = 1;
    calibration.ticksDelay = VOLTAGE_FLASH_ON_TICKS;
    calibrationSetFlashOutput();
}

static void calibrationSpinPattern(void) {

    if (calibration.blinkCount == 0) {
        calibrationSetBaseOutput();
        return;
    }

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

    if (calibration.blinkIndex >= calibration.blinkCount) {
        calibration.blinkIndex = 0;
    }

    calibration.flashOn = 1;
    calibration.ticksDelay = VOLTAGE_FLASH_ON_TICKS;
    calibrationSetFlashOutput();
}
