// ArmUserControl.c
//
// Brandon Coll
//
// Code for robot arm user control.


//ARM CONTROL SYSTEM
void userArm(){
	if (vexRT(Btn6U)){ //Arm moving up
		setArm(127); //Full upward voltage.
	}
	else if(vexRT(Btn6D)){ //Arm moving up
		setArm(-75); //Partial upward voltage.
	}
	else{ //Arm moving down
		setArm(0); //Full downward voltage.
	}
}
