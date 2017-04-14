// ArmUserControl.c
//
// Brandon Coll
//
// Code for robot arm user control.

#define armRestingSpeed -15
#define armDownSpeed -127
#define armManualDownSpeed -75
#define armUpSpeed 127

bool holding = true;
//ARM CONTROL SYSTEM
void userArm(){
	if(vexRT(Btn5U)){
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
