// Drive.c
//
// Brandon Coll
//
// Code for basic robot drive functions.

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


float disterror_Stall, distderivative_Stall, prevdisterror_Stall =0;
int lastLatched_Stall = 0;
bool stalled = false;

task autoStallDetection(){
	disterror_Stall= distderivative_Stall= prevdisterror_Stall =0;
	lastLatched_Stall = 0;
	stalled = false;

	while(true){
		//Calculate if we can't move
		disterror_Stall = getAvgEncoder(); //Get encoder value
		distderivative_Stall = disterror_Stall - prevdisterror_Stall; //Calculate distance derivative
		prevdisterror_Stall = disterror_Stall; //Update previous distance error

		//When derivative error is less than certain value begin latching
		//If latched for more than certain time set hitWall to true
		if(abs(distderivative_Stall) > 0.2){
			lastLatched_Stall = nPgmTime;
			stalled = false;
			} else {
			stalled =  nPgmTime - lastLatched_Stall > 5000;
		}
		if(stalled){
			isDriving = isTurning = isWall = false;
			stopTask(PID_Drive);
			delay(250);
			motor[LD1] = motor[LD2] = motor[LD3] = 0;
			motor[RD1] = motor[RD2] = motor[RD3] = 0;
			stopTask(ArmClawController);
			setArm(0);
			delay(10000000);
		}


			wait1Msec(20);
		}

	}
