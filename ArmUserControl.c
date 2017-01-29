// ArmUserControl.c
//
// Brandon Coll
//
// Code for robot arm user control.


//ARM CONTROL SYSTEM
void userArm(){

	if(vexRT(Btn6D)){ //If arm button is pushed set the PID requested value to dump and to close the claw.
		armPIDToPoint(armLimit);
	}

	if(!armPIDRunning && SensorValue(armBump) == 0){ //If the arm is at the bottom hold it down and zero the encoder.
		setArm(-10); //Hold voltage applied when claw is at the base.
		SensorValue(I2C_1) = 0; //Sets the encoder on the arm to 0.
	}

	else if(!armPIDRunning){ //If the arm is moving down give it the minimum voltage.
		setArm(MIN_VOLTAGE); //Downward voltage.
	}

	else if(SensorValue(I2C_1) > armLimit){ //If the arm has reached its target disable PID.
		armPIDRunning = false;
	}

	else if(SensorValue(I2C_1) > dumpHeight){ //If the arm is running, above the dumping value open the claw.
		 openClaw(); //Open claw
	}
}
