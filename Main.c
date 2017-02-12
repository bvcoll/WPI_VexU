#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, dgtl1,  leftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  rightEncoder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  armBump,        sensorTouch)
#pragma config(Sensor, dgtl12, clawSolenoid,   sensorDigitalOut)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           RD1,           tmotorVex393TurboSpeed_HBridge, openLoop, driveRight)
#pragma config(Motor,  port2,           RD2,           tmotorVex393TurboSpeed_MC29, openLoop, driveRight)
#pragma config(Motor,  port3,           A1,            tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           A2,            tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           RD3,           tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           A3,            tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           LD1,           tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port8,           A4,            tmotorVex393_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port9,           LD2,           tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          LD3,           tmotorVex393TurboSpeed_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        Description: Competition template for VEX EDR                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/

/* DEFINE CONSTANTS */
#define MAX_VOLTAGE 127 //Maximum voltage to be applied to a motor.
#define MIN_VOLTAGE (-127) //Minimum voltage to be applied to a motor.
#define inToMm 25.4 //Conversion factor for autonomous PID movement.

/* GLOBAL VARIABLES */
int middleArmSetpoint = 100; //Setpoint to hold at for driving around the field
int scoreThreshold = 300;  //Point at which to open the claw
int topArmSetpoint = 700;  //Setpoint for dumping
bool armFlag = false;      //flag to see if the arm has already zeroed

/* USER CONTROL ROBOT STATES */
#define BOTTOM 0
#define HOLDING 1
#define DUMPING 2
int robotState = BOTTOM;




// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Include PID libraries
#define BCI_USE_POS_PID
#include "BCI.h"

//INCLUDES
#include "Vex_Competition_Includes.c" //Main competition background code...do not modify!
#include "Claw.c" //All control code for the claw.
#include "Drive.c" //Basic drive functions.
#include "Arm.c" //Basic arm functions.
#include "DrivePID.c" //Drive PID movement functions.
#include "DriveUserControl.c" //User control code for the drive.
#include "ArmUserControl.c" //User control code for the arm.



/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton()
{
	// Set bStopTasksBetweenModes to false if you want to keep user created tasks
	// running between Autonomous and Driver controlled modes. You will need to
	// manage all user created tasks if set to false.
	bStopTasksBetweenModes = true;

	//Set drive encoder values to zero.
	SensorValue(leftEncoder) = 0;
	SensorValue(rightEncoder) = 0;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task autonomous()
{
	// ..........................................................................
	// Insert user code here.
	// ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task usercontrol()
{
	while (true)
	{

		/*User drive method*/
		arcadeDrive();

		/*Arm encoder reset*/
		if(SensorValue(armBump) == 1 && !armFlag){ //Arm at bottom
			//Reset arm encoder and set flag to true so it won't reset again
			SensorValue(I2C_1) = 0;
			armFlag = true;
		}

		/*User state changes*/
		if (vexRT(Btn8R)) {
			robotState = BOTTOM;
		}
		if(vexRT(Btn6D)) {
			robotState = HOLDING;
		}
		if (vexRT(Btn6U)) {
			robotState = DUMPING;
		}

		/*Claw/arm state machine*/
		switch(robotState) {


		case BOTTOM:  /*Arm at bottom ----------------------------------------------------------*/

			//arm control
			if (vexRT(Btn5U)){  //Manual up
				setArm(127);
			}
			else if (vexRT(Btn5D)){ //Manual down and reset encoder
				setArm(-75);
				SensorValue(I2C_1) = 0;
			}
			else if (SensorValue(armBump) == 0) { //PID down until button hit
				pidArm(0);
			}
			else {  //If button is hit turn off arm motors
				setArm(0);
			}

			//Claw control
			userClaw();

			break;


		case HOLDING:  /*Arm holding above ground ----------------------------------------------*/
			pidArm(middleArmSetpoint);
			closeClaw();

			break;


		case DUMPING:  /*Arm Dumping ------------------------------------------------------------*/

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
				robotState = BOTTOM;
			}

			break;
		}




	}
}
