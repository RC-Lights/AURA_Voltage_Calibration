#ifndef CONFIG_VOLTAGE_CALIBRATION_H
#define CONFIG_VOLTAGE_CALIBRATION_H

#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////
//// VOLTAGE CALIBRATION ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/* Messformel:
 * Spannung [10 mV] = VOLTAGE_SCALE_NUMERATOR / ADC_Wert + VOLTAGE_OFFSET
 * Standardwert fuer 1,1-V-Referenz und 10-Bit-ADC: 112530
 */
#define VOLTAGE_SCALE_NUMERATOR                         ((uint32_t) 112530)
#define VOLTAGE_OFFSET                                  ((int16_t)  27)

/* Messintervall.
 * Einheit: 32-ms-Ticks
 */
#define VOLTAGE_MEASURE_IDLE_TICKS                      ((uint8_t)  1)
/* Startverzoegerung vor der ersten Auswertung.
 * Einheit: 32-ms-Ticks
 */
#define SYSTEM_CLICK_TAP_DELAY_TICKS                    ((uint8_t) 16)


/* Spannungsfenster pro Stufe.
 * 4 = 0,04 V = 40 mV
 * Beispiel: 4,16 V bis 4,20 V gehoeren zur 4,2-V-Stufe.
 * Einheit: 10 mV
 */
#define VOLTAGE_RANGE_WINDOW                            ((uint16_t) 4)

/* Grundlicht und Blinkspitze fuer die Kalibrierung.
 * Beide Werte laufen fest in Gear0.
 */
#define VOLTAGE_BASE_DAC                                ((uint8_t)  50)
#define VOLTAGE_FLASH_DAC                               ((uint8_t) 200)

/* Blink-Timing.
 * Einheit: 32-ms-Ticks
 */
#define VOLTAGE_FLASH_ON_TICKS                          ((uint8_t)  2)
#define VOLTAGE_FLASH_OFF_TICKS                         ((uint8_t)  1)
#define VOLTAGE_BURST_PAUSE_TICKS                       ((uint8_t) 12)

/* Kalibrier-Stufen von oben nach unten.
 * Einheit: 10 mV
 */
#define VOLTAGE_PATTERN_COUNT                           ((uint8_t) 15)
#define VOLTAGE_PATTERN_VALUES { 420, 410, 400, 390, 380, 370, 360, 350, 340, 330, 320, 310, 300, 290, 280 }
#define VOLTAGE_PATTERN_BLINKS { 4, 1, 2, 1, 1, 2, 1, 1, 1, 2, 1, 1, 2, 1, 4 }

#endif

