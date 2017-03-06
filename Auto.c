
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
	//openClaw();
	armTask_ArmState = ARM_DEPLOY;
	wait1Msec(1500);
	driveDistance(10);
	armTask_ArmState = ARM_HOLDING;
	wait1Msec(200);
	driveDistance(-12);
	armTask_ArmState = ARM_DUMPING;
	driveIntoWall(1500,-100);
	driveDistance(36); // old 34
	armTask_ArmState = ARM_HOLDING;
	driveDistance(-12);
	armTask_ArmState = ARM_DUMPING;
	driveIntoWall(1500,-100);
	driveDistance(34);
	armTask_ArmState = ARM_HOLDING;
	driveDistance(-12);
	armTask_ArmState = ARM_DUMPING;
	driveIntoWall(1500,-100);
	// drive then turn
	driveDistance(26); // old 30
	turnAngle(-90);
	driveIntoWall(1800);
	//drive to get cube
	driveDistance(55);
	armTask_ArmState = ARM_HIGH_HOLDING;
	driveDistance(32); // old 36
	turnAngle(90);
	//back into fense
	driveDistance(-12);
	armTask_ArmState = ARM_DUMPING;
	driveIntoWall(1800,-100);
	wait1Msec(250);
	// drive forward to grab last cube
	driveDistance(36);
	armTask_ArmState = ARM_HOLDING;
	driveDistance(-12);
	armTask_ArmState = ARM_DUMPING;
	driveIntoWall(1500, -100);

	//wait1Msec(1000000);
}

void auto_climb(){
	driveIntoWall(1500);
	driveDistance(24);
	wait1Msec(1000);
	turnAngle(90);
	wait1Msec(1000);
	driveIntoWall(1200);
	driveDistance(24);
	wait1Msec(1000);
	turnAngle(-53);
	wait1Msec(2000);
	driveIntoWall(2500);
	wait1Msec(1000);
	engagePTO();
	driveIntoWall(4500, -127);
	lockPTO();
}

void clearUnderBar(){
	driveDistance(19);
	turnAngle(90);
	driveIntoWall(1500);
	driveDistance(9);
	turnAngle(-90);
	driveDistance(14);
	armTask_ArmState = ARM_HOLDING;
	driveDistance(-12);
	armTask_ArmState = ARM_DUMPING;
	driveIntoWall(1500,-100);
	driveDistance(30);
	turnAngle(90);
}
