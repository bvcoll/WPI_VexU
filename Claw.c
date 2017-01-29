// Claw.c
//
// Brandon Coll
//
// Code for pneumatic claw control.

bool isClawClosed;

//Opens the claw.
void openClaw(){
	if(isClawClosed){
		SensorValue(clawSolenoid) = 0;
		isClawClosed = false;
	}
}

//Closes the claw.
void closeClaw(){
	if(!isClawClosed){
		SensorValue(clawSolenoid) = 1;
		isClawClosed = true;
	}
}

//User control program for the claw.
void UserClaw(){
	if(vexRT(Btn6U)){
		closeClaw();
	}
	if(vexRT(Btn5U)){
		openClaw();
	}
}
