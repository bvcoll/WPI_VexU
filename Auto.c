
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

	//releaseHook();
	wait1Msec(2000);
	driveIntoWall(2900);
	armTask_ArmState = ARM_HOLDING;
	wait1Msec(1000);
	//engagePTO();
	driveIntoWall(6300, -127);
	//lockPTO();
	//resetHook();
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

void preloads(){
	driveDistance(-24);
	openClaw();
	wait1Msec(400);
	driveDistance(4);
	wait1Msec(500);
	dump();
	driveDistance(32);
	wait1Msec(500);
	dump();
	driveDistance(32);
	wait1Msec(500);
	dump();
}

void getCornerStars(bool isLeft){
	int dir = 2*isLeft - 1;
	driveDistance(32);
	turnAngle(135*dir);
	driveDistance(12);
	hover();
	turnAngle(-115*dir);
	driveDistance(12);
	hover();
	turnAngle(-20*dir);
	dump();
}


void programmingSkills(){
	//preloads();
	//driveDistance(26); // old 30
	//turnAngle(-90);
	//driveIntoWall(1000);

	/*
  //drive to get cube
	driveDistance(42);
	armTask_ArmState = ARM_HIGH_HOLDING;
	driveDistance(48); // old 36
	turnAngle(90);
	dump(true);
	driveWall(1000,100);
	dump();
	*/

	//Cross field to get starts by fense
	driveDistance(15);
	turnAngle(88);
	driveDistance(-10);
	driveDistance(40);
	closeClaw();
	delay(250);
	driveDistance(24);
	openClaw();
	turnAngle(-15);
	driveWall(500,100);

	//Pick up starts and dump
	closeClaw();
	delay(750);
	driveDistance(-12);
	armTask_ArmState = ARM_HIGH_HOLDING;
	delay(500);
	driveDistance(14);
	turnAngle(-90);
	driveWall(100);
	armTask_ArmState = ARM_DUMPING;
	delay(1500);


	//Drive forward and pick up corner star
	driveWall(1500,100);
	turnAngle(-45);
	driveDistance(10);
	turnAngle(-45);
	driveDistance(72);
	driveWall(500,100);
	closeClaw();
}

void oldBS(){


	//Cross field to get starts by fense
	driveDistance(15);
	turnAngle(88);
	driveDistance(-10);
	driveDistance(40);
	closeClaw();
	delay(250);
	driveDistance(24);
	openClaw();
	turnAngle(-15);
	driveWall(500,100);

	//Pick up starts and dump
	closeClaw();
	delay(750);
	driveDistance(-12);
	armTask_ArmState = ARM_HIGH_HOLDING;
	delay(500);
	driveDistance(14);
	turnAngle(-90);
	driveWall(100);
	armTask_ArmState = ARM_DUMPING;
	delay(1500);


	//Drive forward and pick up corner star
	driveWall(1500,100);
	turnAngle(-45);
	driveDistance(10);
	turnAngle(-45);
	driveDistance(72);
	driveWall(500,100);
	closeClaw();

}
