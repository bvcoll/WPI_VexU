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
	ARM_DUMPING,
	ARM_RESET

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

		case ARM_DUMPING:

			//PID to top setpoint
			pidArm(topArmSetpoint);

			//open the claw once above the scoring threshold
			if (SensorValue(I2C_1) < scoreThreshold) {
				closeClaw();
			}
			else {
				openClaw();
			}

			//Change robot state to bottom once near top setpoint
			if (SensorValue(I2C_1) >= topArmSetpoint - 25) {
				armTask_ArmState = ARM_RESET;
			}

			break;

		case ARM_RESET:
			if (SensorValue(armBottomLimit) == 0) { //Move down until button hit
				setArm(armDownSpeed);
			}
			else {
				SensorValue(I2C_1) = 0;
				armTask_ArmState = ARM_USER;
			}
			break;

		}

		//Task wait
		wait1Msec(15);
	}
}
