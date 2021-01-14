#include "main.h"

#define LEFT_FRONT_MOTOR_PORT 12    // Drivetrain motors
#define LEFT_BACK_MOTOR_PORT 13
#define RIGHT_FRONT_MOTOR_PORT 18
#define RIGHT_BACK_MOTOR_PORT 19

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "1010S Replay");
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	FILE* forward_file = fopen("/usd/forward.txt", "r");
	FILE* yaw_file = fopen("/usd/yaw.txt", "r");

	// Init chassis controller and V5 controller
  std::shared_ptr<okapi::ChassisController> chassis = okapi::ChassisControllerBuilder()
    // Right motors reversed
    .withMotors(
      {LEFT_FRONT_MOTOR_PORT, LEFT_BACK_MOTOR_PORT},
      {-RIGHT_FRONT_MOTOR_PORT, -RIGHT_BACK_MOTOR_PORT}
    )
    // Green gears + 2.75" wheel ⌀, 6.5" wheelbase
    .withDimensions(okapi::AbstractMotor::gearset::green, {{3.5_in, 6.5_in}, okapi::imev5GreenTPR})
    // Enable odometry
    .withOdometry()
    .buildOdometry();

  // Set brake mode
  chassis->getModel()->setBrakeMode(okapi::AbstractMotor::brakeMode::hold);

	int i = 0;

	double forward_lines[12000];
	char forward_line[64];
  while (fgets(forward_line, sizeof(forward_line), forward_file)) {
			forward_lines[i] = atof(forward_line);
			i++;
	}

	i = 0;

	double yaw_lines[12000];
	char yaw_line[64];
  while (fgets(yaw_line, sizeof(yaw_line), yaw_file)) {
			yaw_lines[i] = atof(yaw_line);
			i++;
  }

  // Main loop
	for (int i = 0; i < sizeof(forward_lines); i++) {
		// ----------
    // Drive
    // ----------

		// Replay
		float y = forward_lines[i];
    float x = yaw_lines[i];

    double forward = y;
    double yaw = x / 1.5;


    chassis->getModel()->arcade(forward, yaw, 0.15);

    // ----------
    // Misc.
    // ----------

    pros::delay(17);  // Loop delay
	}
}
