// Drive.c
//
// Brandon Coll
//
// Code for basic robot drive functions.

//Drives the left drive at the given voltage.
void leftDrive(int voltage){
	motor(LD1) = motor(LD2) = motor(LD3) = voltage;
}

//Drives the right drive at the given voltage.
void rightDrive(int voltage){
	motor(RD1)  = motor(RD2) = motor(RD3) = voltage;
}
