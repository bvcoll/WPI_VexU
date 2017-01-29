// ArmUserControl.c
//
// Brandon Coll
//
// Code for robot drive user control.

//ARCADE DRIVE SYSTEM
void arcadeDrive(){
	leftDrive(vexRT(Ch3) + vexRT(Ch1));
	rightDrive(vexRT(Ch3) - vexRT(Ch1));
}

//Returns the true speed of the robot based on imput.
char trueSpeed(int power) {
	char tsArray[128] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 21, 21, 22, 22, 22, 23, 24, 24,
		25, 25, 25, 25, 26, 27, 27, 28, 28, 28,
		28, 29, 30, 30, 30, 31, 31, 32, 32, 32,
		33, 33, 34, 34, 35, 35, 35, 36, 36, 37,
		37, 37, 37, 38, 38, 39, 39, 39, 40, 40,
		41, 41, 42, 42, 43, 44, 44, 45, 45, 46,
		46, 47, 47, 48, 48, 49, 50, 50, 51, 52,
		52, 53, 54, 55, 56, 57, 57, 58, 59, 60,
		61, 62, 63, 64, 65, 66, 67, 67, 68, 70,
		71, 72, 72, 73, 74, 76, 77, 78, 79, 79,
		80, 81, 83, 84, 84, 86, 86, 87, 87, 88,
		88, 89, 89, 90, 90,127,127,127
	};
	return ((power>0)?1:-1)*tsArray[power*((power>0)?1:-1)];
}

//Tank Control Based on Real Imput
void tankDrive(){
	rightDrive(trueSpeed(vexRT[Ch2]));
	leftDrive(trueSpeed(vexRT[Ch3]));
}
