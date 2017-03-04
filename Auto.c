
void auto_move(){
		//armTask_ArmState = ARM_DUMPING;
		//driveDistance(-24);
		//driveIntoWall(1000);
		driveDistance(30);
		turnAngle(90);
		driveIntoWall(1200);
		driveDistance(96);
		wait1Msec(1000000);
}

void auto_score(){
	driveDistance(-24);
	armTask_ArmState = ARM_DEPLOY;
	wait1Msec(1500);
	driveDistance(10);
	armTask_ArmState = ARM_HOLDING;
	wait1Msec(200);
	driveDistance(-12);
	armTask_ArmState = ARM_DUMPING;
	driveIntoWall(1500);
	driveDistance(36); // old 34
	armTask_ArmState = ARM_HOLDING;
	driveDistance(-12);
	armTask_ArmState = ARM_DUMPING;
	driveIntoWall(1500);
	driveDistance(34);
	armTask_ArmState = ARM_HOLDING;
	driveDistance(-12);
	armTask_ArmState = ARM_DUMPING;
	driveIntoWall(1500);
	// drive then turn
	driveDistance(26); // old 30
	turnAngle(-90);
	driveIntoWall(1500);
	//drive to get cube
	driveDistance(55);
	armTask_ArmState = ARM_HIGH_HOLDING;
	driveDistance(34); // old 36
	turnAngle(90);
	//back into fense
	driveDistance(-12);
	armTask_ArmState = ARM_DUMPING;
	driveIntoWall(1500);
	wait1Msec(250);
	// drive forward to grab last cube
	driveDistance(34);
	armTask_ArmState = ARM_HOLDING;
	driveDistance(-12);
	armTask_ArmState = ARM_DUMPING;
	driveIntoWall(1500);

	wait1Msec(1000000);
}
