void scoreCenterCube(int dir, bool comeBack = true){
	//drive to get cube
	driveDistance(26);
	turnAngle(90*dir);
	delay(250);
	driveWallTime(1500);

	driveDistance(46);  //OLD 44
	if(!goingBack){
		armTask_ArmState = ARM_HOLDING;
		if(comeBack){
			driveDistance(-38); //OLD -42
			} else {
			driveDistance(42);
		}
		turnAngle(-90*dir);
		//back into fence to dump
		delay(250);
		wallThenDumpTime(1500);
		} else {
		driveDistance(12);
		turnAngle(-90 * dir);
		wallThenDumpTime(1500);
	}
}

void pickUpFromMiddle(int dir, int angle = 20, int distance = 50){

	//drive and back into wall
	driveDistance(26);
	turnAngle(90*dir);
	delay(250);
	driveWallTime(1500);

	//drive to pick up objects
	driveDistance(12);
	turnAngle(angle*-dir);
	driveDistance(distance);

	//lift and back up
	armTask_ArmState = ARM_HOLDING;
	delay(250);
	driveDistance(-distance);
	turnAngle(-90*dir + dir*angle);

	//back into fence to dump
	wallThenDumpTime(1500);
}

void pickUpFromFront(int driveForward = 30){
	delay(500);
	driveDistance(driveForward);
	delay(100);
	armTask_ArmState = ARM_HOLDING;
	delay(250);
	wallThenDumpTime(1500);

}

void auton(bool isLeft, bool comeBack = false, bool cubeComeBack = true){
	// 1 for red, -1 for blue
	int dir = 2 * isLeft - 1;

	//Back up to deploy
	driveDistance(-14);
	openClaw();
	wait1Msec(1500);

	//Collect row
	driveDistance(90);

	//Grab and turn
	wait1Msec(1000);
	if(comeBack){
		armTask_ArmState = ARM_HIGH_HOLDING;
		turnAngle(4*dir);
		driveDistance(-86);
		turnAngle(90*dir);
		wallThenDump(3500,true);

		scoreCenterCube(-dir,cubeComeBack);

		if(!cubeComeBack){
			dir *= -1;
		}

		pickUpFromMiddle(-dir,auto_angle1,auto_distance1);


		//pickUpFromFront(auto_frontDistance);


		//pickUpFromMiddle(-dir,auto_angle2,auto_distance2);
		//pickUpFromMiddle(-dir,auto_angle3,auto_distance3);


		} else {
		armTask_ArmState = ARM_HIGH_HOLDING;
		driveDistance(-8);
		turnAngle(88*dir); //OLD 90

		//Back up and score
		driveDistance(-24);
		wallThenDump(3500,true);

		scoreCenterCube(dir,cubeComeBack);

		if(!cubeComeBack){
			dir *= -1;
		}

		pickUpFromMiddle(dir,auto_angle1,auto_distance1);


		//pickUpFromFront(auto_frontDistance);


		//pickUpFromMiddle(dir,auto_angle2,auto_distance2);
		//pickUpFromMiddle(dir,auto_angle3,auto_distance3);
	}


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


void programmingSkills(){
	preloads();
	driveDistance(26); // old 30
	turnAngle(-90);
	driveWall(1000);


	//drive to get cube
	driveDistance(42);
	armTask_ArmState = ARM_HIGH_HOLDING;
	driveDistance(48); // old 36
	turnAngle(90);
	dump(true);
	driveWall(1000,100);
	dump();


	//Cross field to get starts by fense
	driveDistance(15, true);
	turnAngle(88);
	driveDistance(-10);
	driveDistance(45);
	closeClaw();
	delay(250);
	armTask_ArmState = ARM_HOLDING;
	delay(500);
	driveDistance(40);
	turnAngle(-90);
	driveWall(100);
	armTask_ArmState = ARM_DUMPING;
	delay(1500);


	//Drive forward and pick up corner star
	driveDistance(30);
	turnAngle(-45);
	driveDistance(10);
	turnAngle(-45);
	driveDistance(72);
	turnAngle(-15);
	driveWall(500,100);
	closeClaw();

	//Pick up turn and dump
	driveDistance(-12);
	armTask_ArmState = ARM_HIGH_HOLDING;
	turnAngle(90);
	dump();
}
