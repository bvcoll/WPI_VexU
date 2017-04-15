//Drive control vars
bool isDriving = false;
bool isTurning = false;
bool isWall = false;
float linearDistance = 0;
float turnAng = 0;
int maxspeed = MAX_VOLTAGE;


int driveErrorThreshold = 1;
int turnErrorThreshold = 1;

//Linear variables
float disterror, differror, distintegral, diffintegral,  distspeed, diffspeed, direction = 0;
float distderivative, diffderivative, prevdisterror, prevdifferror = 0;

//PLACEHOLDER VALUES LINEAR DRIVE
float distP = 13;
float distI = 0.1;
float distD = 125; //.2
//PLACEHOLDER VALUES STEADY DRIVE
float diffP = 5;
float diffI = 0.05;
float diffD = 0.2;
//PLACEHOLDER VALUES STEADY DRIVE
float turnP = 10;
float turnI = 0.05;
float turnD = 65;

float BSScalingFactor = 1;
float getLeftEncoder() {
	return ((float)SensorValue(leftEncoder) / 360.0) * 3.25 * PI * BSScalingFactor;
}

float getRightEncoder() {
	return ((float)SensorValue(rightEncoder) / 360.0) * 3.25 * PI * BSScalingFactor;
}

float getGyro(){
	return (SensorValue(leftEncoder) - SensorValue(rightEncoder)) / 6.74;
}

float getAvgEncoder(){
	return (getRightEncoder() + getLeftEncoder()) /2 ;
}

void resetEncoders() {
	SensorValue(leftEncoder) = 0;
	SensorValue(rightEncoder) = 0;
}

void autoTurn(int voltage) {
	motor(LD1) = motor(LD2) = motor(LD3) = voltage;
	motor(RD1)  = motor(RD2) = motor(RD3) = -voltage;
}
bool hitWall = false;
int lastLatched, startingTime, wallTime = 0;

//////////////////////////////////////////////////////////* DRIVE PID TASK *//////////////////////////////////////////////////////////////////
task PID_Drive(){
	while(true){


		//Executes once when isDriving or isTurning is flipped true.
		if(isDriving || isTurning || isWall){
			resetEncoders();
			prevdisterror = 0;
			prevdifferror = 0;
			distintegral = 0;
			diffintegral = 0;
			lastLatched = 0;
			hitWall = false;
		}

		startingTime = nPgmTime + wallTime;

		//Runs the PID loop while isDriving is true and sets isDriving to false when done.
		while(isDriving){
			// Calculate both linear and difference errors
			disterror = linearDistance - getAvgEncoder(); //Calculate distance error
			differror = getGyro(); //Calculate difference error

			// Find the integral ONLY if within controllable range AND if the distance error is not equal to zero
			if(abs(distderivative) < 0.375 && disterror != 0){
				distintegral = distintegral + disterror;
				}else{
				distintegral = 0; //Otherwise, reset the integral
			}

			distderivative = disterror - prevdisterror; //Calculate distance derivative
			diffderivative = differror - prevdifferror; //Calculate difference derivative

			prevdisterror = disterror; //Update previous distance error
			prevdifferror = differror; //Update previous difference error

			distspeed = (disterror * distP) + (distintegral * distI) + (distderivative * distD); //Calculate distance speed
			diffspeed = (differror * diffP) + (diffintegral * diffI) + (diffderivative* diffD); //Calculate difference (turn) speed

			//Check that the speed is not exceeding the maximum set speed
			if(distspeed > maxspeed){
				distspeed = maxspeed;
			}

			//Check that the speed is not exceeding the maximum set speed
			if(distspeed < -maxspeed){
				distspeed = -maxspeed;
			}

			motor[LD1] = motor[LD2] = motor[LD3] = distspeed - diffspeed; //Set motor values
			motor[RD1] = motor[RD2] = motor[RD3] = distspeed + diffspeed; //Set motor values

			if(distspeed > 0){
				direction = 1;
				} else {
				direction = -1;
			}

			if(abs(disterror)<driveErrorThreshold){
				motor[LD1] = motor[LD2] = motor[LD3] = (15 * -direction);
				motor[RD1] = motor[RD2] = motor[RD3] = (15 * -direction);
				wait1Msec(100);
				if(abs(distderivative) < 0.5){
					isDriving = false;
					linearDistance = 0;
					motor[LD1] = motor[LD2] = motor[LD3] = 0;
					motor[RD1] = motor[RD2] = motor[RD3] = 0;
				}
			}
			wait1Msec(20);
		}

		//Runs the PID loop while isTurning is true and sets isTurning to false when done. Turns to the right by default for positive values.
		while(isTurning){

			disterror = turnAng - getGyro(); //Calculate distance error
			//differror = getLeftEncoder() - -1*getRightEncoder(); //Calculate difference error

			// Find the integral ONLY if within controllable range AND if the distance error is not equal to zero
			if( abs(disterror) < 6 && disterror != 0){
				distintegral = distintegral + disterror;
				}else{
				distintegral = 0; //Otherwise, reset the integral
			}


			distderivative = disterror - prevdisterror; //Calculate distance derivative
			diffderivative = differror - prevdifferror; //Calculate difference derivative

			prevdisterror = disterror; //Update previous distance error
			prevdifferror = differror; //Update previous difference error

			distspeed = (disterror * turnP) + (distintegral * turnI) + (distderivative * turnD); //Calculate distance speed
			//diffspeed = (differror * diffP) + (diffintegral * diffI) + (diffderivative* diffD); //Calculate difference (turn) speed
			diffspeed = 0; //TODO: Consider implementing this

			//Check that the speed is not exceeding the maximum set speed
			if(distspeed > maxspeed){
				distspeed = maxspeed;
			}

			//Check that the speed is not exceeding the maximum set speed
			if(distspeed < -maxspeed){
				distspeed = -maxspeed;
			}

			motor[LD1] = motor[LD2] = motor[LD3] = distspeed - diffspeed; //Set motor values
			motor[RD1] = motor[RD2] = motor[RD3] = -1*(distspeed + diffspeed); //Set motor values

			if(distspeed > 0){
				direction = 1;
				} else {
				direction = -1;
			}

			if(abs(disterror)<turnErrorThreshold){
				autoTurn(15 * -direction);
				wait1Msec(100);
				if(abs(distderivative) < 0.5){
					isTurning = false;
					turnAng = 0;
					motor[LD1] = motor[LD2] = motor[LD3] = 0;
					motor[RD1] = motor[RD2] = motor[RD3] = 0;
				}
			}
			wait1Msec(20);
		}
		while(isWall){
			disterror = getAvgEncoder();
			differror = getGyro(); //Calculate difference error

			distderivative = disterror - prevdisterror; //Calculate distance derivative
			diffderivative = differror - prevdifferror; //Calculate difference derivative

			prevdisterror = disterror; //Update previous distance error
			prevdifferror = differror; //Update previous difference error

			diffspeed = (differror * diffP) + (diffintegral * diffI) + (diffderivative* diffD); //Calculate difference (turn) speed

			motor[LD1] = motor[LD2] = motor[LD3] = -100 - diffspeed; //Set motor values
			motor[RD1] = motor[RD2] = motor[RD3] = -100 + diffspeed; //Set motor values

			if(abs(distderivative) > 0.2){
				lastLatched = nPgmTime;
				hitWall = false;
				} else {
				hitWall =  nPgmTime - lastLatched > 500;
			}

			if(startingTime < nPgmTime && hitWall){
				motor[LD1] = motor[LD2] = motor[LD3] = 0;
				motor[RD1] = motor[RD2] = motor[RD3] = 0;
				isWall = false;
			}

			wait1Msec(20);
		}

		wait1Msec(20);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void driveDistance(float dist){
	linearDistance = dist;
	isDriving = true;
	while(isDriving){
		wait1Msec(20);
	}
}

void turnAngle(float ang){
	turnAng = ang;
	isTurning = true;
	while(isTurning){
		wait1Msec(20);
	}
}

void driveWall(int time){
	wallTime = time;
	isWall = true;
	while(isWall){
		wait1Msec(20);
	}
}


//TODO: Add arm functions.
void dump(){
	armTask_ArmState = ARM_HOLDING;
	delay(500);
	motor[LD1] = motor[LD2] = motor[LD3] = -100;
	motor[RD1] = motor[RD2] = motor[RD3] = -100;
	delay(500);
	armTask_ArmState = ARM_DUMPING;
	driveWall(1000);
}
