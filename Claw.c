// Claw.c
//
// Brandon Coll
//
// Code for pneumatic claw control.

bool isClawClosed;

//Opens the claw.
void openClaw(){
	if(isClawClosed){
		SensorValue(clawSolenoid) = 1;
		isClawClosed = false;
	}
}

//Closes the claw.
void closeClaw(){
	if(!isClawClosed){
		SensorValue(clawSolenoid) = 0;
		isClawClosed = true;
	}
}

//User control program for the claw.
void userClaw(){
	if(vexRT(Btn8D)){
		closeClaw();
	}
	if(vexRT(Btn8U)){
		openClaw();
	}
}
