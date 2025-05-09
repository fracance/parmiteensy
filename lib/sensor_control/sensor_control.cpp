#include "sensor_control.h"
#include "motor_control.h"

// chiamare nel setup
void setup_sensor()
{
  //Wire1.setSDA(SDA_PIN);
  //Wire1.setSCL(SCL_PIN);
  //Wire1.begin();

  Wire1.setClock(400000); // use 400 kHz I2C

#if not SENSOR
  sensor_distance.setBus(&Wire1);
#endif
  sensor_distance.setTimeout(500);
  for (int i = 0; i<3 && !sensor_distance.init(); i++)
  {
    Serial.println("Failed to detect and initialize sensor!");
  }

  // Use long distance mode and allow up to 50000 us (50 ms) for a measurement.
  // You can change these settings to adjust the performance of the sensor, but
  // the minimum timing budget is 20 ms for short distance mode and 33 ms for
  // medium and long distance modes. See the VL53L1X datasheet for more
  // information on range and timing limits.
  #if SENSOR
  sensor_distance.setDistanceModesensor_distance(VL53L1X::Short);
  #endif

  sensor_distance.setMeasurementTimingBudget(33000);

  // Start continuous readings at a rate of one measurement every 50 ms (the
  // inter-measurement period). This period should be at least as long as the
  // timing budget.
  sensor_distance.startContinuous(50);
}

// chiamare nel loop
void read_distance(uint16_t &distance)
{
  #if SENSOR
  distance = sensor_distance.read(false);
  #else
  sensor_distance.readRangeNoBlocking(distance);
  #endif
}

void controllo_distanza(int &distance, float &velocita)
{
  if (distance >= D_MAX)
  {
    velocita = V_MAX;
  }
  else if (distance < D_MAX && distance >= D_MIN)
  {
    velocita = V_MIN + (V_MAX - V_MIN) * ((D_MAX - D_MIN) / D_MAX);
  }
  else if (distance < D_MIN)
  {
    stop_motors();
    fine_programma = true;
  }
}