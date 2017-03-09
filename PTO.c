// PTO.c
//
// Oliver Jay
//
// Code for pneumatic PTO control.

bool hookReleased = false;
//Opens the claw.
void releaseHook(){
	if(!hookReleased) {
		SensorValue(hookSolenoid) = 1;
		hookReleased = true;
	}
}

void resetHook(){
	if(hookReleased){
		SensorValue(hookSolenoid) = 0;
		hookReleased = false;
	}
}

//Closes the claw.
void engagePTO(){
	SensorValue(ptoSolenoid) = 1;
}

//User control program for the claw.
void lockPTO(){
	SensorValue(ptoSolenoid) = 0;
}
