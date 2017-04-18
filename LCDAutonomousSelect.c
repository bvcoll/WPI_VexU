/***************************************************************************/
/*                                                                         */
/* Subroutine - Uses the LCD to select an autonomous                       */
/*                                                                         */
/***************************************************************************/
int selectAutonomous()
{
	string currentAuton, specifier;

	int autonColor = 1, autonTile = 1, autonLevel = 1;
	string autonColorString, autonTileString, autonLevelString;

	sprintf(specifier, "Side, Back1,Back2");
	sprintf(autonColorString, "Left");
	sprintf(autonTileString, "False");
	sprintf(autonLevelString, "True");

	timer t;
	timer_Initialize(&t);

	while (true)
	{
		//Left button changes auton color
		if (nLCDButtons & kButtonLeft)
		{
			autonColor = autonColor == 1 ? 2 : 1;
			autonColorString = autonColor == 1 ? "Left" : "Right";
			waitForLCDRelease();
		}

		//Center button changes auton tile or exits
		if (nLCDButtons & kButtonCenter)
		{
			autonTile = autonTile == 1 ? 2 : 1;
			autonTileString = autonTile == 1 ? "False" : "True";
			waitForLCDRelease();
		}

		//Right button changes auton level
		if (nLCDButtons & kButtonRight)
		{
			//Exit when holding right button
			timer_PlaceHardMarker(&t);
			while (nLCDButtons & kButtonRight)
			{
				if (timer_GetDTFromHardMarker(&t) >= 250)
				{
					return (autonColor * 100) + (autonTile * 10) + autonLevel;
				}

				wait1Msec(5);
			}
			timer_ClearHardMarker(&t);


			autonLevel = autonLevel == 1 ? 2 : 1;
			autonLevelString = autonLevel == 1 ? "True" : "False";

			waitForLCDRelease();
		}

		sprintf(currentAuton, "%s,%s,%s", autonColorString, autonTileString, autonLevelString);

		displayLCDCenteredString(0, currentAuton);
		displayLCDCenteredString(1, specifier);

		wait1Msec(25);
	}
}


//LCD Library callback function
void lcd_invoke(int func)
{
	switch (func)
	{
	case 1:
		endPreAuton = true;
		stopTask(lcdControlTask);
		break;

	case 2:
		autonSelection = selectAutonomous();
		break;

	case 3:
		programmingSkills();
		break;

	default:
		break;
	}
}
