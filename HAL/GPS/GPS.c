#include <stdio.h>
#include <math.h>
#include <string.h> // for string functions
#include <stdlib.h> // for atof function
#include <stdbool.h>
#include "bitManipulation.h"
#include "tm4c123gh6pm.h"
#include "GPIO.h"
#include "UART.h"
#include "GPS.h"

char GPS_logname[] = "$GPRMC";
char GPSS [80];
char GPS_formated[12][20];
char* token;

/////////////////////////////////////////////////////////////////////////////////////////////////////
float allLats[200];
int latscount=0;
float allLongs[200];
int longscount=0;


// extract $GPRMC message content
void GPS_read() {
	char i = 0;
	char counter = 0;

	// wait till $GPRMC message
	//UART0_SendChar(UART2_Recievechar());
	do {
		while (UART2_Recievechar() != GPS_logname[i]) {
			i = 0;
		}
		i++;
	} while (i != 6);

	// extract GPRMC message content

	do {
		GPSS[counter] =  UART2_Recievechar();
		
		counter++;
		
	} while (GPSS[counter - 1] != '*');
	 UART0_SendChar('\n');
}

void GPS_readChk(char GPScheck[]) {//testing read
	char i = 0;
	char counter = 0;
int ini=0;
	// wait till $GPRMC message
	do {
		while ( GPScheck[ini]!= GPS_logname[i]) {
			i = 0;ini++;
		}
		i++;ini++;
	} while (i != 6);

	// extract GPRMC message content

	do {
		GPSS[counter] =  GPScheck[ini] ;
		counter++;ini++;
	} while (GPSS[counter - 1] != '*');
	 
}
// extract Latitude  and Longitude of the $GPRMC message
void GPS_format(float *currentLat, float *currentLong) {
	char noofStrings = 0;
	token = strtok(GPSS , ",");

	do {
		strcpy(GPS_formated[noofStrings], token);
		token = strtok(NULL, ",");
		noofStrings++;
	} while (token != NULL);

	if (strcmp(GPS_formated[1], "A") == 0) {
		if (strcmp(GPS_formated[3], "N") == 0){
			*currentLat = atof(GPS_formated[2]);
		allLats[latscount++]= *currentLat;}
		else{
			*currentLat = -(atof(GPS_formated[2]));
		allLats[latscount++]= *currentLat;}

		if (strcmp(GPS_formated[5], "E") == 0){
			*currentLong = atof(GPS_formated[4]);
			allLongs[longscount++]= *currentLong;
		}
		else{
			*currentLong = -(atof(GPS_formated[4]));
			allLongs[longscount++]= *currentLong;
		}
	}
}

float ToDegree(float angle) {
    int degree = (int)angle / 100;
    float minutes = angle - (float)degree * 100;
    return (degree + (minutes / 60));
}

float ToRadian(float degrees) {
	return degrees * PI / 180.0;
}
float Get_Distance(float startLatitude, float startLongitude, float endLatitude, float endLongitude) {
    // convert all coordinates from Degrees into Radians
    float startLat = ToRadian(ToDegree(startLatitude));
    float startLong = ToRadian(ToDegree(startLongitude));
    float endLat = ToRadian(ToDegree(endLatitude));
    float endLong = ToRadian(ToDegree(endLongitude));

    // calculate latitude difference and longitude difference
    float latDifference = endLat - startLat;
    float longDifference = endLong - startLong;

    // use Haversine formula
    float a = sin(latDifference / 2) * sin(latDifference / 2) + cos(startLat) * cos(endLat)
              * sin(longDifference / 2) * sin(longDifference / 2);
    float c = 2 * atan2(sqrt(a), sqrt(1 - a));

    // Multiply by Earth's Radius to get the distance
    return c * Earth_Radius;
}


