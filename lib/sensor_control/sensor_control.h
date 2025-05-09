#include <iostream>
#include <Wire.h>
#include "global_defines.h"

#define SENSOR 0


#if SENSOR
#include <VL53L1X.h>
extern VL53L1X sensor_distance;
#define D_MAX 130
#define D_MIN 80
#else
#include <VL53L0X.h>
extern VL53L0X sensor_distance;
#define D_MAX 170
#define D_MIN 120
#endif

void setup_sensor();
void read_distance(int &distance);
void controllo_distanza(int &distance, float &velocita);
