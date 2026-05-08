#include "driver_secondary.h"
#include "config.h"
#include <avr/io.h>
#include <stdint.h>

driver_secondary_t secondary = {0};

void driverSecondary_init(void) {

    SECONDARY_PORT.DIRSET = (1 << SECONDARY_PIN);
    SECONDARY_HDR_ENABLE_PORT.DIRSET = (1 << SECONDARY_HDR_ENABLE_PIN);
    driverSecondary_turnOff();
}

void driverSecondary_setRaw(uint8_t raw, uint8_t gear) {

    PORTMUX.CTRLC = PORTMUX_TCA03_ALTERNATE_gc;
    TCA0.SINGLE.CTRLA = 0;
    TCA0.SINGLE.CTRLESET = TCA_SINGLE_CMD_RESET_gc;
    TCA0.SPLIT.CTRLD = TCA_SPLIT_SPLITM_bm;
    TCA0.SPLIT.HPER  = 255;
    TCA0.SPLIT.HCMP0 = raw;
    TCA0.SPLIT.CTRLA = TCA_SPLIT_CLKSEL_DIV1_gc;
    TCA0.SPLIT.HCNT = 0;
    TCA0.SPLIT.CTRLB |= TCA_SPLIT_HCMP0EN_bm;
    TCA0.SPLIT.CTRLA |= TCA_SPLIT_ENABLE_bm;

    if (gear) {
        SECONDARY_HDR_ENABLE_PORT.OUTSET = (1 << SECONDARY_HDR_ENABLE_PIN);
    }
    else {
        SECONDARY_HDR_ENABLE_PORT.OUTCLR = (1 << SECONDARY_HDR_ENABLE_PIN);
    }

    secondary.on = (raw > 0);
    secondary.raw = raw;
    secondary.gear = gear;
}

void driverSecondary_turnOff(void) {

    SECONDARY_HDR_ENABLE_PORT.OUTCLR = (1 << SECONDARY_HDR_ENABLE_PIN);
    TCA0.SPLIT.CTRLA &= ~TCA_SPLIT_ENABLE_bm;
    TCA0.SPLIT.CTRLB &= ~TCA_SPLIT_HCMP0EN_bm;
    PORTMUX.CTRLC = BOOST_ENABLE_PORTMUX;

    secondary.on = 0;
    secondary.raw = 0;
    secondary.gear = 0;
}
