#pragma config(Sensor, in1,    powerExpander,  sensorAnalog)
#pragma config(Sensor, dgtl1,  leftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  rightEncoder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  armBottomLimit, sensorTouch)
#pragma config(Sensor, dgtl6,  armTopLimit,    sensorTouch)
#pragma config(Sensor, dgtl7,  armEncoder,     sensorQuadEncoder)
#pragma config(Sensor, dgtl9,  hookSolenoid,   sensorDigitalOut)
#pragma config(Sensor, dgtl11, ptoSolenoid,    sensorDigitalOut)
#pragma config(Sensor, dgtl12, clawSolenoid,   sensorDigitalOut)
#pragma config(Motor,  port1,           RD1,           tmotorVex393TurboSpeed_HBridge, openLoop, driveRight)
#pragma config(Motor,  port2,           RD2,           tmotorVex393TurboSpeed_MC29, openLoop, driveRight)
#pragma config(Motor,  port3,           A1,            tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           A2,            tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           RD3,           tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           A3,            tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           LD1,           tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port8,           A4,            tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           LD2,           tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          LD3,           tmotorVex393TurboSpeed_HBridge, openLoop, reversed)
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
// old 150
int middleArmSetpoint = 145; //Setpoint to hold at for driving around the field
int scoreThreshold = 250;  //Point at which to open the claw
int highHoldingArmSetpoint = 200;
int topArmSetpoint = 615;  //Setpoint for dumping
bool isAutonomous = false;

//Whether or not to end pre auton
bool endPreAuton = false;

//The autonomous program to run
int autonSelection = -1;

/* USER CONTROL ROBOT STATES */
#define BOTTOM 0
#define HOLDING 1
#define DUMPING 2
#define RESET 3

//ARM PID CONSTANTS
float kP_arm = 0.555, kI_arm = 0, kD_arm = 0;
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
#include "Arm.c" //Basic arm functions.
#include "ArmUserControl.c" //User control code for the arm.
#include "ArmClawController.c"
#include "Auto.c"
#include "LCDAutonomousSelect.c"
#include "PID_Drive.c"

//Menus
//Level 1 - General Info
menu *programmingSkillsMenu;
menu *autonomousSelectionMenu;
menu *endPreAutonMenu;
menu *batteryVoltageMenu;
menu *powerExpanderVoltageMenu;
menu *backupBatteryVoltageMenu;

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

	//Menu system
	//Level 1 - General Info
	autonomousSelectionMenu = lcd_newMenu("Select Auton", 2);
	programmingSkillsMenu = lcd_newMenu("Prog Skills", 3);
	endPreAutonMenu = lcd_newMenu("Confirm", 1);

	string batteryVoltage;
	sprintf(batteryVoltage, "Main: %1.2f%c", nAvgBatteryLevel / 1000.0, 'V');
	batteryVoltageMenu = lcd_newMenu(batteryVoltage);

	string powerExpanderVoltage;
	sprintf(powerExpanderVoltage, "Expander: %1.2f%c", SensorValue[powerExpander] / ANALOG_IN_TO_V, 'V');
	powerExpanderVoltageMenu = lcd_newMenu(powerExpanderVoltage);

	string backupBatteryVoltage;
	sprintf(backupBatteryVoltage, "Backup: %1.2f%c", BackupBatteryLevel / 1000.0, 'V');
	backupBatteryVoltageMenu = lcd_newMenu(backupBatteryVoltage);

	lcd_linkMenus(programmingSkillsMenu, autonomousSelectionMenu, endPreAutonMenu, batteryVoltageMenu, powerExpanderVoltageMenu, backupBatteryVoltageMenu);

	bLCDBacklight = true;
	startTask(lcdControlTask);
	while (!lcd_getLCDSafetyState() && !endPreAuton) { wait1Msec(50); }
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
	programmingSkills();
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

float leftEncoderValue, rightEncoderValue;
task usercontrol()
{
	//Initialize odometry to run off of our left and right quadrature encoders
	//and assume we start at exactly position (0,0) and rotation 0
	odom_Initialize(leftEncoder, rightEncoder, 0, 0, 0);

	//Guess the scales for our robot. Our wheel base is 17.5 inches and we are
	//using 4 inch omni wheels. Note: This will never be as good as determining
	//thse numbers yourself experimentally and calling odometry_SetScales instead
	odom_SetScales(0.7315, 0.22687);

	//Start the odometry task. From this point onward, we are not allowed to
	//modify the values of the quadrature encoders we passed in earlier
	//startTask(trackOdometry);
	startTask(ArmClawController);
	int ptoPressed = 0;
	while (true)
	{
		leftEncoderValue = getLeftEncoder();
		rightEncoderValue = getRightEncoder();
		/*User drive method*/
		//arcadeDrive();

		if(vexRT(Btn8R)){
			programmingSkills();
			//auto_score();
			//clearUnderBar();
			//auto_climb();
			//turnAngle(90);
			//driveDistance(-36);
			} else {
			arcadeDrive();
			//tankDrive();
		}
		if(vexRT(Btn7U)){
			releaseHook();
		}
		else{
			resetHook();
		}


		if(vexRT(Btn7R) && ptoPressed == false){
			engagePTO();
			ptoPressed = true;
		}
		else if(!vexRT(Btn7R) && ptoPressed == true){
			ptoPressed = false;
		}
		if(vexRT(Btn8L)){
			resetEncoders();
		}

		gyro = getGyro();

		/*User state changes*/
		if (vexRT(Btn7D)) {
			armTask_ArmState = ARM_HIGH_HOLDING;
		}
		if (vexRT(Btn7L)) {
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
	//Naming convention: <red side = 1, blue side = 2><left side = 1, right side = 2><primary = 1, secondary = 2, tertiary = 3>

	switch (autonSelection)
	{
		case 111:
			//redLeftAutonPrimary();
			break;

		case 112:
			//redLeftAutonSecondary();
			break;

		case 113:
			//redLeftAutonTertiary();
			break;

		case 121:
			//redRightAutonPrimary();
			break;

		case 122:
			//redRightAutonSecondary();
			break;

		case 123:
			//redRightAutonTertiary();
			break;

		case 211:
			//blueLeftAutonPrimary();
			break;

		case 212:
			//blueLeftAutonSecondary();
			break;

		case 213:
			//blueLeftAutonTertiary();
			break;

		case 221:
			//blueRightAutonPrimary();
			break;

		case 222:
			//blueRightAutonSecondary();
			break;

		case 223:
			//blueRightAutonTertiary();
			break;

		default:
			break;
	}
}
