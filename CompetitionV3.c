#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_3,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           leftMotor,     tmotorVex393_HBridge, openLoop, driveLeft)
#pragma config(Motor,  port2,           clawMotor,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           highMotor,     tmotorVex393_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port4,           chevronMotor,  tmotorVex393_MC29, openLoop, encoderPort, I2C_2)
#pragma config(Motor,  port5,           trackMotor,    tmotorVex393_MC29, openLoop, encoderPort, I2C_3)
#pragma config(Motor,  port6,           MGMotorL,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           MGMotorR,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          rightMotor,    tmotorVex393_HBridge, openLoop, driveRight)

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        Creator: Vex Team GWS Lörrach Baden-Wuerttemberg                   */
/*				Version: 3.2																											 */
/*				Email: 	 marc.steinebrunner@gmail.com															 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

//VEX cortex platform
#pragma platform(VEX2)

#pragma competitionControl(Competition)

#include "JoystickDriver.c"

//Main competition background code...do not modify!
//#include "Vex_Competition_Includes.c"


/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*---------------------------------------------------------------------------*/

void pre_auton()
{
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*---------------------------------------------------------------------------*/

task autonomous()
{
	// ..........................................................................
	// Insert user code here.
	// ..........................................................................

	// Remove this function call once you have "real" code.
	//  AutonomousCodePlaceholderForTesting();
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

task main()			//Has to be changed to "usercontrol()"
{
	// Values from the Joysticks will be safed here
	int joy_xl;
	int joy_yl;
	int joy_xr;
	int joy_yr;

	//The Tolleranz that you can hold the sticks even when you dont want to move the Robot
	int tolleranz = 10;

	//Encoders Set to 0 for the Startvalue, In the Code the value will be used for safing the position
	nMotorEncoder[highMotor] = 0;
	nMotorEncoder[chevronMotor] = 0;

	//The Minimal and Maximal Values for the Servos are saved here they can be modifyed to get the right Position
	int highMotorEncMin = 0;
	int highMotorEncMax =  500;

	int chevronMotorEncMin = -500;
	int chevronMotorEncMax =  500;

	//The direction the motor have to take
	int direction[] = {0,0,0,0}; //4Direction 0.MobileGoal 1.Chevron 2.Lift 3.Claw

	//The Power that goes to the Motor (Basic Movement)
	int leftPower = 0;
	int rightPower = 0;

	while(true){
		//current Values of the joysticks


		joy_xl = vexRT[Ch4];
		joy_yl = vexRT[Ch3];
		joy_xr = vexRT[Ch1];
		joy_yr = vexRT[Ch2];

		//reset the Power Level of the Movemt motors
		leftPower = 0;
		rightPower = 0;

		//Movement
		{
			if((joy_yl < tolleranz)  && (joy_yl > -tolleranz))		//Backward and Forward Movement 
			{
				leftPower 	= leftPower;
				rightPower 	= rightPower;
				}else{
				leftPower = leftPower +(joy_yl /4);
				rightPower = leftPower;
			}

			if((joy_xl < tolleranz) && (joy_xl > -tolleranz))			//Right Left Movement
			{
				leftPower = leftPower 	+(0);
				rightPower = rightPower +(0);
				}
				else if(joy_xl > 0){														//Right
				leftPower = leftPower +(joy_xl/4);
				rightPower = rightPower -(joy_xl/4);
				}
				else{																					//Left
				leftPower = leftPower -(joy_xl/4);
				rightPower = rightPower +(joy_xl/4);
			}
		}

		//Mobile Goal	Btn Gruppe 5
		{
			if(vexRT[Btn5U] == 1){
				direction[0] = 1;
			}
			else if(vexRT[Btn5D] == 1){
				direction[0] = -1;
			}
		}

		//Winkel Ch1
		{
			if(nMotorEncoder[chevronMotor] < chevronMotorEncMin){
				if(vexRT[Ch1] > 0){
					direction[1] = 1;
				}
				}else if(nMotorEncoder[chevronMotor] > chevronMotorEncMax){
				if(vexRT[Ch1] < 0){
					direction[1] = -1;
				}
			}
		}


		//Lift UP/DOWN Ch2
		{
			if(nMotorEncoder[highMotor] < highMotorEncMin){
				if(vexRT[Ch2] > 0){
					direction[2] = 1;
				}
				}else if(nMotorEncoder[highMotor] > highMotorEncMax){
				if(vexRT[Ch2] < 0){
					direction[2] = -1;
				}
			}
		}

		//Claw Btn Gruppe 6
		{
			if(vexRT[Btn6U] == 1)
			{
				direction[3] = 1;
			}
			else if(vexRT[Btn6D] == 1)
			{
				direction[3] = -1;
			}
		}

		motor[leftMotor] 		= 	leftPower;
		motor[rightMotor]		= 	rightPower;

		motor[MGMotorL]			=	direction[0] * 63;
		motor[MGMotorR] 		=	direction[0] * 63;

		motor[chevronMotor]		=	direction[1] * 63;

		motor[highMotor]		=	direction[2] * 63;

		motor[clawMotor]		=	direction[3] * 127;
	}
}
