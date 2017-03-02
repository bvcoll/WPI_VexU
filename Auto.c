
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
	armTask_ArmState = ARM_HOLDING;
	driveDistance(-12);
	armTask_ArmState = ARM_DUMPING;
	driveIntoWall(1500);
	driveDistance(34);
	armTask_ArmState = ARM_HOLDING;
	driveDistance(-12);
	armTask_ArmState = ARM_DUMPING;
	driveIntoWall(1500);
	driveDistance(34);
	armTask_ArmState = ARM_HOLDING;
	driveDistance(-12);
	armTask_ArmState = ARM_DUMPING;
	driveIntoWall(1500);
	driveDistance(30);
	turnAngle(-90);
	driveIntoWall(1500);
	driveDistance(96);
	wait1Msec(1000000);
}
