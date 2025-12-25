#include "Keypad.h"

// Updated to match your physical keypad layout after pin swap
static uint8_t KPD_ARR[KEYPAD_ROWS][KEYPAD_COL] =
{
    { '7', '8', '9', '/' },    // Row A (PA0)
    { '4', '5', '6', '*' },    // Row B (PA1)
    { '1', '2', '3', '-' },    // Row C (PA2)
    { 'C', '0', '=', '+' }     // Row D (PA3)
};

void KEYPAD_Init(KEYPAD_PinConfig *Keypad){
    // Initialize rows as input pull-up
    for(uint8_t r = 0; r < KEYPAD_ROWS; r++){
        MCAL_GPIO_Init(Keypad->GPIOx, &Keypad->KeypadRows[r]);
    }
    // Initialize columns as output push-pull
    for(uint8_t c = 0; c < KEYPAD_COL; c++){
        MCAL_GPIO_Init(Keypad->GPIOx, &Keypad->KeypadCols[c]);
    }
    // Set all columns HIGH after GPIO init
    for(uint8_t c = 0; c < KEYPAD_COL; c++){
        MCAL_GPIO_WritePin(Keypad->GPIOx, Keypad->KeypadCols[c].GPIO_PinNumber, GPIO_PIN_STATE_HIGH);
    }
}

uint8_t KEYPAD_GetChar(KEYPAD_PinConfig *Keypad)
{
    for(uint8_t col = 0; col < KEYPAD_COL; col++)
    {
        MCAL_GPIO_WritePin(Keypad->GPIOx,
                           Keypad->KeypadCols[col].GPIO_PinNumber,
                           GPIO_PIN_STATE_LOW);

        for(volatile uint16_t d = 0; d < 100; d++);

        for(uint8_t row = 0; row < KEYPAD_ROWS; row++)
        {
            if(!MCAL_GPIO_ReadPin(Keypad->GPIOx,
                                  Keypad->KeypadRows[row].GPIO_PinNumber))
            {
                uint8_t key = KPD_ARR[row][col];

                volatile uint32_t timeout = 0;
                while(!MCAL_GPIO_ReadPin(Keypad->GPIOx, Keypad->KeypadRows[row].GPIO_PinNumber))
                {
                    timeout++;
                    if(timeout > 50000) break; // Break if button is stuck for too long
                }
                MCAL_GPIO_WritePin(Keypad->GPIOx,
                                   Keypad->KeypadCols[col].GPIO_PinNumber,
                                   GPIO_PIN_STATE_HIGH);
                for(int i=0 ; i<500 ; i++);

                return key;
            }
        }

        MCAL_GPIO_WritePin(Keypad->GPIOx,
                           Keypad->KeypadCols[col].GPIO_PinNumber,
                           GPIO_PIN_STATE_HIGH);
    }
    return RELEASED_BUTTON;
}

