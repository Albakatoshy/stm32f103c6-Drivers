/*
 * stm32f103c6_GPIO_Driver.c
 *
 *  Created on: Dec 22, 2025
 *      Author: Abdalrahman
 */

#include "stm32f103c6_GPIO_Driver.h"

//----------------------------------
//Helper Functions
//----------------------------------

/**
 * @brief  Gets the bit position in CRL/CRH register for a given pin
 * @param  PinNumber: Pin number from GPIO_PINS_DEFINE
 * @retval Bit position (0, 4, 8, 12, 16, 20, 24, or 28)
 */
static uint8_t Get_CRL_CRH_PinPosition(uint16_t PinNumber)
{
    switch(PinNumber)
    {
        case GPIO_PIN_0:  return 0;
        case GPIO_PIN_1:  return 4;
        case GPIO_PIN_2:  return 8;
        case GPIO_PIN_3:  return 12;
        case GPIO_PIN_4:  return 16;
        case GPIO_PIN_5:  return 20;
        case GPIO_PIN_6:  return 24;
        case GPIO_PIN_7:  return 28;
        case GPIO_PIN_8:  return 0;
        case GPIO_PIN_9:  return 4;
        case GPIO_PIN_10: return 8;
        case GPIO_PIN_11: return 12;
        case GPIO_PIN_12: return 16;
        case GPIO_PIN_13: return 20;
        case GPIO_PIN_14: return 24;
        case GPIO_PIN_15: return 28;
        default:          return 0;
    }
}

//----------------------------------
//APIs Implementation
//----------------------------------

/**
 * @FN            MCAL_GPIO_Init
 * @brief         Initializes the GPIO pin according to the specified parameters
 *                in the GPIO_PinConfig_t structure.
 *
 * @param[in]     GPIOx: Pointer to GPIO port (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE)
 * @param[in]     PinConfig: Pointer to GPIO_PinConfig_t structure containing
 *                the configuration information for the specified GPIO pin.
 *
 * @retval        None
 *
 * @note          - The GPIO peripheral clock must be enabled before calling this function
 *                - Configures: Pin number, Mode, Output speed, Pull-up/Pull-down
 */
void MCAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_PinConfig_t *PinConfig)
{
    // Input validation
    if (GPIOx == NULL || PinConfig == NULL)
    {
        return;
    }

    // Port configuration register low (GPIOx_CRL): Pins 0-7
    // Port configuration register high (GPIOx_CRH): Pins 8-15
    volatile uint32_t *ConfigRegister = NULL;
    uint8_t PinConfig_Value = 0;
    uint8_t PinPosition = 0;

    // Select CRL or CRH based on pin number
    uint8_t pin_index = 0;

    if (PinConfig->GPIO_PinNumber & 0x00FF)
        ConfigRegister = &GPIOx->CRL;
    else
        ConfigRegister = &GPIOx->CRH;

    // Get bit position in the register
    PinPosition = Get_CRL_CRH_PinPosition(PinConfig->GPIO_PinNumber);

    // Clear CNFx[1:0] MODEx[1:0] (4 bits per pin)
    (*ConfigRegister) &= ~(0xF << PinPosition);

    // Configure OUTPUT modes
    if ((PinConfig->GPIO_Mode == GPIO_MODE_OUTPUT_PUSH_PULL) ||
        (PinConfig->GPIO_Mode == GPIO_MODE_OUTPUT_OPEN_DRAIN) ||
        (PinConfig->GPIO_Mode == GPIO_MODE_AL_OUTPUT_PUSH_PULL) ||
        (PinConfig->GPIO_Mode == GPIO_MODE_AL_OUTPUT_OPEN_DRAIN))
    {
        // For output modes (4-7), subtract 4 to get CNF bits (0-3)
        // Then shift left by 2 and OR with speed bits
        PinConfig_Value = ((((PinConfig->GPIO_Mode) - 4) << 2) | (PinConfig->GPIO_Output_Speed)) & 0x0F;
    }
    // Configure INPUT modes (MODE[1:0] = 00)
    else
    {
        if ((PinConfig->GPIO_Mode == GPIO_MODE_ANALOG_INPUT) ||
            (PinConfig->GPIO_Mode == GPIO_MODE_FLOATING_INPUT))
        {
            // Analog or Floating input: CNF = mode, MODE = 00
            PinConfig_Value = ((PinConfig->GPIO_Mode << 2) | 0x0) & 0x0F;
        }
        else if (PinConfig->GPIO_Mode == GPIO_MODE_AL_INPUT)
        {
            // Alternate function input: configured as floating input
            PinConfig_Value = ((GPIO_MODE_FLOATING_INPUT << 2) | 0x0) & 0x0F;
        }
        else if (PinConfig->GPIO_Mode == GPIO_MODE_INPUT_PU)
        {
            // Input pull-up: CNF = 10, MODE = 00, ODR = 1
            PinConfig_Value = ((GPIO_MODE_INPUT_PU << 2) | 0x0) & 0x0F;
            GPIOx->ODR |= PinConfig->GPIO_PinNumber;
        }
        else // GPIO_MODE_INPUT_PD
        {
            // Input pull-down: CNF = 10, MODE = 00, ODR = 0
            PinConfig_Value = ((GPIO_MODE_INPUT_PD << 2) | 0x0) & 0x0F;
            GPIOx->ODR &= ~(PinConfig->GPIO_PinNumber);
        }
    }

    // Write configuration to register
    (*ConfigRegister) |= (PinConfig_Value << PinPosition);
}

/**
 * @FN            MCAL_GPIO_DeInit
 * @brief         Resets the specified GPIO port to its default reset state.
 *
 * @param[in]     GPIOx: Pointer to GPIO port (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE)
 *
 * @retval        None
 *
 * @note          - Uses RCC APB2 peripheral reset register
 *                - Does NOT disable the peripheral clock
 *                - All pins return to floating input mode
 */
void MCAL_GPIO_DeInit(GPIO_TypeDef *GPIOx)
{
    // Input validation
    if (GPIOx == NULL)
    {
        return;
    }

    // Reset using RCC APB2 peripheral reset register (RCC_APB2RSTR)
    // Set bit to 1 then clear to 0 to perform reset

    if (GPIOx == GPIOA)
    {
        RCC->APB2RSTR |= (1 << 2);
        RCC->APB2RSTR &= ~(1 << 2);
    }
    else if (GPIOx == GPIOB)
    {
        RCC->APB2RSTR |= (1 << 3);
        RCC->APB2RSTR &= ~(1 << 3);
    }
    else if (GPIOx == GPIOC)
    {
        RCC->APB2RSTR |= (1 << 4);
        RCC->APB2RSTR &= ~(1 << 4);
    }
    else if (GPIOx == GPIOD)
    {
        RCC->APB2RSTR |= (1 << 5);
        RCC->APB2RSTR &= ~(1 << 5);
    }
    else if (GPIOx == GPIOE)
    {
        RCC->APB2RSTR |= (1 << 6);
        RCC->APB2RSTR &= ~(1 << 6);
    }
}

/**
 * @FN            MCAL_GPIO_ReadPin
 * @brief         Reads the logic level of the specified GPIO pin.
 *
 * @param[in]     GPIOx: Pointer to GPIO port (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE)
 * @param[in]     PinNumber: GPIO pin to be read (@ref GPIO_PINS_DEFINE)
 *
 * @retval        uint8_t: Pin state (GPIO_PIN_STATE_HIGH or GPIO_PIN_STATE_LOW)
 *
 * @note          Reads from the IDR (Input Data Register)
 *
 * @example       uint8_t state = MCAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
 */
uint8_t MCAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t PinNumber)
{
    if (GPIOx == NULL)
    {
        return GPIO_PIN_STATE_LOW;
    }

    return ((GPIOx->IDR & PinNumber) ? GPIO_PIN_STATE_HIGH : GPIO_PIN_STATE_LOW);
}

/**
 * @FN            MCAL_GPIO_ReadPort
 * @brief         Reads the logic levels of all pins of the specified GPIO port.
 *
 * @param[in]     GPIOx: Pointer to GPIO port (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE)
 *
 * @retval        uint16_t: 16-bit port value (each bit represents a pin)
 *
 * @note          Reads from the IDR (Input Data Register)
 *
 * @example       uint16_t port_value = MCAL_GPIO_ReadPort(GPIOB);
 */
uint16_t MCAL_GPIO_ReadPort(GPIO_TypeDef *GPIOx)
{
    if (GPIOx == NULL)
    {
        return 0;
    }

    return (uint16_t)(GPIOx->IDR);
}

/**
 * @FN            MCAL_GPIO_WritePin
 * @brief         Writes a logic value to the specified GPIO pin.
 *
 * @param[in]     GPIOx: Pointer to GPIO port (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE)
 * @param[in]     PinNumber: GPIO pin to be written (@ref GPIO_PINS_DEFINE)
 * @param[in]     Value: Value to write (GPIO_PIN_STATE_HIGH or GPIO_PIN_STATE_LOW)
 *
 * @retval        None
 *
 * @note          - Pin must be configured as output or alternate function output
 *                - Uses BSRR for atomic bit set/reset operation
 *
 * @example       MCAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_STATE_HIGH);
 */
void MCAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t PinNumber, uint8_t Value)
{
    if (GPIOx == NULL)
    {
        return;
    }

    if (Value != GPIO_PIN_STATE_LOW)
    {
        // Set pin using BSRR (Bit Set/Reset Register) - atomic operation
        // Bits 15:0 set the corresponding ODR bit
        GPIOx->BSRR = (uint32_t)PinNumber;
    }
    else
    {
        // Reset pin using BSRR
        // Bits 31:16 reset the corresponding ODR bit
        GPIOx->BSRR = ((uint32_t)PinNumber << 16);
    }
}

/**
 * @FN            MCAL_GPIO_WritePort
 * @brief         Writes a 16-bit value to the GPIO output data register.
 *
 * @param[in]     GPIOx: Pointer to GPIO port (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE)
 * @param[in]     Value: 16-bit value to write (each bit controls a pin)
 *
 * @retval        None
 *
 * @note          - All pins affected must be configured as output
 *                - Writes directly to ODR register
 *
 * @example       MCAL_GPIO_WritePort(GPIOB, 0x00FF);
 */
void MCAL_GPIO_WritePort(GPIO_TypeDef *GPIOx, uint16_t Value)
{
    if (GPIOx == NULL)
    {
        return;
    }

    GPIOx->ODR = (uint32_t)Value;
}

/**
 * @FN            MCAL_GPIO_TogglePin
 * @brief         Toggles (inverts) the logic level of the specified GPIO pin.
 *
 * @param[in]     GPIOx: Pointer to GPIO port (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE)
 * @param[in]     PinNumber: GPIO pin to be toggled (@ref GPIO_PINS_DEFINE)
 *
 * @retval        None
 *
 * @note          Pin must be configured as output or alternate function output
 *
 * @example       MCAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
 */
void MCAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t PinNumber)
{
    if (GPIOx == NULL)
    {
        return;
    }

    GPIOx->ODR ^= PinNumber;
}

/**
 * @FN            MCAL_GPIO_TogglePort
 * @brief         Toggles (inverts) the logic levels of all pins of the GPIO port.
 *
 * @param[in]     GPIOx: Pointer to GPIO port (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE)
 *
 * @retval        None
 *
 * @note          All affected pins must be configured as output
 *
 * @example       MCAL_GPIO_TogglePort(GPIOB);
 */
void MCAL_GPIO_TogglePort(GPIO_TypeDef *GPIOx)
{
    if (GPIOx == NULL)
    {
        return;
    }

    GPIOx->ODR ^= 0xFFFF;
}

/**
 * @FN            MCAL_GPIO_LockPin
 * @brief         Locks the configuration of the specified GPIO pin.
 *
 * @param[in]     GPIOx: Pointer to GPIO port (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE)
 * @param[in]     PinNumber: GPIO pin to be locked (@ref GPIO_PINS_DEFINE)
 *
 * @retval        uint8_t: GPIO_LOCK_OK if successful, GPIO_LOCK_ERROR if failed
 *
 * @note          - Once locked, configuration cannot be changed until system reset
 *                - Follows the LCKR register lock sequence from reference manual:
 *                  1. Write 1
 *                  2. Write 0
 *                  3. Write 1
 *                  4. Read 0
 *                  5. Read 1 (optional but confirms lock is active)
 *
 * @example       if (MCAL_GPIO_LockPin(GPIOA, GPIO_PIN_0) == GPIO_LOCK_OK)
 *                { /* Pin successfully locked }**/


uint8_t MCAL_GPIO_LockPin(GPIO_TypeDef *GPIOx, uint16_t PinNumber)
{
    if (GPIOx == NULL)
    {
        return GPIO_LOCK_ERROR;
    }

    // LOCK key writing sequence (Reference Manual)
    // Bit 16: LCKK (Lock Key)
    // Bits 15:0: LCKy (Lock bits for each pin)

    volatile uint32_t temp = (1 << 16);
    temp |= PinNumber;

    // Write 1 (LCKK=1, LCKy=pin)
    GPIOx->LCKR = temp;

    // Write 0 (LCKK=0, LCKy=pin)
    GPIOx->LCKR = PinNumber;

    // Write 1 (LCKK=1, LCKy=pin)
    GPIOx->LCKR = temp;

    // Read 0 (dummy read)
    temp = GPIOx->LCKR;

    // Read 1 and verify LCKK bit is set
    if ((GPIOx->LCKR & (1 << 16)))
    {
        return GPIO_LOCK_OK;
    }
    else
    {
        return GPIO_LOCK_ERROR;
    }
}
