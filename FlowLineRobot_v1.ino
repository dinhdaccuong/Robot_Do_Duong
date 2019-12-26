
//motir left
#define MOTOR_LEFT_1A		15		//PWM motor 1
#define MOTOR_LEFT_2A		16		//not PWM

//motor right
#define MOTOR_RIGHT_3A		0		
#define MOTOR_RIGHT_4A		2		//PWM motor 2

#define NUMBER_OF_SENSOR 5

#define DEBUG 0

typedef struct
{
	int16_t speed_motorLeft;
	int16_t speed_motorRight;

} motor_speed_t, *motor_speed_p;



uint8_t sensor_port[5] = { 14, 12, 13, 5, 4 };
uint8_t sensor_status[5] = { 0, 0, 0, 0, 0 };
uint8_t sensor_status_old[5] = { 0, 0, 0, 0, 0 };

motor_speed_t robot_moveForward_speed = {1000, 1000 };

motor_speed_t robot_turnLeft_error1 = { 0, 700 };
motor_speed_t robot_turnLeft_error2 = { 0, 500 };

motor_speed_t robot_turnRight_error1 = { 700, 0 };
motor_speed_t robot_turnRight_error2 = { 700, 0 };

int8_t error = 0;
int8_t error_old = 0;
int8_t check = 0;
int8_t check_old = 0;

void motorLeft_moveForward(uint16_t speed);

void motorLeft_moveBackward(uint16_t speed);

void motorRight_moveForward(uint16_t speed);

void motorRight_moveBackward(uint16_t speed);

void robot_moveForward(motor_speed_p p_speed);

void robot_turnLeft(motor_speed_p p_speed);

void robot_turnRight(motor_speed_p p_speed);

void robot_readSensor();

int8_t robot_getError_modeWhiteLine();

int8_t robot_getError_modeBlackLine();

uint8_t robot_checkMode();





void setup()
{
#if DEBUG
	Serial.begin(115200);
#endif

	pinMode(sensor_port[0], INPUT);
	pinMode(sensor_port[1], INPUT);
	pinMode(sensor_port[2], INPUT);
	pinMode(sensor_port[3], INPUT);
	pinMode(sensor_port[4], INPUT);
	pinMode(MOTOR_LEFT_1A, OUTPUT);
	pinMode(MOTOR_LEFT_2A, OUTPUT);
	pinMode(MOTOR_RIGHT_3A, OUTPUT);
	pinMode(MOTOR_RIGHT_4A, OUTPUT);

	robot_turnLeft(&robot_turnLeft_error1);
}


void loop()
{
	//check = robot_checkMode();
	//if (check == 10) {	// Neu khong nhan ra mode nao thi chay theo trang thai cu
	//	check = check_old;
	//}
	//if (check == 1) {
	//	error = robot_getError_modeWhiteLine();
	//}
	//else if (check == 0) {
	//	error = robot_getError_modeBlackLine();
	//}
	//check_old = check;
	//switch (error) {
	//case 0:
	//	robot_moveForward(&robot_moveForward_speed);
	//	break;
	//case 1:
	//	robot_turnLeft(&robot_turnLeft_error1);
	//	break;
	//case 2:
	//	robot_turnLeft(&robot_turnLeft_error2);
	//	break;
	//case -1:
	//	robot_turnRight(&robot_turnRight_error1);
	//	break;
	//case -2:
	//	robot_turnRight(&robot_turnLeft_error2);
	//	break;
	//default:
	//	if (check == 1) { /*Neu line trang*/
	//		if (sensor_status_old[0] == 1 || sensor_status_old[1] == 1)
	//			robot_turnLeft(&robot_turnLeft_error2);
	//		else if (sensor_status_old[3] == 1 || sensor_status_old[4] == 1)
	//			robot_turnRight(&robot_turnRight_error2);
	//	}
	//	else if (check == 0) {
	//		if (sensor_status_old[0] == 0 || sensor_status_old[1] == 0)
	//			robot_turnLeft(&robot_turnLeft_error2);
	//		else if (sensor_status_old[3] == 0 || sensor_status_old[4] == 0)
	//			robot_turnRight(&robot_turnRight_error2);
	//	}
	//	break;
	//}
	//for (uint8_t i = 0; i < 5; i++)
	//	sensor_status_old[i] = sensor_status[i];
}
void robot_readSensor()
{
	for (uint8_t i = 0; i < NUMBER_OF_SENSOR; i++)
	{
		sensor_status[i] = digitalRead(sensor_port[i]);
	}
}

uint8_t robot_checkMode()
{
	robot_readSensor();
	/*Neu hai cam bien ben ngoai = 0 -> xe dang di tren line trang*/
	if (sensor_status[0] == 0 && sensor_status[4] == 0)
		return 1;   // Line trang - Mode White line
	else if (sensor_status[0] == 1 && sensor_status[4] == 1)
		return 0;   // Line den - Mode Black line
	return 10;		// Neu khong nhan ra mode nao
}

int8_t robot_getError_modeWhiteLine()
{
	robot_readSensor();
	if (sensor_status[0] == 0 && sensor_status[1] == 0 && sensor_status[2] == 1 && sensor_status[3] == 0 && sensor_status[4] == 0)
		return 0;

	/*Xe lech phai*/
	else if (sensor_status[0] == 0 && sensor_status[1] == 1 && sensor_status[2] == 1 && sensor_status[3] == 0 && sensor_status[4] == 0)
		return 1;
	else if ((sensor_status[0] == 1 && sensor_status[1] == 0 && sensor_status[2] == 0 && sensor_status[3] == 0 && sensor_status[4] == 0) ||
		(sensor_status[0] == 1 && sensor_status[1] == 1 && sensor_status[2] == 0 && sensor_status[3] == 0 && sensor_status[4] == 0))
		return 2;
	/*Xe lech trai*/
	else if (sensor_status[0] == 0 && sensor_status[1] == 0 && sensor_status[2] == 1 && sensor_status[3] == 1 && sensor_status[4] == 0)
		return -1;
	else if ((sensor_status[0] == 0 && sensor_status[1] == 0 && sensor_status[2] == 0 && sensor_status[3] == 1 && sensor_status[4] == 1) ||
		(sensor_status[0] == 0 && sensor_status[1] == 0 && sensor_status[2] == 0 && sensor_status[3] == 0 && sensor_status[4] == 1))
		return -2;
	/*Khong co sensor nao trong vach*/
	else if (sensor_status[0] == 0 && sensor_status[1] == 0 && sensor_status[2] == 0 && sensor_status[3] == 0 && sensor_status[4] == 0)
		return  3;
	return 5;
}

int8_t robot_getError_modeBlackLine()
{
	robot_readSensor();
	if (sensor_status[0] == 1 && sensor_status[1] == 1 && sensor_status[2] == 0 && sensor_status[3] == 1 && sensor_status[4] == 1)
		return 0;

	/*Xe lech phai*/
	else if ((sensor_status[0] == 1 && sensor_status[1] == 0 && sensor_status[2] == 0 && sensor_status[3] == 1 && sensor_status[4] == 1) ||
		(sensor_status[0] == 1 && sensor_status[1] == 0 && sensor_status[2] == 0 && sensor_status[3] == 1 && sensor_status[4] == 1))
		return 1;
	else if ((sensor_status[0] == 0 && sensor_status[1] == 1 && sensor_status[2] == 1 && sensor_status[3] == 1 && sensor_status[4] == 1) ||
		(sensor_status[0] == 0 && sensor_status[1] == 0 && sensor_status[2] == 1 && sensor_status[3] == 1 && sensor_status[4] == 1))
		return 2;

	/*Xe lech trai*/
	else if ((sensor_status[0] == 1 && sensor_status[1] == 1 && sensor_status[2] == 0 && sensor_status[3] == 0 && sensor_status[4] == 1) ||
		(sensor_status[0] == 1 && sensor_status[1] == 1 && sensor_status[2] == 0 && sensor_status[3] == 0 && sensor_status[4] == 1))
		return -1;
	else if ((sensor_status[0] == 1 && sensor_status[1] == 1 && sensor_status[2] == 1 && sensor_status[3] == 0 && sensor_status[4] == 0) ||
		(sensor_status[0] == 1 && sensor_status[1] == 1 && sensor_status[2] == 1 && sensor_status[3] == 1 && sensor_status[4] == 0))
		return -2;

	else if (sensor_status[0] == 0 && sensor_status[1] == 0 && sensor_status[2] == 0 && sensor_status[3] == 0 && sensor_status[4] == 0)
		return  3;
	return 5;
}


void motorLeft_moveForward(uint16_t speed)
{
	digitalWrite(MOTOR_LEFT_2A, LOW);
	analogWrite(MOTOR_LEFT_1A, speed);
}

void motorLeft_moveBackward(uint16_t speed)
{
	digitalWrite(MOTOR_LEFT_2A, HIGH);
	analogWrite(MOTOR_LEFT_1A, 1023 - speed);
}

void motorRight_moveForward(uint16_t speed)
{
	digitalWrite(MOTOR_RIGHT_3A, LOW);
	analogWrite(MOTOR_RIGHT_4A, speed);
}

void motorRight_moveBackward(uint16_t speed)
{
	digitalWrite(MOTOR_RIGHT_3A, HIGH);
	analogWrite(MOTOR_RIGHT_4A, 1023-speed);
}


void robot_moveForward(motor_speed_p p_speed)
{
	motorLeft_moveForward(p_speed->speed_motorLeft);
	motorRight_moveForward(p_speed->speed_motorRight);
}

void robot_turnLeft(motor_speed_p p_speed)
{
	if (p_speed->speed_motorLeft < 0)
		motorLeft_moveBackward(-(p_speed->speed_motorLeft));
	else
		motorLeft_moveForward(p_speed->speed_motorLeft);

	motorRight_moveForward(p_speed->speed_motorRight);
}

void robot_turnRight(motor_speed_p p_speed)
{
	if (p_speed->speed_motorRight < 0)
		motorRight_moveBackward(-(p_speed->speed_motorRight));
	else
		motorRight_moveForward(p_speed->speed_motorRight);

	motorLeft_moveForward(p_speed->speed_motorLeft);
}