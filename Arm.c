// Arm.c
//
// Brandon Coll
//
// Code for basic robot arm control.

//ARM PID CONSTANTS
float kP_arm = 0.3, kI_arm = 0, kD_arm = 0;

pos_PID armPID; //Make a pid controller for the arm

//ARM SETPOINTS
#define armRestingSpeed 0 //OLD -15
#define armDownSpeed -127
#define armManualDownSpeed -75
#define armUpSpeed 127

void initArmPID(){
pos_PID_InitController(&armPID, A4, kP_arm, kI_arm, kD_arm); //Initialize our Arm PID controller with sensor and gains

}
//Moves the arm at the given voltage.
void setArm(int voltage){
	motor(A1) = motor(A2) = motor(A3) = motor(A4) = voltage;
	//motor(A4) = voltage;
}

void pidArm(int armSetpoint) {

	//Set the target position for our pid controller
	pos_PID_SetTargetPosition(&armPID, armSetpoint);

	//Run our motor with the output of the pid controller
	setArm(pos_PID_StepController(&armPID , SensorValue(armPot)));
}


bool holding = true;
//ARM CONTROL SYSTEM
void userArm(){
	if(vexRT(Btn5U)){ //TODO FIX
		setArm(armUpSpeed);
		holding = false;
	}
	else if(vexRT(Btn5D)){
		setArm(armManualDownSpeed);
		holding = true;
	}
	else if(vexRT(Btn7D)){
		setArm(-127);
		holding = true;
	}
	else {
			setArm(armRestingSpeed);
	}
}
