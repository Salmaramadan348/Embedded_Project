#include "bitManipulation.h"
#include "bit_Utilies.h"
#include "tm4c123gh6pm.h"
#include "UART.h"
#include <stdint.h>
#include <string.h>
#define CB 0x2A
#define UART_LCRH_WLEN_8 0x00000060
#define UART_LCRH_FEN 0x00000010
#define CR   			  0x0D
#define END    			0x2A
//UART0 initialization on port A

void UART0_Init (void){
	SYSCTL_RCGCUART_R |= 0x0001; 	//activate UART0
	while((SYSCTL_PRUART_R & 0x00000001) == 0);
	SYSCTL_RCGCGPIO_R |= 0x0001;  //activate portA clk
	while((SYSCTL_PRGPIO_R & 0x01) == 0);
	UART0_CTL_R &= ~0x0001;				//disable UART
	UART0_IBRD_R = 0x68;
	UART0_FBRD_R = 0xB;
	UART0_LCRH_R = 0x070;					//8-bits Data + enable FIFO
	UART0_CTL_R = 0x0301;					//activate RXE, TXE & UART
	GPIO_PORTA_AFSEL_R |= 0x03; 	//enable alternate function PA0 & PA1
	GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFFFFFF00) | 0x00000011;
	GPIO_PORTA_DEN_R |= 0x03;			//enable digtal I/O 
	GPIO_PORTA_AMSEL_R &= ~0x03;	//disable analog I/O
}


void UART2_Init (void){
	
	SYSCTL_RCGCUART_R |= 0x0004; 	//activate UARTD
	//while((SYSCTL_PRUART_R & 0x0004) == 0);
	SYSCTL_RCGCGPIO_R |= 0x0008;  //activate portD clk
	while((SYSCTL_PRGPIO_R & 0x08) == 0);
	GPIO_PORTD_LOCK_R = 0x4C4F434B;  // Unlock the GPIOCR register
	GPIO_PORTD_CR_R |= 0xC0; 
	UART2_CTL_R &= ~0x0001;				//disable UART
	UART2_IBRD_R = 0x68;					//IBRD = int(80000000/(16*9600)) = int(520.8333)
	UART2_FBRD_R = 0xB;					//FBRD = int (0.833*64 + 0.5)
	UART2_LCRH_R |= 0x070;					//8-bits Data + enable FIFO
	UART2_CTL_R |= 0x0301;					//activate RXE, TXE & UART
	GPIO_PORTD_AFSEL_R |= 0xC0; 	//enable alternate function PD6 & PD7
	GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R & 0x00FFFFFF) | 0x11000000;
	GPIO_PORTD_DEN_R |= 0xC0;			//enable digtal I/O 
	GPIO_PORTD_AMSEL_R &= ~0xC0;	//disable analog I/O
}


void UART1_Init (void){
	SYSCTL_RCGCUART_R |= (1 << 1);
	SYSCTL_RCGCGPIO_R |= (1 << 1);
	while ((SYSCTL_PRGPIO_R & (1 << 1)) == 0);
	
	GPIO_PORTB_LOCK_R = 0x4C4F434B;  // Unlock the GPIOCR register
	GPIO_PORTB_CR_R |= 0x03; 
	CLR_BIT(UART1_CTL_R , 0);
	UART1_IBRD_R = 104;
	UART1_FBRD_R = 11;
	UART1_CTL_R = 0x0301;
	UART1_LCRH_R = 0x0070;
	GPIO_PORTB_AFSEL_R |= 0x03;
	GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFFFFFF00) + 0x00000011;
	GPIO_PORTB_DEN_R |= 0x03;
	GPIO_PORTB_AMSEL_R &= ~0x03;
}
void UART5_Init (void){
	SYSCTL_RCGCUART_R |= 0x0020; 	//activate UARTE
	//while((SYSCTL_PRUART_R & 0x0020) == 0);
	SYSCTL_RCGCGPIO_R |= 0x0010;  //activate portE clk
	while((SYSCTL_PRGPIO_R & 0x0010) == 0);
	GPIO_PORTE_LOCK_R = 0x4C4F434B;  // Unlock the GPIOCR register
	GPIO_PORTE_CR_R |= 0x30; 
	UART5_CTL_R &= ~0x0001;				//disable UART
	UART5_IBRD_R = 0x68;					//IBRD = int(16000000/(16*9600)) = int(104.16667)
	UART5_FBRD_R = 0xB;					  //FBRD = int (0.16667*64 + 0.5)
	UART5_LCRH_R = 0x070;					//8-bits Data + enable FIFO
	UART5_CTL_R = 0x0301;					//activate RXE, TXE & UART
	GPIO_PORTE_AFSEL_R |= 0x30; 	//enable alternate function PE4 & PE5
	GPIO_PORTE_PCTL_R = (GPIO_PORTE_PCTL_R & 0xFF00FFFF) | 0x00110000;
	GPIO_PORTE_DEN_R |= 0x30;			//enable digtal I/O 
	GPIO_PORTE_AMSEL_R &= ~0x30;	//disable analog I/O
}


void UART0_SendChar (char data){
		while ((UART0_FR_R & 0x20));
	UART0_DR_R = data;
}

void UART0_SendString (char *pt){
		while (*pt){
			UART0_SendChar(*pt);
			pt++;
		}
}

char UART0_Recievechar (){
		while ((UART0_FR_R & 0x10));
	return (char)(UART0_DR_R&0xFF);
}
char UART1_Recievechar (){
		while ((UART1_FR_R & 0x10));
	return (char)(UART1_DR_R&0xFF);
}

void UART0_RecieveString (char *command, int length){
	char chararacter;
	int i;
	for(i =0;i<length;i++){
	  	chararacter = UART0_Recievechar();
			if(chararacter!=CR)
			{
				command[i] = chararacter;
				UART0_SendChar(command[i]);
			}
			else if (chararacter == CR || i == length) break;
	}
}

void UART2_SendChar (char data){
		while ((UART2_FR_R & 0x20));
	UART2_DR_R = data;
}

void UART2_SendString (char *pt){
		while (*pt){
			UART2_SendChar(*pt);
			pt++;
		}
}
char UART5_Recievechar (){
		while ((UART5_FR_R & 0x10));
	return (char)(UART5_DR_R&0xFF);
}
char UART2_Recievechar (){
		while ((UART2_FR_R & 0x10));
	return (char)(UART2_DR_R&0xFF);
}

void UART2_RecieveString (char *command, int length){
	char chararacter;
	int i;
	for(i =0;i<length;i++){
	  	chararacter = UART2_Recievechar();
			if(chararacter != END)
			{
				command[i] = chararacter;
				UART2_SendChar(command[i]);
			}
			else if (chararacter == END || i == length){break;}
	}
}