// DrivePID.c
//
// Brandon Coll
//
// Code for robot drive PID control.

#pragma systemFile            	// eliminates warning for "unreferenced" functions

typedef struct {
	float kP;
	float kI;
	float kD;
	float target;
	float error;
	float integral;
	float derivative;
	float lastError;
	float threshold;
} pid;

float limit(float min, float max, float in);

//Drive forward PID code for autonomous.
bool drivePID(int mm) {
	int distance = ((180/3.142)*(mm/25.4));//+130*sgn(mm));
	int minSpeed = -70/2;
	int maxSpeed = 70/2;
	writeDebugStreamLine("%i",distance);
	SensorValue[leftEncoder] = 0;
	SensorValue[rightEncoder] = 0;
	int prevDist = 0;
	float vel = 999;
	long velTime = 0;
	pid straight;
	pid angle;
	short startupLoops = 50;
	short startupCounter = 0;
	short min = 0;
	short max = 0;

	straight.kP = 1;
	straight.kI = 0.00;
	straight.kD = 2;

	angle.kP = 1;
	angle.kI = 0.00;
	angle.kD = 0.55;
	long stoppedTime;

	int timeGuess = 7*abs(distance);//#magic number 5
	clearTimer(T2);
	angle.target=0;
	straight.target=distance;
	do{
		if(startupCounter <  startupLoops)
		{
			if(min > minSpeed)
				min = min - 2;

			if(max < maxSpeed)
				max = max + 2;
			startupCounter++;
		}
		else
		{
			min = minSpeed;
			max = maxSpeed;
		}

		straight.error=straight.target -(SensorValue[leftEncoder]+	SensorValue[rightEncoder]);
		angle.error=(SensorValue[leftEncoder]-SensorValue[rightEncoder]); //target is 0 so lazy


		if(angle.error == 0) { angle.integral = 0; }
		if(straight.error == 0) { straight.integral = 0; }

		straight.derivative = straight.error - straight.lastError;
		angle.derivative = angle.error - angle.lastError;

		straight.lastError = straight.error;
		angle.lastError = angle.error;

		int StraightOut = straight.kP*straight.error + straight.kI*straight.integral + straight.kD*straight.derivative;
		int AngleOut = angle.kP*angle.error + angle.kI*angle.integral + angle.kD*angle.derivative;

		StraightOut = limit(min,max,StraightOut);
		leftDrive(StraightOut-AngleOut);
		rightDrive(StraightOut+AngleOut);
		writeDebugStreamLine("%d,%d,%d",(straight.error)-130*sgn(mm),SensorValue[leftEncoder],SensorValue[rightEncoder]);
		delay(50);
		if(time1[T2]>timeGuess){ //if something went wrong give up
			leftDrive(0);
			rightDrive(0);
			return false;
		}
		vel = (((float)(SensorValue[leftEncoder] - prevDist))/((float)(nSysTime- velTime)));
		prevDist = SensorValue[leftEncoder];
		velTime = nSysTime;
		writeDebugStreamLine("Vel: %1.5f",vel);
		//if(vel < 0.05)
		//	stoppedTime = nSysTime;

	}	while((abs(straight.error) > 130 || abs(straight.lastError) > 130) || abs(angle.error) > 10 || abs(angle.lastError) > 10 || vel > 0);
	leftDrive(0);
	rightDrive(0);
	return true;
}


short const turnEstimatedTimeMultiplier=3;
//Turn PID code for autonomous.
bool turnPID(float degrees) {
	int distance = degrees*7.44;
	SensorValue[leftEncoder] = 0;
	SensorValue[rightEncoder] = 0;
	short maxAngleSpeed = 35;
	short minAngleSpeed = -1 * maxAngleSpeed;
	int prevDist = 0;
	float vel = 999;
	long velTime = 0;
	pid angle;


	angle.kP = 0.55;
	angle.kI = 0.0000;
	angle.kD = 1.35;

	int timeGuess = turnEstimatedTimeMultiplier*abs(distance);
	clearTimer(T2);
	angle.target = distance;
	do{

		angle.error=angle.target-(SensorValue[rightEncoder]-SensorValue[leftEncoder]); //target is 0 so lazy


		if(angle.error == 0) { angle.integral = 0; }


		angle.derivative = angle.error - angle.lastError;

		angle.lastError = angle.error;

		int AngleOut = angle.kP*angle.error + angle.kI*angle.integral + angle.kD*angle.derivative;
		vel = (((float)(SensorValue[leftEncoder] - prevDist))/((float)(nSysTime- velTime)));
		prevDist = SensorValue[rightEncoder];
		velTime = nSysTime;
		leftDrive(limit(minAngleSpeed,maxAngleSpeed,-AngleOut));
		rightDrive(limit(minAngleSpeed,maxAngleSpeed,AngleOut));
		//writeDebugStreamLine("%d,%d,%d",(angle.error),SensorValue[leftDrive],SensorValue[rightDrive]);
		delay(50);
		if(time1[T2]>timeGuess){ //if something went wrong give up
			leftDrive(0);
			rightDrive(0);
			return false;
		}
	}   while(abs(angle.error)>15 || abs(angle.lastError)>15 || abs(vel) > 0);
	leftDrive(0);
	rightDrive(0);
	return true;
}


float limit(float min, float max, float in){
	if(in > max)
		return max;
	if(in < min)
		return min;
	return in;
}
