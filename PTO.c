// PTO.c
//
// Oliver Jay
//
// Code for pneumatic PTO control.

bool hookReleased = false;
//Opens the claw.
void releaseHook(){
	hookReleased = 1-hookReleased;
	SensorValue(hookSolenoid) = hookReleased;
}

//Closes the claw.
void engagePTO(){
	SensorValue(ptoSolenoid) = 1;
}

//User control program for the claw.
void lockPTO(){
	SensorValue(ptoSolenoid) = 0;
}
