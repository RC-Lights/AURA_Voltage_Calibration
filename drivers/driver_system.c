#include "driver_system.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

static void clkctrlInit(void);
static void portInit(void);
static void rtcInit(void);

driverSystem_system_t system;

void driverSystem_init(void) {

    clkctrlInit();
    portInit();
    rtcInit();
    system.spin = 0;
    system.justBooted = SYSTEM_CLICK_TAP_DELAY_TICKS;
}

void driverSystem_softReset(void) {

    _PROTECTED_WRITE(RSTCTRL.SWRR, RSTCTRL_SWRE_bm);
}

void driverSystem_hardReset(void) {

    _PROTECTED_WRITE(RSTCTRL.SWRR, RSTCTRL_SWRE_bm);
}

ISR(RTC_PIT_vect) {

    if (system.justBooted) {
        system.justBooted--;
    }
    system.spin = 1;
    RTC.PITINTFLAGS = RTC_PI_bm;
}

static void clkctrlInit(void) {

    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, CLKCTRL_CLKSEL_OSCULP32K_gc);
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, !CLKCTRL_PEN_bm);
    _PROTECTED_WRITE(CLKCTRL.OSC20MCTRLA, CLKCTRL_RUNSTDBY_bm);
    _PROTECTED_WRITE(CLKCTRL.OSC32KCTRLA, CLKCTRL_RUNSTDBY_bm);
}

static void portInit(void) {

    PORTA.DIRSET = 0xFF;
    PORTA.OUTCLR = 0xFF;
    PORTB.DIRSET = 0xFF;
    PORTB.OUTCLR = 0xFF;
    PORTC.DIRSET = 0xFF;
    PORTC.OUTCLR = 0xFF;
}

static void rtcInit(void) {

    RTC.PITCTRLA = RTC_PITEN_bm | RTC_PERIOD_CYC1024_gc;
    RTC.PITINTCTRL = RTC_PI_bm;
}
