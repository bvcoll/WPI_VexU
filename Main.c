#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, dgtl1,  leftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  rightEncoder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  armBottomLimit, sensorTouch)
#pragma config(Sensor, dgtl6,  armTopLimit,    sensorTouch)
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
// old 150
int middleArmSetpoint = 200; //Setpoint to hold at for driving around the field
int scoreThreshold = 300;  //Point at which to open the claw
int highHoldingArmSetpoint = 350;
int topArmSetpoint = 700;  //Setpoint for dumping
bool isAutonomous = false;

/* USER CONTROL ROBOT STATES */
#define BOTTOM 0
#define HOLDING 1
#define DUMPING 2
#define RESET 3

//ARM PID CONSTANTS
float kP_arm = 0.625, kI_arm = 0, kD_arm = 0;
//Drive PID CONSTANTS
float kP_drive = 4.5, kI_drive = 0, kD_drive = 0;
//Drive PID CONSTANTS
float kP_turn = 1.35, kI_turn = 0, kD_turn = 0.2;



// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Include PID libraries
#define BCI_USE_POS_PID
#define BCI_USE_ODOMETRY
#include "BCI.h"



//INCLUDES
#include "Vex_Competition_Includes.c" //Main competition background code...do not modify!
#include "Claw.c" //All control code for the claw.
#include "Drive.c" //Basic drive functions.
#include "Arm.c" //Basic arm functions.
#include "ArmUserControl.c" //User control code for the arm.
#include "ArmClawController.c"
#include "Auto.c"



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

	while (true)
	{
		leftEncoderValue = getLeftEncoder();
		rightEncoderValue = getRightEncoder();
		/*User drive method*/
		//arcadeDrive();

		if(vexRT(Btn8R)){
		  auto_score();
			//turnAngle(90)
		} else {
		arcadeDrive();
		}

		if(vexRT(Btn8L)){
		resetEncoders();
		}

		gyro = getGyro();

		/*User state changes*/
		if (vexRT(Btn7D)) {
			armTask_ArmState = ARM_RESET;
		}
		if (vexRT(Btn7L) || vexRT(Btn7R)) {
			armTask_ArmState = ARM_USER;
		}
		if(vexRT(Btn6D)) {
			armTask_ArmState = ARM_HOLDING;
		}
		if (vexRT(Btn6U)) {
			armTask_ArmState = ARM_DUMPING;
		}

	}
}
