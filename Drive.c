// Drive.c
//
// Brandon Coll
//
// Code for basic robot drive functions.

#define     JOY_DRIVE_V     Ch3
#define     JOY_DRIVE_H     Ch1
#define     JOYSTICK_THRESHOLD   15

float avgEncoders;
int drivePIDOutput;
bool driveDone = false;
bool turnDone = false;

void initDriveSlew(){
	addMotor(LD1);
	addMotor(LD2);
	addMotor(LD3);
	addMotor(RD1);
	addMotor(RD2);
	addMotor(RD3);
}


/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

void
DriveLeftMotor( int value )
{
	setMotorSpeed(LD1,value);
	setMotorSpeed(LD2,value);
	setMotorSpeed(LD3,value);
}

/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

void
DriveRightMotor( int value )
{
	setMotorSpeed(RD1,value);
	setMotorSpeed(RD2,value);
	setMotorSpeed(RD3,value);
}

/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
int    ctl_v;
int    ctl_h;
int    drive_l;
int    drive_r;

void ArcadeDrive()
{
	// Get joystick H and V control
	ctl_v = vexRT[ JOY_DRIVE_V ];

	if(vexRT[Btn5U]){
		ctl_h = vexRT[ JOY_DRIVE_H ]/2;
		} else {
		ctl_h = vexRT[ JOY_DRIVE_H ];
	}

	// Ignore joystick near center
	if( (abs(ctl_v) <= JOYSTICK_THRESHOLD) && (abs(ctl_h) <= JOYSTICK_THRESHOLD) )
	{
		drive_l = 0;
		drive_r = 0;
	}
	else
	{
		// Create drive for left and right motors
		drive_l = (ctl_v + ctl_h);
		drive_r = (ctl_v - ctl_h);
	}

	// Now send out to motors
	DriveLeftMotor( drive_l );
	DriveRightMotor( drive_r );


}


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
void basicArcadeDrive(){
	if(vexRT(Btn5U)){
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
int lastLatched_Drive,lastLatched_Arm,driveMotorPower,lastDrive,lastArm = 0;
bool armStalled,driveStalled = false;

task autoStallDetection(){
	disterror_Stall= distderivative_Stall= prevdisterror_Stall = lastDrive = lastArm =0;
	lastLatched_Arm = lastLatched_Drive = nPgmTime;
	armStalled = driveStalled = false;

	while(true){
		//Calculate if we can't move
		disterror_Stall = getAvgEncoder(); //Get encoder value
		distderivative_Stall = disterror_Stall - prevdisterror_Stall; //Calculate distance derivative
		prevdisterror_Stall = disterror_Stall; //Update previous distance error


		//When derivative error is less than certain value begin latching
		//If latched for more than certain time set hitWall to true
		if(abs(distderivative_Stall) < 0.2 && abs(motor(LD1)) > 50 && lastDrive - 5 < motor(LD1) && lastDrive + 5 > motor(LD1)){
			driveStalled =  nPgmTime - lastLatched_Drive > 5000;
			} else {
			lastLatched_Drive = nPgmTime;
			driveStalled = false;
		}

		if( abs(motor(A1)) > 50 && lastArm -5 < motor(A1) && lastArm + 5 > motor(A1)){
			armStalled =  nPgmTime - lastLatched_Arm > 6000;
			} else {
			lastLatched_Arm = nPgmTime;
			armStalled = false;
		}

		if(driveStalled || armStalled){
			isDriving = isTurning = isWall = false;
			stopTask(PID_Drive);
			delay(100);
			motor[LD1] = motor[LD2] = motor[LD3] = 0;
			motor[RD1] = motor[RD2] = motor[RD3] = 0;
			stopTask(ArmClawController);
			setArm(0);
			//delay(10000000);
		}

		lastDrive = motor(LD1);
		lastArm = motor(A1);

		wait1Msec(20);
	}

}
