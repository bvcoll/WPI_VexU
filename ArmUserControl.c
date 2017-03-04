// ArmUserControl.c
//
// Brandon Coll
//
// Code for robot arm user control.

#define armRestingSpeed -5
#define armDownSpeed -75
#define armUpSpeed 127

bool holding = true;
//ARM CONTROL SYSTEM
void userArm(){
	if(vexRT(Btn5U)){
		setArm(armUpSpeed);
		holding = false;
	}
	else if(vexRT(Btn5D)){
		setArm(armDownSpeed);
		holding = true;
	}
	else {
		if(holding) {
			setArm(armRestingSpeed);
			} else {
			setArm(0);
		}
	}
}
