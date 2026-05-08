/*
 * File:        driver_system.h
 * Project:     AURA - Adaptive Unified Regulation Architecture
 * Description: Handles task scheduler implementation for managing tasks.
 *
 * Author:      Cem Saribal saribalc@outlook.com
 * Created:     2025-05-30
 * Version:     0.1.0
 */


#ifndef DRIVER_SYSTEM_H
#define DRIVER_SYSTEM_H

#include "config.h"
#include <stdint.h>

typedef struct {
    volatile uint8_t spin;
    volatile uint8_t justBooted;
} driverSystem_system_t;

extern driverSystem_system_t system;

void driverSystem_init(void);
void driverSystem_softReset(void);
void driverSystem_hardReset(void);

#endif
