
void auto_move(){
	//armTask_ArmState = ARM_DUMPING;
	//driveDistance(-24); //Hi
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
	driveDistance(56);
	armTask_ArmState = ARM_HIGH_HOLDING;
	driveDistance(36); // old 36
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
	//Start facing wall between fence and bar

	//Align to first wall
	driveIntoWall(1500);
	driveDistance(22);
	turnAngle(90);

	//Align to second wall
	driveIntoWall(1500);
	driveDistance(24);

	//Turn to bar
	turnAngle(-53);

	releaseHook();
	wait1Msec(2000);
	driveIntoWall(2900);
	armTask_ArmState = ARM_HOLDING;
	wait1Msec(1000);
	engagePTO();
	driveIntoWall(6300, -127);
	lockPTO();
	resetHook();
}

void clearUnderBar(){
	//Start against fence

	//Position to face star
	driveDistance(20);
	turnAngle(90);
	driveIntoWall(1500);
	driveDistance(8);
	turnAngle(-90);

	//Get star
	driveDistance(16);
	armTask_ArmState = ARM_HOLDING;
	driveDistance(-15);

	//Score star
	armTask_ArmState = ARM_DUMPING;
	driveIntoWall(1500,-100);

	//Line up for climb sequence
	driveDistance(30);
	turnAngle(90);
}

void programmingSkills(){
	auto_score();
	clearUnderBar();
	auto_climb();
}

void auton(bool isLeft){
	// 1 for red, -1 for blue
	int dir = 2 * isLeft - 1;

	//Back up to deploy
	driveDistance(-10);
	armTask_ArmState = ARM_DEPLOY;
	wait1Msec(1500);

	//Collect row
	driveDistance(80);

	//Grab and turn
	armTask_ArmState = ARM_HOLDING;
	wait1Msec(600);
	driveDistance(-8);
	turnAngle(-90*dir);

	//Back up
	driveDistance(24);
	turnAngle(-180*dir);
	armTask_ArmState = ARM_DUMPING;
	driveIntoWall(1500,-100);
	// drive then turn
	driveDistance(26); // old 30
	turnAngle(90*dir);
	driveIntoWall(1800);
	//drive to get cube
	driveDistance(56);
	armTask_ArmState = ARM_HOLDING;
	driveDistance(35); // old 36
	turnAngle(-90*dir);
	//back into fense
	driveDistance(-12);
	armTask_ArmState = ARM_DUMPING;
	driveIntoWall(1800,-100);
}
