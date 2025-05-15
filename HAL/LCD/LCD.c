#include "LCD.h"

//port E control & port B data
                 
void LCD_init(void){
	//port E control PE1 --> RS | PE2 --> RW | PE3 --> E
	//port B data    PB4 --> D4 | PB5 --> D5 | PB6 --> D6 | PB7 --> D7
	SYSCTL_RCGCGPIO_R |= 0x12;              //Port B & E clock enable
	while((SYSCTL_PRGPIO_R & 0x12) == 0);
	
	//Control port configurations
	GPIO_PORTE_AFSEL_R &= ~(RS|RW|E);       
	GPIO_PORTE_PCTL_R  &= ~(RS|RW|E);       
	GPIO_PORTE_AMSEL_R &= ~(RS|RW|E);     
	GPIO_PORTE_DEN_R   |= (RS|RW|E);     
	GPIO_PORTE_DIR_R   |= (RS|RW|E);    
	
	//Data port configurations
	GPIO_PORTB_AFSEL_R =0;                 
	GPIO_PORTB_PCTL_R  =0;                   
	GPIO_PORTB_AMSEL_R = 0;                 
	GPIO_PORTB_DEN_R = 0XFF;                
	GPIO_PORTB_DIR_R |= 0xF0;                 
	
	
	LCD_sendCommand(FOUR_BITS_DATA_MODE);          
	LCD_sendCommand(TWO_LINE_LCD_FOUR_BIT_MODE);    
	LCD_sendCommand(CURSOR_OFF);                      
	LCD_sendCommand(CLEAR_COMMAND);                    
}

void LCD_sendCommand(uint8_t command){
     GPIO_PORTE_DATA_R &= ~RS;       // Disable RS
     GPIO_PORTE_DATA_R &= ~RW;       // Disable RW
     delay_ms();                     // wait for 1 ms
	 
     GPIO_PORTE_DATA_R |= E;         // Enable E
     delay_ms();                     // wait for 1 ms
	 
     GPIO_PORTB_DATA_R = (GPIO_PORTB_DATA_R & 0x0F) | (command & 0xF0);
     delay_ms();                     
	 
     GPIO_PORTE_DATA_R &= ~E;         // Disable E
     delay_ms();                      
	 
     GPIO_PORTE_DATA_R |= E;          // Enable E
     delay_ms();                      
	 
     GPIO_PORTB_DATA_R = (GPIO_PORTB_DATA_R & 0x0F) | ((command & 0x0F) << 4);
     delay_ms();               
	 
     GPIO_PORTE_DATA_R &= ~E;        // Disable E
     delay_ms();
}

void LCD_clearScreen(void){
    LCD_sendCommand(CLEAR_COMMAND); //clear screen
}

void LCD_displayCharacter(uint8_t data){
    GPIO_PORTE_DATA_R |= RS;    //Enable RS
    GPIO_PORTE_DATA_R &= ~RW;   //Disable RW
    delay_ms();                 // wait for 1ms
	
    GPIO_PORTE_DATA_R |= E;     //Enable E
    delay_ms();                
	
    //Configure 8 bit mode or 4 bit mode
    GPIO_PORTB_DATA_R = (GPIO_PORTB_DATA_R & 0x0F) | (data & 0xF0);
    delay_ms(); 
	                
    GPIO_PORTE_DATA_R &= ~E;    //Disable E
    delay_ms();     
	            
    GPIO_PORTE_DATA_R |= E;     //Enable E
    delay_ms();                 
	
    //out the lowest 4 bits of the required data to the data bus B4 --> B7
    GPIO_PORTB_DATA_R = (GPIO_PORTB_DATA_R & 0x0F) | ((data & 0x0F) << 4);
    delay_ms();                 
	
    GPIO_PORTE_DATA_R &= ~E;    // Disable E
    delay_ms();               
}

void LCD_displayString(const char *Str){
	uint8_t i = 0;
	while(Str[i] != '\0')
	{
		LCD_displayCharacter(Str[i]);
		i++;
	}
}

void LCD_goToRowColumn(uint8_t row,uint8_t col)
{
	uint8_t Address;
	switch(row)
	{
		case 0:
		Address=col;
		break;
		case 1:
		Address=col+0x40;
		break;
		case 2:
		Address=col+0x10;
		break;
		case 3:
		Address=col+0x50;
		break;
	}
	
	LCD_sendCommand(Address | SET_CURSOR_LOCATION);
}

void LCD_intgerToString(double data)
{
	char buff[16]; 
	sprintf(buff, "%f", data);
	LCD_displayString(buff);
}
