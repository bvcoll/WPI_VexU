// Drive.c
//
// Brandon Coll
//
// Code for basic robot drive functions.

pos_PID drivePID; //Make a pid controller for the arm
float avgEncoders;
int drivePIDOutput;


int removeDeadband(int input) {
	if (abs(input) > 20) {
		return input;
	}
	else {
		return 0;
	}
}

void resetEncoders() {
	SensorValue(leftEncoder) = 0;
	SensorValue(rightEncoder) = 0;
}

float getLeftEncoder() {
	return ((float)SensorValue(leftEncoder) / 360.0) * 3.25 * PI;
}

float getRightEncoder() {
	return ((float)SensorValue(rightEncoder) / 360.0) * 3.25 * PI;
}

float getAvgEncoder(){
	return (getRightEncoder() + getLeftEncoder()) /2 ;
}

//Drives the left drive at the given voltage.
void leftDrive(int voltage){
	motor(LD1) = motor(LD2) = motor(LD3) = removeDeadband(voltage);
}

//Drives the right drive at the given voltage.
void rightDrive(int voltage){
	motor(RD1)  = motor(RD2) = motor(RD3) = removeDeadband(voltage);
}

//ARCADE DRIVE SYSTEM
void arcadeDrive(){
	leftDrive(vexRT(Ch3) + vexRT(Ch1));
	rightDrive(vexRT(Ch3) - vexRT(Ch1));
}

void autoDrive(int voltage) {
	motor(LD1) = motor(LD2) = motor(LD3) = motor(RD1)  = motor(RD2) = motor(RD3) = voltage;
}

void driveDistance(int distance) {
	//Initialize our pid controller with sensor myQuad and gains
	pos_PID_InitController(&drivePID, leftEncoder , kP_drive, kI_drive, kD_drive);

	//Set the target position for our pid controller
	pos_PID_SetTargetPosition(&drivePID, distance);


	avgEncoders = getAvgEncoder();

	//Run our motor with the output of the pid controller
	drivePIDOutput = pos_PID_StepController(&drivePID , avgEncoders);

	int direction = drivePIDOutput/abs(drivePIDOutput);

	//semi janky fix to stop the robot from drifting (why u no have break mode vex)
	if(abs(drivePIDOutput) < 15) {
		autoDrive(10 * -direction);
	}
	else {
		autoDrive(drivePIDOutput);

	}
}
