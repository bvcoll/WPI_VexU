// Arm.c
//
// Brandon Coll
//
// Code for basic robot arm control.


//Moves the arm at the given voltage.
void setArm(int voltage){
	motor(A1) = motor(A2) = motor(A3) = motor(A4) = motor(A5) = motor(A6) = voltage;
}
