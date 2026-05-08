#ifndef CONFIG_HARDWARE_H
#define CONFIG_HARDWARE_H

////////////////////////////////////////////////////////////////////////////////
//// HARDWARE //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/* Verbaute Schalter-Hardware:
 * BUTTON_CLICKY  = mechanischer Clicky
 * BUTTON_ESWITCH = elektronischer Seitenschalter
 * Es darf nur eine Variante aktiv sein.
 */
#define BUTTON_CLICKY

/* Taster-Pin und Interrupt-Vektor. */
#ifdef BUTTON_ESWITCH
#define BUTTON_PORT                                     PORTC
#define BUTTON_PIN                                      PIN2
#define BUTTON_INTERRUPT_VECTOR                         PORTC_PORT_vect
#elif defined(BUTTON_CLICKY)
#define BUTTON_PORT                                     PORTA
#define BUTTON_PIN                                      PIN1
#define BUTTON_INTERRUPT_VECTOR                         PORTA_PORT_vect
#endif
/* Freigabe des Tasterpfads. */
#define BUTTON_ENABLE_PORT                              PORTB
#define BUTTON_ENABLE_PIN                               PIN0

/* Boost-Kanal:
 * ENABLE = Treiber ein/aus
 * LEVEL  = DAC-Sollwert
 * HDR    = hoher Gang
 * VREF   = DAC-Referenz
 */
#define BOOST_ENABLE_PORT                               PORTA
#define BOOST_ENABLE_PIN                                PIN3
#define BOOST_ENABLE_PORTMUX                            PORTMUX_TCA03_DEFAULT_gc
#define BOOST_LEVEL_PORT                                PORTA
#define BOOST_LEVEL_PIN                                 PIN6
#define BOOST_LEVEL_DAC                                 DAC0
#define BOOST_LEVEL_VREF                                VREF_DAC0REFSEL_1V1_gc
#define BOOST_HDR_ENABLE_PORT                           PORTA
#define BOOST_HDR_ENABLE_PIN                            PIN7

/* Secondary-Kanal:
 * SECONDARY_* = Ausgang
 * HDR_ENABLE  = hoher Gang
 */
#define SECONDARY_PORT                                  PORTC
#define SECONDARY_PIN                                   PIN3
#define SECONDARY_HDR_ENABLE_PORT                       PORTB
#define SECONDARY_HDR_ENABLE_PIN                        PIN5

/* Antiflash-Klemme des Boost-Reglers. */
#define ANTIFLASH_ENABLE_PORT                           PORTA
#define ANTIFLASH_ENABLE_PIN                            PIN5

/* Verfuegbarer EEPROM-Bereich in Byte. */
#define MEMORY_EEPROM_SIZE                              ((uint16_t) 64  )

#endif
