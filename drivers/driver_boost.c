#include "driver_boost.h"
#include "config.h"
#include <avr/io.h>
#include <stdint.h>

driverBoost_boost_t boost = {0};

void driverBoost_init(void) {

    VREF.CTRLA = BOOST_LEVEL_VREF;
    VREF.CTRLB = VREF_DAC0REFEN_bm;
    BOOST_LEVEL_DAC.CTRLA = DAC_ENABLE_bm | DAC_OUTEN_bm;

    driverBoost_turnOff();
}

void driverBoost_setGear0Raw(uint8_t raw) {

    PORTMUX.CTRLC = BOOST_ENABLE_PORTMUX;
    BOOST_HDR_ENABLE_PORT.OUTCLR = (1 << BOOST_HDR_ENABLE_PIN);
    BOOST_LEVEL_DAC.DATA = raw;
    BOOST_ENABLE_PORT.OUTSET = (1 << BOOST_ENABLE_PIN);

    boost.on = (raw > 0);
    boost.raw = raw;
}

void driverBoost_setGear1Raw(uint8_t raw) {

    PORTMUX.CTRLC = BOOST_ENABLE_PORTMUX;
    BOOST_LEVEL_DAC.DATA = raw;
    BOOST_HDR_ENABLE_PORT.OUTSET = (1 << BOOST_HDR_ENABLE_PIN);
    BOOST_ENABLE_PORT.OUTSET = (1 << BOOST_ENABLE_PIN);

    boost.on = (raw > 0);
    boost.raw = raw;
}

void driverBoost_turnOff(void) {

    BOOST_LEVEL_DAC.DATA = 0;
    BOOST_HDR_ENABLE_PORT.OUTCLR = (1 << BOOST_HDR_ENABLE_PIN);
    BOOST_ENABLE_PORT.OUTCLR = (1 << BOOST_ENABLE_PIN);
    ANTIFLASH_ENABLE_PORT.OUTCLR = (1 << ANTIFLASH_ENABLE_PIN);

    boost.on = 0;
    boost.raw = 0;
}
