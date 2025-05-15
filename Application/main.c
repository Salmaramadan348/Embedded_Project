
#include <math.h>
#include <stdlib.h>
#include "bitManipulation.h"
#include "tm4c123gh6pm.h"
#include "GPIO.h"
#include "UART.h"
#include "Sys.h"
#include "GPS.h"
#include "LCD.h"
#include <TM4C123.h>

#include "stdint.h"

typedef struct{
	float lat;
	float lon;
	char name[15];
} Landmark;

/*char GPScheck[6][90] = {
	"sdasdsa$GPRMC,143328.00,A,3003.8490,N,3116.8114,E,2.822,180.13,050525,,,A*6A",//"Hall A & B" test
"sdasdsa$GPRMC,143328.00,A,3003.8184,N,3116.8276,E,2.822,180.13,050525,,,A*6A",//HALL C D TEST
	"sdasdsa$GPRMC,143328.00,A,3003.8076,N,3116.6998,E,2.822,180.13,050525,,,A*6A",//credit check
"sdasdsa$GPRMC,143328.00,A,3003.8904,N,3116.7280,E,2.822,180.13,050525,,,A*6A",//main buidling check
	"sdasdsa$GPRMC,143328.00,A,3003.7884,N,3116.7706,E,2.822,180.13,050525,,,A*6A",//luban check
	"sdasdsa$GPRMC,143328.00,A,3003.08745,N,03119.922336,E,2.822,180.13,050525,,,A*6A"//no landmarkcheck

}*/





Landmark landmarks[] = {
	{3003.8490, 3116.8144, "Hall A & B"},
	{3003.8219, 3116.8358, "Hall C & D"},
	{3003.8076, 3116.6998, "Credit"},
	{3003.8904, 3116.7280, "Main Building"},
	{3003.7884, 3116.7706, "Luban"}
};

int NearestLandmark(float Lat, float Long){
	int i=0, minIndex = -1, minDist = 100;
	for (i = 0; i < 5; i++){
		float Dist = Get_Distance(Lat,Long,landmarks[i].lat,landmarks[i].lon);
		if (Dist <= minDist){
			minIndex = i;
			minDist = Dist;
		}
	}
		
		return minIndex;
}
	
int main(){
	char displacement_display[20] ,lat_display[20] ,lon_display[20];
	float lat, lon, displacement =0,gg;
    int ind;int ini=-1;
	//int ab=0,cd=0,lu=0,cr=0,ma=0;
		//int mode, toHallA_B, toHallD_C, toMainBuilding, toCredit;
//**** Initializations ****
	 GPIO_InitPort('F');
	 GPIO_InitPort('B');
	 //GPIO_InitPort('D');
	
	 GPIO_SetPinDirection('F',1,'O');
	 GPIO_SetPinDirection('F',2,'O');
	 GPIO_SetPinDirection('F',3,'O');
	 GPIO_SetPinDirection('B',0,'O');
	 GPIO_SetPinDirection('B',1,'O');
	
	/* GPIO_SetPinDirection('D',0,'I');
	 GPIO_SetPinDirection('D',1,'I');
	 GPIO_SetPinDirection('D',2,'I');
	 GPIO_SetPinDirection('D',3,'I');
	 GPIO_SetPinDirection('B',2,'I');*/
	
	 SysTick_Init();
	
	
	 UART2_Init();
   UART0_Init();
	
   LCD_init();
   LCD_sendCommand(0x80);
   LCD_displayString("Welcome");
	 
	 UART0_SendString("initialization compelete\n");
	 
	 SysTick_Wait10ms(300); // 3 seconds delay
	 
	 
///
///////////////////////////////////////////////////////////////////////

while(1){ 
	/*  mode= GPIO_GetPinValue('B', 2); ;
	  toHallA_B= GPIO_GetPinValue('D', 0);
    toHallD_C= GPIO_GetPinValue('D', 1);
	  toMainBuilding= GPIO_GetPinValue('D', 2);
	 toCredit= GPIO_GetPinValue('D', 3);*/
	
	GPS_read();
	/*==========================testing without UART==========================
	
	ini=ini+1;
	ini=ini%6;
	GPS_readChk(GPScheck[ini]);
*/
	GPIO_SetPinValue('F',2,'S');
	
	GPS_format(&lat,&lon);
	
	
	
 //if (mode||((!(toHallA_B)&& !(toHallD_C)&& !(toMainBuilding)&&  !(toCredit))&&(!(ab||cd||lu||ma||cr )))){
	ind = NearestLandmark(lat, lon);  // index of nearest landmark
	 
    if(ind == -1) {
			GPIO_SetPinValue('F',1,'S');
		// Uart Display
		UART0_SendString("No nearby landmark found \n");
			
		// LCD Display
		LCD_clearScreen();
		LCD_displayString("No landmark");
    }
    else {
			displacement = Get_Distance(lat,lon,landmarks[ind].lat,landmarks[ind].lon); // calculate distance between current place and nearest landmark
		GPIO_SetPinValue('F',3,'S');
		
		// Uart Display
		UART0_SendString("Nearest landmark is ");
		UART0_SendString(landmarks[ind].name);
		UART0_SendString("\n");
		
		UART0_SendString("Displacement is: ");
		sprintf(displacement_display, "%f", displacement);
		UART0_SendString(displacement_display);
		UART0_SendString("\n ");
		
		// LCD Display
		LCD_clearScreen();
		LCD_displayString("Near: ");
		LCD_displayString(landmarks[ind].name);
		LCD_goToRowColumn(1, 0);
		LCD_displayString("Dist: ");
		LCD_intgerToString(displacement);
				if (displacement <40)    // Turn on buzzer
		{
		GPIO_SetPinValue('B',1,'S');
			GPIO_SetPinValue('B',0,'S');
			UART0_SendString("You are too close \n");
    SysTick_Wait10ms(300); // 3 seconds delay
        

        // Turn off buzzer
        GPIO_SetPinValue('B',1,'C');
			
			     GPIO_SetPinValue('B',0,'C');
       
		}
	else if (displacement <60)    // Turn on buzzer
		{
			UART0_SendString("You are close \n");
		GPIO_SetPinValue('B',0,'S');
    SysTick_Wait10ms(300); // 3 seconds delay
        

        // Turn off buzzer
        GPIO_SetPinValue('B',0,'C');
		}
		 
	}
	
	SysTick_Wait10ms(300); // 3 seconds delay
	GPIO_SetPinValue('F',1,'C');
	GPIO_SetPinValue('F',3,'C');
	GPIO_SetPinValue('F',2,'C');}
		

  /*else if ((toHallA_B) && (!(toHallD_C)&& !(toMainBuilding)  && !(toCredit))||ab ) {
	displacement = Get_Distance(lat,lon,landmarks[0].lat,landmarks[0].lon);
	cd=0;ab=1,ma=0,cr=0;
		// LCD Display
		LCD_clearScreen();
		LCD_displayString(landmarks[0].name);
		LCD_goToRowColumn(1, 0);
		LCD_displayString("Dist: ");
		LCD_intgerToString(displacement);
				if (displacement <40)    // Turn on buzzer
		{
		GPIO_SetPinValue('B',1,'S');
			GPIO_SetPinValue('B',0,'S');
    SysTick_Wait10ms(300); // 3 seconds delay
        

        // Turn off buzzer
        GPIO_SetPinValue('B',1,'C');
			
			     GPIO_SetPinValue('B',0,'C');
       
		}
	else if (displacement <60)    // Turn on buzzer
		{
		GPIO_SetPinValue('B',0,'S');
    SysTick_Wait10ms(300); // 3 seconds delay
        

        // Turn off buzzer
        GPIO_SetPinValue('B',0,'C');
		}
	}
	  else if (!(toHallA_B) && (toHallD_C)&& !(toMainBuilding)  && !(toCredit) ||cd) {
	displacement = Get_Distance(lat,lon,landmarks[1].lat,landmarks[1].lon);
	cd=1;ab=0,ma=0,cr=0;
		// LCD Display
		LCD_clearScreen();
		LCD_displayString(landmarks[1].name);
		LCD_goToRowColumn(1, 0);
		LCD_displayString("Dist: ");
		LCD_intgerToString(displacement);
				if (displacement <40)    // Turn on buzzer
		{
		GPIO_SetPinValue('B',1,'S');
			GPIO_SetPinValue('B',0,'S');
    SysTick_Wait10ms(300); // 3 seconds delay
        

        // Turn off buzzer
        GPIO_SetPinValue('B',1,'C');
			
			     GPIO_SetPinValue('B',0,'C');
       
		}
	else if (displacement <60)    // Turn on buzzer
		{
		GPIO_SetPinValue('B',0,'S');
    SysTick_Wait10ms(300); // 3 seconds delay
        

        // Turn off buzzer
        GPIO_SetPinValue('B',0,'C');
		}
	}

		  else if (!(toHallA_B)&& !(toHallD_C)&& (toMainBuilding)&& !(toCredit) ||ma) {
	displacement = Get_Distance(lat,lon,landmarks[3].lat,landmarks[3].lon);
	cd=0;ab=0,ma=1,cr=0;
		// LCD Display
		LCD_clearScreen();
		LCD_displayString(landmarks[3].name);
		LCD_goToRowColumn(1, 0);
		LCD_displayString("Dist: ");
		LCD_intgerToString(displacement);
				if (displacement <40)    // Turn on buzzer
		{
		GPIO_SetPinValue('B',1,'S');
			GPIO_SetPinValue('B',0,'S');
    SysTick_Wait10ms(300); // 3 seconds delay
        

        // Turn off buzzer
        GPIO_SetPinValue('B',1,'C');
			
			     GPIO_SetPinValue('B',0,'C');
       
		}
	else if (displacement <60)    // Turn on buzzer
		{
		GPIO_SetPinValue('B',0,'S');
    SysTick_Wait10ms(300); // 3 seconds delay
        

        // Turn off buzzer
        GPIO_SetPinValue('B',0,'C');
		}
	}	
	  else if (!(toHallA_B) && !(toHallD_C)&& !(toMainBuilding)  && (toCredit) ||cr) {
	displacement = Get_Distance(lat,lon,landmarks[2].lat,landmarks[2].lon);
	cd=0;ab=0,ma=0,cr=1;
		// LCD Display
		LCD_clearScreen();
		LCD_displayString(landmarks[2].name);
		LCD_goToRowColumn(1, 0);
		LCD_displayString("Dist: ");
		LCD_intgerToString(displacement);
				if (displacement <40)    // Turn on buzzer
		{
		GPIO_SetPinValue('B',1,'S');
			GPIO_SetPinValue('B',0,'S');
    SysTick_Wait10ms(300); // 3 seconds delay
        

        // Turn off buzzer
        GPIO_SetPinValue('B',1,'C');
			
			     GPIO_SetPinValue('B',0,'C');
       
		}
	else if (displacement <60)    // Turn on buzzer
		{
		GPIO_SetPinValue('B',0,'S');
    SysTick_Wait10ms(300); // 3 seconds delay
        

        // Turn off buzzer
        GPIO_SetPinValue('B',0,'C');
		}
	}*/
}


	