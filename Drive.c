// Drive.c
//
// Brandon Coll
//
// Code for basic robot drive functions.

//Drives the left drive at the given voltage.
void leftDrive(int voltage){
	motor(L1) = motor(L2YL3) = voltage;
}

//Drives the right drive at the given voltage.
void rightDrive(int voltage){
	motor(R1) = motor(R2YR3) = voltage;
}
