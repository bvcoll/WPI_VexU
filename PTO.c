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

int ptoEngaged = false;
//Closes the claw.
void engagePTO(){
	if(ptoEngaged){
		SensorValue(ptoSolenoid) = 0;
		ptoEngaged = false;
	}
	else if(!ptoEngaged){
		SensorValue(ptoSolenoid) = 1;
		ptoEngaged = true;
	}
}

//User control program for the claw.
void lockPTO(){
	if(ptoEngaged){
		SensorValue(ptoSolenoid) = 0;
		ptoEngaged = false;
	}
}
