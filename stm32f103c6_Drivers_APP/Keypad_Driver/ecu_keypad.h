///*
// * File:   ecu_keypad.h
// * Author: anaal
// *
// * Created on March 8, 2025, 10:15 PM
// */
//
//#ifndef ECU_KEYPAD_H
//#define	ECU_KEYPAD_H
//
///*Section : Includes */
//
//#include "../../MCAL_layer/GPIO/hal_gpio.h"
//#include "ecu_keypad_cfg.h"
//
///* Section : Macro Declarations */
//
//#define KEYPAD_ROWS     4
//#define KEYPAD_COLUMNS  4
//
///* Section : Macro Functions Declarations */
//
///* Section : Data Type Declarations */
//
//typedef struct {
//    pin_config_t keypad_row_pins[KEYPAD_ROWS];
//    pin_config_t kepyad_col_pins[KEYPAD_COLUMNS];
//}keypad_t;
//
///* Section : Function Declarations */
//
//Std_ReturnType keypad_initialize(const keypad_t *keypad);
//Std_ReturnType keypad_get_value(const keypad_t *keypad , uint8 *value);
//
//#endif	/* ECU_KEYPAD_H */
//
