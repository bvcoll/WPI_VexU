
#pragma config(Sensor, in1,    armPot,         sensorPotentiometer)
#pragma config(Sensor, dgtl1,  rightEncoder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  leftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  armBottomLimit, sensorTouch)
#pragma config(Sensor, dgtl6,  armTopLimit,    sensorTouch)
#pragma config(Sensor, dgtl9,  hookSolenoid,   sensorDigitalOut)
#pragma config(Sensor, dgtl11, ptoSolenoid,    sensorDigitalOut)
#pragma config(Sensor, dgtl12, clawSolenoid,   sensorDigitalOut)
#pragma config(Motor,  port1,           RD1,           tmotorVex393TurboSpeed_HBridge, openLoop, reversed, driveRight)
#pragma config(Motor,  port2,           RD2,           tmotorVex393TurboSpeed_MC29, openLoop, driveRight)
#pragma config(Motor,  port3,           A1,            tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port4,           RD3,           tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           A3,            tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           A2,            tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port7,           LD3,           tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           A4,            tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           LD2,           tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          LD1,           tmotorVex393TurboSpeed_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//#pragma config(Sensor, I2C_1,  					,              sensorQuadEncoderOnI2CPort,    , AutoAssign )
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        Description: Competition template for VEX EDR                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)
#include "Vex_Competition_Includes.c" //Main competition background code...do not modify!

/* DEFINE CONSTANTS */
#define MAX_VOLTAGE 127 //Maximum voltage to be applied to a motor.
#define MIN_VOLTAGE (-127) //Minimum voltage to be applied to a motor.
#define inToMm 25.4 //Conversion factor for autonomous PID movement.

/* GLOBAL VARIABLES */
//Whether or not to end pre auton
bool endPreAuton = false;

//The autonomous program to run
int autonSelection = -1;

int middleArmSetpoint = 1000; //Setpoint to hold at for driving around the field
int highHoldingArmSetpoint = 1325;
int topArmSetpoint = 3000; //3000 //Setpoint for dumping
int scoreThreshold = topArmSetpoint - 1250; //minus 1500 //Point at which to open the claw

//ARM PID CONSTANTS
float kP_arm = 0.3, kI_arm = 0, kD_arm = 0;
//Drive PID CONSTANTS
float kP_drive = 4.5, kI_drive = 0, kD_drive = 0;
//Drive PID CONSTANTS
float kP_turn = 1.35, kI_turn = 0, kD_turn = 0.2;

//LCD Chooser Defines
#define LCD_SAFETY_REQ_COMP_SWITCH //prevents waiting for LCD imput blocking driver control when comp switch is plugged in
#define MENU_NUM 6 //The number of allocated menus

//Include BCI
#define BCI_USE_POS_PID //Position domain PID control
#define BCI_USE_ODOMETRY //Odometry tracking
#define BCI_USE_LCDCONTROL //LCD menu system
#include "BCI/BCI.h"

//INCLUDES
#include "Claw.c" //All control code for the claw.
#include "PTO.c" //All control for hook release and pto.
#include "Drive.c" //Basic drive functions.
#include "SlewDrive.c" //Slew drive functions.
#include "Arm.c" //Basic arm functions.
#include "ArmUserControl.c" //User control code for the arm.
#include "ArmClawController.c"
#include "Auto.c"
#include "LCDAutonomousSelect.c"
#include "PID_Drive.c"



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
	bStopTasksBetweenModes = false;
	resetEncoders();
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
	//Call startAutonomous() here

	startTask(ArmClawController);
	//programmingSkills();
	//True for left, false for right
	auton(false);
	wait1Msec(100000);
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

float leftEncoderValue, rightEncoderValue, pot;
task usercontrol()
{
	// Start motor slew rate control
  StartTask( MotorSlewRateTask );

  // Start driver control tasks
  StartTask( ArcadeDrive );

	startTask(ArmClawController);

	while (true)
	{

		leftEncoderValue = getLeftEncoder();
		rightEncoderValue = getRightEncoder();

		/*User drive method*/
		//arcadeDrive();
		//userClaw();
		//userArm();
		//tankDrive();

		if(vexRT(Btn8R)){
			//programmingSkills();

			} else {
			//arcadeDrive();
			//tankDrive();
		}

		if(vexRT(Btn8L)){
			resetEncoders();
		}

		gyro = getGyro();
		pot = SensorValue(armPot);


		/*User state changes*/
		if (vexRT(Btn7R)) {
			armTask_ArmState = ARM_HIGH_HOLDING;
		}
		if (vexRT(Btn7D) || vexRT(Btn5U)) {
			armTask_ArmState = ARM_USER;
		}
		if(vexRT(Btn6D)) {
			armTask_ArmState = ARM_HOLDING;
		}
		if (vexRT(Btn6U)) {
			armTask_ArmState = ARM_DUMPING;
		}
		wait1Msec(15);
	}
}

//Run an autonomous function based on current selection
void startAutonomous()
{

}
