/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                        Copyright (c) James Pearman                          */
/*                                   2011                                      */
/*                            All Rights Reserved                              */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*        Module:     MultiTask.c                                              */
/*        Author:     James Pearman                                            */
/*        Created:    13 Oct 2011                                              */
/*                                                                             */
/*        Revisions:  V0.1                                                     */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*        Description:                                                         */
/*                                                                             */
/*        Demo for slew rate motor control                                     */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
/*                                                                             */

/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*  definitions used by driver control                                         */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

#define     JOY_DRIVE_V     Ch3
#define     JOY_DRIVE_H     Ch1
#define     JOYSTICK_THRESHOLD   15

/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*  definitiona and variables for the motor slew rate controller.              */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

#define MOTOR_NUM               10       //kNumbOfTotalMotors
#define MOTOR_MAX_VALUE         127
#define MOTOR_MIN_VALUE         (-127)
#define MOTOR_DEFAULT_SLEW_RATE 25      // Default (10) will cause 375mS from full fwd to rev
#define MOTOR_FAST_SLEW_RATE    256 //256    // essentially off
#define MOTOR_TASK_DELAY        15      // task 1/frequency in mS (about 66Hz)
#define MOTOR_DEADBAND          10

// Array to hold requested speed for the motors
int motorReq[ MOTOR_NUM ];

// Array to hold "slew rate" for the motors, the maximum change every time the task
// runs checking current mootor speed.
int motorSlew[ MOTOR_NUM ];

/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*  Task  - compares the requested speed of each motor to the current speed    */
/*          and increments or decrements to reduce the difference as nexessary */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

task MotorSlewRateTask()
{
	int motorIndex;
	int motorTmp;

	// Initialize stuff
	for(motorIndex=0;motorIndex<MOTOR_NUM;motorIndex++)
	{
		motorReq[motorIndex] = 0;
		motorSlew[motorIndex] = MOTOR_DEFAULT_SLEW_RATE;
	}

	// run task until stopped
	while( true )
	{
		// run loop for every motor
		for( motorIndex=0; motorIndex<MOTOR_NUM; motorIndex++)
		{
			// So we don't keep accessing the internal storage
			motorTmp = motor[ motorIndex ];

			// Do we need to change the motor value ?
			if( motorTmp != motorReq[motorIndex] )
			{
				// increasing motor value
				if( motorReq[motorIndex] > motorTmp )
				{
					motorTmp += motorSlew[motorIndex];
					// limit
					if( motorTmp > motorReq[motorIndex] )
						motorTmp = motorReq[motorIndex];
				}

				// decreasing motor value
				if( motorReq[motorIndex] < motorTmp )
				{
					motorTmp -= motorSlew[motorIndex];
					// limit
					if( motorTmp < motorReq[motorIndex] )
						motorTmp = motorReq[motorIndex];
				}

				// finally set motor
				motor[motorIndex] = motorTmp;
			}
		}

		// Wait approx the speed of motor update over the spi bus
		wait1Msec( MOTOR_TASK_DELAY );
	}
}

/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

void
DriveLeftMotor( int value )
{
	motorReq[LD1] = value;
	motorReq[LD2] = value;
	motorReq[LD3] = value;
}

/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

void
DriveRightMotor( int value )
{
	motorReq[RD1] = value;
	motorReq[RD2] = value;
	motorReq[RD3] = value;
}

/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

task ArcadeDrive()
{
	int    ctl_v;
	int    ctl_h;
	int    drive_l;
	int    drive_r;

	// Basic arcade control
	while( true )
	{
		// Get joystick H and V control
		ctl_v = vexRT[ JOY_DRIVE_V ];

		if(vexRT[Btn5U]){
			ctl_h = vexRT[ JOY_DRIVE_H ]/2;
			} else {
			ctl_h = vexRT[ JOY_DRIVE_H ];
		}

		// Ignore joystick near center
		if( (abs(ctl_v) <= JOYSTICK_THRESHOLD) && (abs(ctl_h) <= JOYSTICK_THRESHOLD) )
		{
			drive_l = 0;
			drive_r = 0;
		}
		else
		{
			// Create drive for left and right motors
			drive_l = (ctl_v + ctl_h);
			drive_r = (ctl_v - ctl_h);
		}

		// Now send out to motors
		DriveLeftMotor( drive_l );
		DriveRightMotor( drive_r );

		// don't hog CPU
		wait1Msec( 25 );
	}
}
