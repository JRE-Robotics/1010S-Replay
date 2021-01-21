#include "record_tasks.h"
#include "pros/motors.h"

void record_motor_task(void* motor) {
  // Overwrite file
  char buf[16];
  snprintf(buf, sizeof(buf), "/usd/motor%d.txt", (int) motor_port);
  FILE* temp;
	temp = fopen("/usd/motor.txt", "w");
  fclose(temp);


  while (true) {
    // Open file
    char buf[16];
    snprintf(buf, sizeof(buf), "/usd/motor%d.txt", (int) motor_port);
    FILE* f = fopen(buf, "a");

    // Get motor values


    // Write
    fputs(valsStr.str().c_str(), f);
    fclose(f);

    pros::delay(10);  // prevent infinite loops
  }
}

void create_tasks() {



}
