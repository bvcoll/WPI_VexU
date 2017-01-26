// ArmPID.c
//
// Brandon Coll
//
// Code for robot arm PID control.


/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*  								          Arm PID Control Task                             */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

//These could be constants but leaving as variables allows them to be modified in the debugger "live"
float  arm_pid_Kp = 2.0;
float  arm_pid_Kd = 1.4;

//Define if PID is running. Set in other functions.
static bool armPIDRunning = true;

//Define the value requested to PID move to.
float armPIDRequestedValue;

task armPIDController(){
	//Initialize variables.
	float  pidSensorCurrentValue;

	float  pidError;
	float  pidLastError;

	float  pidDerivative;
	float  pidDrive;

	//Set the first error to 0.
	pidLastError  = 0;

	while(true){
		//Read the sensor value and scale.
		pidSensorCurrentValue = SensorValue[armEncoder];

		//Calculate error.
		pidError = (armPIDRequestedValue - pidSensorCurrentValue);

		//Calculate the derivative.
		pidDerivative = pidError - pidLastError;
		pidLastError  = pidError;

		//If PID is running calculate the drive and set the motor.
		if(armPIDRunning){
			// calculate drive
			pidDrive = (arm_pid_Kp * pidError) + (arm_pid_Kd * pidDerivative);

			// limit drive
			if(pidDrive > MAX_VOLTAGE)
				pidDrive = MAX_VOLTAGE;
			if(pidDrive < MIN_VOLTAGE)
				pidDrive = MIN_VOLTAGE;

			// send to motor
			setArm(pidDrive);
		}

		// Run task at 50Hz
		wait1Msec(25);
	}
}

//Uses PID tasks to move the arm to a given point.
void armPIDToPoint(float point){
	armPIDRequestedValue = point;
	armPIDRunning = true;
}
