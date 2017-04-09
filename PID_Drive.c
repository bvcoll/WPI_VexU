bool isDriving;

int disterror, differror, distintegral, diffintegral, distderivative, diffderivative, distspeed, diffspeed, prevdisterror, prevdifferror = 0;

//PLACEHOLDER VALUES LINEAR DRIVE
float distP = .5;
float distI = 0.025;
float distD = 0.2;

float diffP = 1;
float diffI = 0.05;
float diffD = 0.2;

float linearDistance = 0;
int maxspeed = MAX_VOLTAGE;
int driveErrorThreshold = 5;

task PID_Drive(){

	while(true){
		//Executes once when isDriving is flipped true.
		if(isDriving){
			resetEncoders();
			prevdisterror = 0;
			prevdifferror = 0;
			distintegral = 0;
			diffintegral = 0;
		}
		//Runs the PID loop while isDriving is true and sets isDriving to false when done.
		while(isDriving){

			disterror = linearDistance - ((SensorValue(leftEncoder) + SensorValue(rightEncoder))/2); //Calculate distance error
			differror = SensorValue(leftEncoder) - SensorValue(rightEncoder); //Calculate difference error

			// Find the integral ONLY if within controllable range AND if the distance error is not equal to zero
			if( abs(disterror) < 60 && disterror != 0){
				distintegral = distintegral + disterror;
				}else{
				distintegral = 0; //Otherwise, reset the integral
			}

			// Find the integral ONLY if within controllable range AND if the difference error is not equal to zero
			if(abs(differror) < 60 && differror != 0){
				diffintegral = diffintegral + differror;
				}else{
				diffintegral = 0; //Otherwise, reset the integral
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

			if(disterror<driveErrorThreshold){
				isDriving = false;
			}
			wait1Msec(20);
		}

		//TODO: Add turn function.

		wait1Msec(20);
	}
}

void driveForward(float dist){
	linearDistance = dist;
	isDriving = true;
}
