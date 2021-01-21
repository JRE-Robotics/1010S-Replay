#include "main.h"
// #include "record_tasks.h"

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
	pros::lcd::set_text(1, "1010S Record");
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
	// Initialize controller
	pros::Controller controller(pros::E_CONTROLLER_MASTER);

	// Initialize motors
	// Note: all motors on right side
	pros::Motor left_back_wheel (LEFT_BACK_MOTOR_PORT);
	pros::Motor right_back_wheel (RIGHT_BACK_MOTOR_PORT, true);
	pros::Motor left_front_wheel (LEFT_FRONT_MOTOR_PORT);
	pros::Motor right_front_wheel (RIGHT_FRONT_MOTOR_PORT, true);

	left_back_wheel.set_brake_mode (pros::E_MOTOR_BRAKE_BRAKE);
	right_back_wheel.set_brake_mode (pros::E_MOTOR_BRAKE_BRAKE);
	left_front_wheel.set_brake_mode (pros::E_MOTOR_BRAKE_BRAKE);
	right_front_wheel.set_brake_mode (pros::E_MOTOR_BRAKE_BRAKE);

	// Start tasks
	// pros::Task motorTask(record_motor_task, (std::shared_ptr<okapi::ChassisController>*) chassis.get(), TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "motor task");

	// Main loop
	while (true) {
		// Run motors based on joystick input (arcade drive)
		int power = controller.get_analog(ANALOG_LEFT_Y);
		int turn = controller.get_analog(ANALOG_LEFT_X);
		int left = power + turn;
		int right = power - turn;

		left_front_wheel.move(left);
		left_back_wheel.move(left);

		right_front_wheel.move(right);
		right_back_wheel.move(right);

		pros::delay(10);  // Loop delay
	}
}
