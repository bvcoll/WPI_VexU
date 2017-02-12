// Arm.c
//
// Brandon Coll
//
// Code for basic robot arm control.

pos_PID armPID; //Make a pid controller for the arm


//ARM PID CONSTANTS
float kP_arm = 0.625, kI_arm = 0, kD_arm = 0;


//Moves the arm at the given voltage.
void setArm(int voltage){
	motor(A1) = motor(A2) = motor(A3) = motor(A4) = voltage;
}

void pidArm(int armSetpoint) {

	//Initialize our pid controller with sensor myQuad and gains
	pos_PID_InitController(&armPID, A4, kP_arm, kI_arm, kD_arm);

	//Set the target position for our pid controller
	pos_PID_SetTargetPosition(&armPID, armSetpoint);

	//Run our motor with the output of the pid controller
	setArm(pos_PID_StepController(&armPID , SensorValue(I2C_1)));
	//pidOutput = pos_PID_StepController(&armPID, SensorValue(I2C_1));
}
