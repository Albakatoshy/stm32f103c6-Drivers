//#include "ecu_chr_lcd.h"
//
//static Std_ReturnType lcd_send_4bits(const chr_lcd_4bit_t *lcd , uint8 bits );
//
//static Std_ReturnType lcd_4bit_send_enable_signal(const chr_lcd_4bit_t *lcd);
//static Std_ReturnType lcd_8bit_send_enable_signal(const chr_lcd_8bit_t *lcd);
//
//static Std_ReturnType lcd_4bit_set_cursor(const chr_lcd_4bit_t *lcd , uint8 row , uint8 column );
//static Std_ReturnType lcd_8bit_set_cursor(const chr_lcd_8bit_t *lcd , uint8 row , uint8 column );
//
//
//
//
//
//
//
//
//Std_ReturnType lcd_4bit_initialize(const chr_lcd_4bit_t *lcd){
//    Std_ReturnType ret = E_OK ;
//    uint8 l_data_pins_counter = ZERO_INIT ;
//
//    if (NULL == lcd ){
//         ret = E_NOT_OK ;
//    }
//    else{
//        ret = gpio_pin_intialize(&(lcd->lcd_rs_pin));
//        ret = gpio_pin_intialize(&(lcd->lcd_en_pin));
//        for (l_data_pins_counter = ZERO_INIT ; l_data_pins_counter < 4 ; l_data_pins_counter++ ){
//            ret = gpio_pin_intialize(&(lcd->lcd_data[l_data_pins_counter]));
//        }
//        __delay_ms(20);
//        ret = lcd_4bit_send_command(lcd , _LCD_8BIT_MODE_2_LINE);
//        __delay_ms(5);
//        ret = lcd_4bit_send_command(lcd , _LCD_8BIT_MODE_2_LINE);
//        __delay_us(150);
//        ret = lcd_4bit_send_command(lcd , _LCD_8BIT_MODE_2_LINE);
//        ret = lcd_4bit_send_command(lcd , _LCD_CLEAR);
//        ret = lcd_4bit_send_command(lcd , _LCD_RETURN_HOME);
//        ret = lcd_4bit_send_command(lcd , _LCD_ENRTY_MODE);
//        ret = lcd_4bit_send_command(lcd , _LCD_CURSOR_OFF_DISPLAY_ON);
//        ret = lcd_4bit_send_command(lcd , _LCD_4BIT_MODE_2_LINE);
//        ret = lcd_4bit_send_command(lcd , 0x80);
//    }
//
//    return ret ;
//}
//Std_ReturnType lcd_4bit_send_command(const chr_lcd_4bit_t *lcd , uint8 command){
//    Std_ReturnType ret = E_OK ;
//    if (NULL == lcd ){
//         ret = E_NOT_OK ;
//    }
//    else{
//        ret = gpio_pin_write_logic(&(lcd->lcd_rs_pin),GPIO_LOW);
//        ret =lcd_send_4bits(lcd , command >> 4);
//        ret = lcd_4bit_send_enable_signal(lcd);
//        ret =lcd_send_4bits(lcd , command );
//        ret = lcd_4bit_send_enable_signal(lcd);
//    }
//
//    return ret ;
//}
//Std_ReturnType lcd_4bit_send_char_data(const chr_lcd_4bit_t *lcd , uint8 data){
//    Std_ReturnType ret = E_OK ;
//    if (NULL == lcd ){
//         ret = E_NOT_OK ;
//    }
//    else{
//        ret = gpio_pin_write_logic(&(lcd->lcd_rs_pin),GPIO_HIGH);
//        ret =lcd_send_4bits(lcd , data >> 4);
//        ret = lcd_4bit_send_enable_signal(lcd);
//        ret =lcd_send_4bits(lcd , data );
//        ret = lcd_4bit_send_enable_signal(lcd);
//    }
//
//    return ret ;
//}
//Std_ReturnType lcd_4bit_send_char_data_pos(const chr_lcd_4bit_t *lcd , uint8 row , uint8 column , uint8 data){
//    Std_ReturnType ret = E_OK ;
//    if (NULL == lcd ){
//        Std_ReturnType ret = E_NOT_OK ;
//    }
//    else{
//        ret = lcd_4bit_set_cursor(lcd , row ,column );
//        ret = lcd_4bit_send_char_data(lcd , data);
//    }
//
//    return ret ;
//}
//Std_ReturnType lcd_4bit_send_string(const chr_lcd_4bit_t *lcd , uint8 *str){
//    Std_ReturnType ret = E_OK ;
//    if (NULL == lcd || str == NULL ){
//        Std_ReturnType ret = E_NOT_OK ;
//    }
//    else{
//        while(*str){
//            ret = lcd_4bit_send_char_data(lcd , *str++);
//        }
//    }
//
//    return ret ;
//}
//Std_ReturnType lcd_4bit_send_string_pos(const chr_lcd_4bit_t *lcd , uint8 row , uint8 column , uint8 *str){
//    Std_ReturnType ret = E_OK ;
//    if (NULL == lcd || str == NULL ){
//        Std_ReturnType ret = E_NOT_OK ;
//    }
//    else{
//        ret = lcd_4bit_set_cursor(lcd , row ,column );
//        while(*str){
//            ret = lcd_4bit_send_char_data(lcd , *str++);
//        }
//    }
//
//    return ret ;
//}
//Std_ReturnType lcd_4bit_send_custom_char(const chr_lcd_4bit_t *lcd , uint8 row , uint8 column , const uint8 _chr[] , uint8 memory_pos){
//    Std_ReturnType ret = E_OK ;
//    uint8 l_lcd_counter = ZERO_INIT ;
//    if (NULL == lcd ){
//        Std_ReturnType ret = E_NOT_OK ;
//    }
//    else{
//        ret = lcd_4bit_send_command(lcd , (_LCD_CGRAM_START+(memory_pos*8)));
//        for (l_lcd_counter=0 ; l_lcd_counter<=7 ; l_lcd_counter++){
//            ret = lcd_4bit_send_char_data(lcd , _chr[l_lcd_counter]);
//        }
//        ret = lcd_4bit_send_char_data_pos(lcd , row , column , memory_pos);
//    }
//
//    return ret ;
//}
//
//
//Std_ReturnType lcd_8bit_initialize(const chr_lcd_8bit_t *lcd){
//    Std_ReturnType ret = E_OK ;
//    uint8 l_data_pins_counter = ZERO_INIT ;
//    if (NULL == lcd ){
//        Std_ReturnType ret = E_NOT_OK ;
//    }
//    else{
//        ret = gpio_pin_intialize(&(lcd->lcd_rs_pin));
//        ret = gpio_pin_intialize(&(lcd->lcd_en_pin));
//        for (l_data_pins_counter = ZERO_INIT ; l_data_pins_counter < 8 ; l_data_pins_counter++ ){
//            ret = gpio_pin_intialize(&(lcd->lcd_data[l_data_pins_counter]));
//        }
//        __delay_ms(20);
//        ret = lcd_8bit_send_command(lcd , _LCD_8BIT_MODE_2_LINE);
//        __delay_ms(5);
//        ret = lcd_8bit_send_command(lcd , _LCD_8BIT_MODE_2_LINE);
//        __delay_us(150);
//        ret = lcd_8bit_send_command(lcd , _LCD_8BIT_MODE_2_LINE);
//        ret = lcd_8bit_send_command(lcd , _LCD_CLEAR);
//        ret = lcd_8bit_send_command(lcd , _LCD_RETURN_HOME);
//        ret = lcd_8bit_send_command(lcd , _LCD_ENRTY_MODE);
//        ret = lcd_8bit_send_command(lcd , _LCD_CURSOR_OFF_DISPLAY_ON);
//        ret = lcd_8bit_send_command(lcd , _LCD_8BIT_MODE_2_LINE);
//        ret = lcd_8bit_send_command(lcd , 0x80);
//    }
//
//    return ret ;
//}
//Std_ReturnType lcd_8bit_send_command(const chr_lcd_8bit_t *lcd , uint8 command){
//    Std_ReturnType ret = E_OK ;
//    uint8 l_counter_send_commands = ZERO_INIT ;
//    if (NULL == lcd ){
//        Std_ReturnType ret = E_NOT_OK ;
//    }
//    else{
//        ret = gpio_pin_write_logic(&(lcd->lcd_rs_pin),GPIO_LOW);
//
//        for (l_counter_send_commands = ZERO_INIT ; l_counter_send_commands < 8 ; l_counter_send_commands++ ){
//            ret = gpio_pin_write_logic(&(lcd->lcd_data[l_counter_send_commands]), (command >> l_counter_send_commands ) & (uint8)0x01 );
//        }
//        ret = lcd_8bit_send_enable_signal(lcd);
//    }
//
//    return ret ;
//}
//Std_ReturnType lcd_8bit_send_char_data(const chr_lcd_8bit_t *lcd , uint8 data){
//    Std_ReturnType ret = E_OK ;
//    uint8 l_counter_send_data = ZERO_INIT ;
//    if (NULL == lcd ){
//        Std_ReturnType ret = E_NOT_OK ;
//    }
//    else{
//        ret = gpio_pin_write_logic(&(lcd->lcd_rs_pin),GPIO_HIGH);
//        for (l_counter_send_data = ZERO_INIT ; l_counter_send_data < 8 ; l_counter_send_data++ ){
//            ret = gpio_pin_write_logic(&(lcd->lcd_data[l_counter_send_data]), (data >> l_counter_send_data ) & (uint8)0x01 );
//        }
//        ret = lcd_8bit_send_enable_signal(lcd);
//    }
//
//    return ret ;
//}
//Std_ReturnType lcd_8bit_send_char_data_pos(const chr_lcd_8bit_t *lcd , uint8 row , uint8 column , uint8 data){
//    Std_ReturnType ret = E_OK ;
//    if (NULL == lcd ){
//        Std_ReturnType ret = E_NOT_OK ;
//    }
//    else{
//        ret = lcd_8bit_set_cursor(lcd , row , column);
//        ret = lcd_8bit_send_char_data(lcd , data);
//    }
//
//    return ret ;
//}
//Std_ReturnType lcd_8bit_send_string(const chr_lcd_8bit_t *lcd , uint8 *str){
//    Std_ReturnType ret = E_OK ;
//    if (NULL == lcd ){
//        Std_ReturnType ret = E_NOT_OK ;
//    }
//    else{
//        while(*str){
//            ret = lcd_8bit_send_char_data(lcd , *str++);
//        }
//    }
//
//    return ret ;
//}
//Std_ReturnType lcd_8bit_send_string_pos(const chr_lcd_8bit_t *lcd , uint8 row , uint8 column , uint8 *str){
//    Std_ReturnType ret = E_OK ;
//    if (NULL == lcd || str == NULL){
//        Std_ReturnType ret = E_NOT_OK ;
//    }
//    else{
//        ret = lcd_8bit_set_cursor(lcd , row , column);
//        while(*str){
//            ret = lcd_8bit_send_char_data(lcd , *str++);
//        }
//    }
//
//    return ret ;
//}
//Std_ReturnType lcd_8bit_send_custom_char(const chr_lcd_8bit_t *lcd , uint8 row , uint8 column , const uint8 _chr[] , uint8 memory_pos){
//    Std_ReturnType ret = E_OK ;
//    uint8 l_lcd_counter = ZERO_INIT ;
//    if (NULL == lcd ){
//        Std_ReturnType ret = E_NOT_OK ;
//    }
//    else{
//        ret = lcd_8bit_send_command(lcd , (_LCD_CGRAM_START+(memory_pos*8)));
//        for (l_lcd_counter=0 ; l_lcd_counter<=7 ; l_lcd_counter++){
//            ret = lcd_8bit_send_char_data(lcd , _chr[l_lcd_counter]);
//        }
//        ret = lcd_8bit_send_char_data_pos(lcd , row , column , memory_pos);
//    }
//
//    return ret ;
//}
//
//
//
//Std_ReturnType convert_byte_to_string(uint8 val , uint8 *str){
//    Std_ReturnType ret = E_OK ;
//    if (NULL == str){
//        Std_ReturnType ret = E_NOT_OK ;
//    }
//    else{
//        memset(str , '\0' , 4);
//        sprintf(str , "%i" ,val);
//    }
//
//    return ret ;
//}
//Std_ReturnType convert_short_to_string(uint16 val , uint8 *str){
//    Std_ReturnType ret = E_OK ;
//    if (NULL == str){
//        Std_ReturnType ret = E_NOT_OK ;
//    }
//    else{
//        memset(str , '\0' , 6);
//        sprintf(str , "%i" ,val);
//    }
//
//    return ret ;
//}
//Std_ReturnType convert_int_to_string(uint32 val , uint8 *str){
//    Std_ReturnType ret = E_OK ;
//    if (NULL == str){
//        Std_ReturnType ret = E_NOT_OK ;
//    }
//    else{
//        memset(str , '\0' , 11);
//        sprintf(str , "%i" ,val);
//    }
//
//    return ret ;
//}
//
//
//
//static Std_ReturnType lcd_send_4bits(const chr_lcd_4bit_t *lcd , uint8 bits ){
//    Std_ReturnType ret = E_OK ;
//    uint8 l_counter = ZERO_INIT ;
//
//    if (NULL == lcd ){
//        Std_ReturnType ret = E_NOT_OK ;
//    }
//    else{
//        for (l_counter = ZERO_INIT ; l_counter<4 ; l_counter++){
//            ret = gpio_pin_write_logic(&(lcd->lcd_data[l_counter]), (bits>>l_counter) & ((uint8)0x01) );
//        }
//    }
//
//    return ret ;
//}
//
//static Std_ReturnType lcd_4bit_send_enable_signal(const chr_lcd_4bit_t *lcd){
//    Std_ReturnType ret = E_OK ;
//    if (NULL == lcd ){
//        Std_ReturnType ret = E_NOT_OK ;
//    }
//    else{
//        ret = gpio_pin_write_logic(&(lcd->lcd_en_pin), GPIO_HIGH);
//        __delay_us(5);
//        ret = gpio_pin_write_logic(&(lcd->lcd_en_pin), GPIO_LOW);
//    }
//
//    return ret ;
//}
//
//static Std_ReturnType lcd_8bit_send_enable_signal(const chr_lcd_8bit_t *lcd){
//    Std_ReturnType ret = E_OK ;
//    if (NULL == lcd ){
//        Std_ReturnType ret = E_NOT_OK ;
//    }
//    else{
//        ret = gpio_pin_write_logic(&(lcd->lcd_en_pin), GPIO_HIGH);
//        __delay_us(5);
//        ret = gpio_pin_write_logic(&(lcd->lcd_en_pin), GPIO_LOW);
//    }
//
//    return ret ;
//}
//
//static Std_ReturnType lcd_4bit_set_cursor(const chr_lcd_4bit_t *lcd , uint8 row , uint8 column ){
//    Std_ReturnType ret = E_OK ;
//    column--;
//    if (NULL == lcd ){
//        Std_ReturnType ret = E_NOT_OK ;
//    }
//    else{
//        switch(row){
//            case ROW1 : ret = lcd_4bit_send_command(lcd , (0x80+column)); break;
//            case ROW2 : ret = lcd_4bit_send_command(lcd , (0xC0+column)); break;
//            case ROW3 : ret = lcd_4bit_send_command(lcd , (0x94+column)); break;
//            case ROW4 : ret = lcd_4bit_send_command(lcd , (0xD4+column)); break;
//            default : ;
//
//        }
//    }
//
//    return ret ;
//}
//
//static Std_ReturnType lcd_8bit_set_cursor(const chr_lcd_8bit_t *lcd , uint8 row , uint8 column ){
//    Std_ReturnType ret = E_OK ;
//    column--;
//    if (NULL == lcd ){
//        Std_ReturnType ret = E_NOT_OK ;
//    }
//    else{
//        switch(row){
//            case ROW1 : ret = lcd_8bit_send_command(lcd , (0x80+column)); break;
//            case ROW2 : ret = lcd_8bit_send_command(lcd , (0xC0+column)); break;
//            case ROW3 : ret = lcd_8bit_send_command(lcd , (0x94+column)); break;
//            case ROW4 : ret = lcd_8bit_send_command(lcd , (0xD4+column)); break;
//            default : ;
//
//        }
//    }
//
//    return ret ;
//}
