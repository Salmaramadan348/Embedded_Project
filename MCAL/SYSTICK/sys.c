#include <stdint.h>
#include "Sys.h"
#include "bitManipulation.h"
#include "tm4c123gh6pm.h"

// Function to initialize the SysTick timer
void SysTick_Init(void){
    NVIC_ST_CTRL_R = 0; // Disable SysTick by clearing the control register
    NVIC_ST_RELOAD_R = 0x00FFFFFF; // Set the maximum reload value (for a long delay)
    NVIC_ST_CURRENT_R = 0; // Clear the current value register
    NVIC_ST_CTRL_R = 0x05; // Enable SysTick timer with system clock source
}

void delay_ms(void){
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = 16000-1; //1 msec.
	NVIC_ST_CURRENT_R = 0; //clear current register and COUNTFLAG
	NVIC_ST_CTRL_R = 5; //enable and clock source 101 = 5
	while((NVIC_ST_CTRL_R & 0x10000) == 0){}
}

// Function to generate a delay using SysTick
void SysTick_Wait(uint32_t delay){
    NVIC_ST_RELOAD_R = delay - 1; // Set the reload value for the delay (reload value is delay-1)
    NVIC_ST_CURRENT_R = 0; // Clear the current value register to start counting
    while((NVIC_ST_CTRL_R & 0x00010000) == 0); // Wait until the COUNT flag in the SysTick control register is set (indicating timer has finished)
}

// Function to generate a delay of 10 milliseconds using SysTick
void SysTick_Wait10ms(uint32_t delay){
    unsigned long i;
    for(i = 0; i < delay; i++){
        SysTick_Wait(160000); // Call SysTick_Wait to create a delay of approximately 10ms (assuming 16 MHz system clock)
    }
}

