//#include "ecu_keypad.h"
//
//
//static const uint8 btn_value[KEYPAD_ROWS][KEYPAD_COLUMNS] = {
//
//                    { '7' , '8' , '9' , '/' },
//                    { '4' , '5' , '6' , '*' },
//                    { '1' , '2' , '3' , '-' },
//                    { '#' , '0' , '=' , '+' },
//
//};
//
//
//
//Std_ReturnType keypad_initialize(const keypad_t *keypad){
//    Std_ReturnType ret = E_OK;
//    uint8 Rows_counter      = ZERO_INIT , Columns_counter   = ZERO_INIT ;
//
//    if(NULL == keypad){
//        ret = E_NOT_OK;
//    }
//    else{
//        for (Rows_counter=ZERO_INIT ; Rows_counter < KEYPAD_ROWS ;Rows_counter++){
//            ret = gpio_pin_intialize(&(keypad->keypad_row_pins[Rows_counter]));
//        }
//
//        for (Columns_counter=ZERO_INIT ; Columns_counter < KEYPAD_COLUMNS ;Columns_counter++){
//            ret = gpio_pin_direction_intialize(&(keypad->kepyad_col_pins[Columns_counter]));
//        }
//
//    }
//
//
//    return ret;
//}
//
//
//Std_ReturnType keypad_get_value(const keypad_t *keypad , uint8 *value){
//    Std_ReturnType ret = E_OK;
//    uint8 Rows_counter      = ZERO_INIT , Columns_counter   = ZERO_INIT ,counter = ZERO_INIT ;
//    uint8 columns_logic = ZERO_INIT;
//
//
//    if(NULL == keypad || NULL == value){
//        ret = E_NOT_OK;
//    }
//    else{
//        for (Rows_counter=ZERO_INIT ; Rows_counter < KEYPAD_ROWS ;Rows_counter++){
//            for (counter=0 ; counter < KEYPAD_ROWS ; counter++ ){
//                ret = gpio_pin_write_logic(&(keypad->keypad_row_pins[counter]) , GPIO_LOW);
//            }
//            ret = gpio_pin_write_logic(&(keypad->keypad_row_pins[Rows_counter]) , GPIO_HIGH);
//
//            for (Columns_counter = 0 ; Columns_counter < KEYPAD_COLUMNS ; Columns_counter++){
//                ret = gpio_pin_read_logic(&(keypad->kepyad_col_pins[Columns_counter]),&columns_logic);
//                if (GPIO_HIGH == columns_logic){
//                    *value = btn_value[Rows_counter][Columns_counter];
//                }
//            }
//        }
//    }
//
//
//    return ret;
//}
