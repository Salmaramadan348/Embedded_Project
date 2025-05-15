#ifndef GPS
#define GPS


#define PI 3.141592653589793238462643383279502884197
#define Earth_Radius  	6371000

void GPS_read();
void GPS_readChk();
void GPS_format(float *currentLat, float *currentLong);

float Get_Distance(float startLatitude, float startLongitude, float endLatitude, float endLongitude);

float ToDegree (float angle);

float ToRadian(float degrees);


#endif	
