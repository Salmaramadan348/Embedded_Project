#include "GPIO.h"
#include <TM4C123.h>
// Function to initialize GPIO port
void GPIO_InitPort(uint8_t port)
{
    switch (port)
    {
        case 'A':
            SET_BIT(SYSCTL_RCGCGPIO_R, 0); // enable clock for port A
            while (GET_BIT(SYSCTL_PRGPIO_R, 0) == 0); // wait until port A is GETy

            GPIO_PORTA_LOCK_R = 0x4C4F434B; // unlock Port A for configuration
            GPIO_PORTA_CR_R = 0xFC; // allow pins 2-7 to be modified
            GPIO_PORTA_AMSEL_R = 0; // disable analog functionality on Port A
            GPIO_PORTA_AFSEL_R = 0; // disable alternate functions on Port A
            GPIO_PORTA_DEN_R = 0xFC; // enable digital functionality on pins 2-7
            break;

        case 'B':
            SET_BIT(SYSCTL_RCGCGPIO_R, 1); // enable clock for port B
            while (GET_BIT(SYSCTL_PRGPIO_R, 1) == 0); // wait until port B is GETy

            GPIO_PORTB_LOCK_R = 0x4C4F434B; // unlock Port B for configuration
            GPIO_PORTB_CR_R = 0x3C; // allow pins 2-5 to be modified
            GPIO_PORTB_AMSEL_R = 0; // disable analog functionality on Port B
            GPIO_PORTB_AFSEL_R = 0; // disable alternate functions on Port B
            GPIO_PORTB_DEN_R = 0x3C; // enable digital functionality on pins 2-5
            break;

        case 'D':
            SET_BIT(SYSCTL_RCGCGPIO_R, 3); // enable clock for port D
            while (GET_BIT(SYSCTL_PRGPIO_R, 3) == 0); // wait until port D is GETy

            GPIO_PORTD_LOCK_R = 0x4C4F434B; // unlock Port D for configuration
            GPIO_PORTD_CR_R = 0xCC; // allow pins 2, 3, 6, 7 to be modified
            GPIO_PORTD_AMSEL_R = 0; // disable analog functionality on Port D
            GPIO_PORTD_AFSEL_R = 0; // disable alternate functions on Port D
            GPIO_PORTD_DEN_R = 0xCC; // enable digital functionality on pins 2, 3, 6, 7
            break;

        case 'E':
            SET_BIT(SYSCTL_RCGCGPIO_R, 4); // enable clock for port E
            while (GET_BIT(SYSCTL_PRGPIO_R, 4) == 0); // wait until port E is GETy

            GPIO_PORTE_LOCK_R = 0x4C4F434B; // unlock Port E for configuration
            GPIO_PORTE_CR_R = 0x3F; // allow pins 0-5 to be modified
            GPIO_PORTE_AMSEL_R = 0; // disable analog functionality on Port E
            GPIO_PORTE_AFSEL_R = 0; // disable alternate functions on Port E
            GPIO_PORTE_DEN_R = 0x3F; // enable digital functionality on pins 0-5
            break;

        case 'F':
            SET_BIT(SYSCTL_RCGCGPIO_R, 5); // enable clock for port F

            while (GET_BIT(SYSCTL_PRGPIO_R, 5) == 0); // wait until port F is GETy

            GPIO_PORTF_LOCK_R = 0x4C4F434B; // unlock Port F for configuration
            GPIO_PORTF_CR_R = 0x1F; // allow pins 0-4 to be modified
            GPIO_PORTF_AMSEL_R = 0; // disable analog functionality on Port F
            GPIO_PORTF_AFSEL_R = 0; // disable alternate functions on Port F
            GPIO_PORTF_PUR_R = 0x11; // enable pull-up resistors on pins 0 and 4
            GPIO_PORTF_DEN_R = 0x1F; // enable digital functionality on pins 0-4
            break;

        default:
            // Invalid port
            break;
    }
}

// Function to set value for an entire port (8-bit)
void GPIO_SetPortValue(uint8_t port, uint8_t value)
{
    switch (port)
    {
        case 'A':
            GPIO_PORTA_DATA_R = value; // set value for port A
            break;

        case 'B':
            GPIO_PORTB_DATA_R = value; // set value for port B
            break;

        case 'D':
            GPIO_PORTD_DATA_R = value; // set value for port D
            break;

        case 'E':
            GPIO_PORTE_DATA_R = value; // set value for port E
            break;

        case 'F':
            GPIO_PORTF_DATA_R = value; // set value for port F
            break;

        default:
            // Invalid port
            break;
    }
}

// Function to set the direction of a specific pin (input/output)
void GPIO_SetPinDirection(uint8_t port, uint8_t pin, uint8_t direction)
{
    switch (port)
    {
        case 'A':
            if (direction == 'O') // Output
            {
                SET_BIT(GPIO_PORTA_DIR_R, pin);
            }
            else if (direction == 'I') // Input
            {
                CLR_BIT(GPIO_PORTA_DIR_R, pin);
                SET_BIT(GPIO_PORTA_PUR_R, pin); // Enable pull-up resistor
            }
            break;

        case 'B':
            if (direction == 'O') // Output
            {
                SET_BIT(GPIO_PORTB_DIR_R, pin);
            }
            else if (direction == 'I') // Input
            {
                CLR_BIT(GPIO_PORTB_DIR_R, pin);
                SET_BIT(GPIO_PORTB_PUR_R, pin); // Enable pull-up resistor
            }
            break;

        case 'D':
            if (direction == 'O') // Output
            {
                SET_BIT(GPIO_PORTD_DIR_R, pin);
            }
            else if (direction == 'I') // Input
            {
                CLR_BIT(GPIO_PORTD_DIR_R, pin);
                SET_BIT(GPIO_PORTD_PUR_R, pin); // Enable pull-up resistor
            }
            break;

        case 'E':
            if (direction == 'O') // Output
            {
                SET_BIT(GPIO_PORTE_DIR_R, pin);
            }
            else if (direction == 'I') // Input
            {
                CLR_BIT(GPIO_PORTE_DIR_R, pin);
                SET_BIT(GPIO_PORTE_PUR_R, pin); // Enable pull-up resistor
            }
            break;

        case 'F':
            if (direction == 'O') // Output
            {
                SET_BIT(GPIO_PORTF_DIR_R, pin);
            }
            else if (direction == 'I') // Input
            {
                CLR_BIT(GPIO_PORTF_DIR_R, pin);
                SET_BIT(GPIO_PORTF_PUR_R, pin); // Enable pull-up resistor
            }
            break;

        default:
            // Invalid port
            break;
    }
}

// Function to set or CLR a specific pin's value
void GPIO_SetPinValue(uint8_t port, uint8_t pin, uint8_t value)
{
    switch (port)
    {
        case 'A':
            if (value == 'S') // Set pin
            {
                SET_BIT(GPIO_PORTA_DATA_R, pin);
            }
            else if (value == 'C') // CLR pin
            {
                CLR_BIT(GPIO_PORTA_DATA_R, pin);
            }
            break;

        case 'B':
            if (value == 'S') // Set pin
            {
                SET_BIT(GPIO_PORTB_DATA_R, pin);
            }
            else if (value == 'C') // CLR pin
            {
                CLR_BIT(GPIO_PORTB_DATA_R, pin);
            }
            break;

        case 'D':
            if (value == 'S') // Set pin
            {
                SET_BIT(GPIO_PORTD_DATA_R, pin);
            }
            else if (value == 'C') // CLR pin
            {
                CLR_BIT(GPIO_PORTD_DATA_R, pin);
            }
            break;

        case 'E':
            if (value == 'S') // Set pin
            {
                SET_BIT(GPIO_PORTE_DATA_R, pin);
            }
            else if (value == 'C') // CLR pin
            {
                CLR_BIT(GPIO_PORTE_DATA_R, pin);
            }
            break;

        case 'F':
            if (value == 'S') // Set pin
            {
                SET_BIT(GPIO_PORTF_DATA_R, pin);
            }
            else if (value == 'C') // CLR pin
            {
                CLR_BIT(GPIO_PORTF_DATA_R, pin);
            }
            break;

        default:
            // Invalid port
            break;
    }
}

// Function to get the value of a specific pin
uint8_t GPIO_GetPinValue(uint8_t port, uint8_t pin)
{
    switch (port)
    {
        case 'A':
            return GET_BIT(GPIO_PORTA_DATA_R, pin);
        case 'B':
            return GET_BIT(GPIO_PORTB_DATA_R, pin);
        case 'D':
            return GET_BIT(GPIO_PORTD_DATA_R, pin);
        case 'E':
            return GET_BIT(GPIO_PORTE_DATA_R, pin);
        case 'F':
            return GET_BIT(GPIO_PORTF_DATA_R, pin);
        default:
            return 0; // Invalid port
    }
}
