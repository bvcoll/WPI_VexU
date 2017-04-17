// ArmClaw_Controller.c
//
// Brandon Coll
//
// Code for the conrol of the arm and claw

enum armState
{
	ARM_INIT,
	ARM_USER,
	ARM_HOLDING,
	ARM_HIGH_HOLDING,
	ARM_DUMPING,
	ARM_RESET,
	ARM_DEPLOY,
	ARM_FENCE_KNOCKING,
	ARM_CLIMBING_SETUP

};


armState armTask_ArmState = ARM_RESET;

task ArmClawController()
{

	while(true)
	{

		if (SensorValue(armTopLimit) == 1) {
			armTask_ArmState = ARM_RESET;
		}

		switch(armTask_ArmState) {

		case ARM_USER:

				userClaw();
				userArm();

			break;

		case ARM_HOLDING:
			pidArm(middleArmSetpoint);
			closeClaw();
			break;

		case ARM_HIGH_HOLDING:
			pidArm(highHoldingArmSetpoint);
			closeClaw();
			break;

		case ARM_DUMPING:

			//PID to top setpoint
			pidArm(topArmSetpoint);

			//open the claw once above the scoring threshold
			if (SensorValue(armPot) < scoreThreshold) {
				closeClaw();
			}
			else {
				openClaw();
			}

			//Change robot state to bottom once near top setpoint
			if (SensorValue(armPot) >= topArmSetpoint - 25) {
				armTask_ArmState = ARM_RESET;
			}

			break;

		case ARM_RESET:
			if (SensorValue(armBottomLimit) == 0) { //Move down until button hit
				setArm(armDownSpeed);
			}
			else {
				holding = true;
				armTask_ArmState = ARM_USER;
			}
			break;

		case ARM_DEPLOY:
			openClaw();
			setArm(75);
			delay(250);
			setArm(0);
			delay(250);
			armTask_ArmState = ARM_RESET;
			break;


		case ARM_FENCE_KNOCKING:
			openClaw();
			pidArm(centerFenceKnockingSetpoint);
			break;

		case ARM_CLIMBING_SETUP:
			pidArm(hangingSetpoint);
			break;

		}

		//Task wait
		wait1Msec(20);
	}
}

void hover(bool isHigh = 0){
	if(isHigh){
		armTask_ArmState = ARM_HIGH_HOLDING;
	}
	else{
		armTask_ArmState = ARM_HOLDING;
	}
}

void lowerArm(){
	openClaw();
	armTask_ArmState = ARM_RESET;
}
