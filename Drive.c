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

void driveIntoWall(int time, int power=-65){
	resetEncoders();
	motor(LD1) = motor(LD2) = motor(LD3) = power;
	motor(RD1)  = motor(RD2) = motor(RD3) = power;
	while(time>0){
		motor(LD1) = motor(LD2) = motor(LD3) = power;
		motor(RD1)  = motor(RD2) = motor(RD3) = power;
		time-=20;
		wait1Msec(20);
	}
	//wait1Msec(time);
	motor(LD1) = motor(LD2) = motor(LD3) = 0;
	motor(RD1)  = motor(RD2) = motor(RD3) = 0;
}
