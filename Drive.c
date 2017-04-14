// Drive.c
//
// Brandon Coll
//
// Code for basic robot drive functions.

pos_PID drivePID; //Make a pid controller for the arm
pos_PID turnPID; //Make a pid controller for the arm
float avgEncoders;
int drivePIDOutput;
bool driveDone = false;
bool turnDone = false;


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

float getGyro(){
	return (SensorValue(leftEncoder) - SensorValue(rightEncoder)) / 8.92;
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

int leftValue, rightValue;
//ARCADE DRIVE SYSTEM
void arcadeDrive(){
	if(vexRT(Btn5D)){
		leftDrive(vexRT(Ch3) + vexRT(Ch1)/2);
		rightDrive(vexRT(Ch3) - vexRT(Ch1)/2);
	}
	else{
		leftDrive(vexRT(Ch3) + vexRT(Ch1));
		rightDrive(vexRT(Ch3) - vexRT(Ch1));
	}
}

//TANK DRIVE SYSTEM
void tankDrive(){
	leftDrive(vexRT(Ch3));
	rightDrive(vexRT(Ch2));
}

void autoDrive(int voltage) {
	int turn = 0;
	if(abs(voltage) > 15){
		turn = getGyro() * 4;
	}
	motor(LD1) = motor(LD2) = motor(LD3) = voltage - turn;
	motor(RD1)  = motor(RD2) = motor(RD3) = voltage + turn;
}

void autoTurn(int voltage) {
	motor(LD1) = motor(LD2) = motor(LD3) = voltage;
	motor(RD1)  = motor(RD2) = motor(RD3) = -voltage;
}

void driveDistance(int distance) {
	driveDone = false;
	resetEncoders();

	//Initialize our pid controller with sensor myQuad and gains
	pos_PID_InitController(&drivePID, leftEncoder , kP_drive, kI_drive, kD_drive);
	//Set the target position for our pid controller
	pos_PID_SetTargetPosition(&drivePID, distance);

	while(!driveDone){

		avgEncoders = getAvgEncoder();

		//Run our motor with the output of the pid controller
		drivePIDOutput = pos_PID_StepController(&drivePID , avgEncoders);

		int direction = drivePIDOutput/abs(drivePIDOutput);

		//semi janky fix to stop the robot from drifting (why u no have break mode vex)
		if(abs(drivePIDOutput) < 20) {
			autoDrive(10 * -direction);
			wait1Msec(100);
			driveDone = true;
		}
		else {
			autoDrive(drivePIDOutput);

		}
		wait1Msec(15);
	}
}

float gyro, turnPIDOutput;
int direction;
void turnAngle(int angle) {
	turnDone = false;
	resetEncoders();

	//Initialize our pid controller with sensor myQuad and gains
	pos_PID_InitController(&turnPID, leftEncoder , kP_turn, kI_turn, kD_turn);
	//Set the target position for our pid controller
	pos_PID_SetTargetPosition(&turnPID, angle);

	while(!turnDone){
		gyro = getGyro();

		//Run our motor with the output of the pid controller
		turnPIDOutput = pos_PID_StepController(&turnPID , gyro);

		//int direction = turnPIDOutput/abs(turnPIDOutput);
		if(turnPIDOutput > 0){
			direction = 1;
			} else {
			direction = -1;
		}

		//semi janky fix to stop the robot from drifting (why u no have break mode vex)
		if(abs(turnPIDOutput) < 25) { //15
			autoTurn(15 * -direction);
			wait1Msec(100);
			turnDone = true;
		}
		else {
			autoTurn(turnPIDOutput);

		}
		wait1Msec(15);
	}
}

void driveIntoWall(int time, int power=-65){
	resetEncoders();
	autoDrive(power);
	while(time>0){
		autoDrive(power);
		time-=20;
		wait1Msec(20);
	}
	//wait1Msec(time);
	autoDrive(0);
}
