int sensor_s[5] = {0, 0, 0, 0, 0};  //trang thai cua sensor
int sensor_s_old[5] = {0, 0, 0, 0, 0};
int sensor_port[5] = {12, 11, 10, 9, 8};  // cong doc sensor
int motor_left = 6, motor_right = 5;  // cong dieu khien dong co bang PWM
int button_s = 0;            // trang thai cua button
int button_port = 4;        // cong doc button

void Read_Sensor(){
  for(int i = 0; i < 5; i++){
    sensor_s[i] = digitalRead(sensor_port[i]);
  }
}

/*Kiem tra loai duong dang di*/
int Check_Mode(){
    Read_Sensor();
    /*Neu hai cam bien ben ngoai = 1 -> xe dang di tren line trang*/
    if(sensor_s[0] == 1 && sensor_s[4] == 1)
        return 1;   // Line trang - Mode 1
    else if(sensor_s[0] == 0 && sensor_s[4] == 0)
        return 0;   // Line den - Mode 0
    return 10;		// Neu khong nhan ra mode nao
}
/*Mode 0: line den*/
int Get_Error_Mode0(){
    Read_Sensor();
    /*Xe bam duong*/
    if(sensor_s[0] == 0 && sensor_s[1] == 0 && sensor_s[2] == 1 && sensor_s[3] == 0 && sensor_s[4] == 0)
        return 0;

    /*Xe lech phai*/
    else if(sensor_s[0] == 0 && sensor_s[1] == 1 && sensor_s[2] == 0 && sensor_s[3] == 0 && sensor_s[4] == 0)
        return 1;
    else if((sensor_s[0] == 1 && sensor_s[1] == 0 && sensor_s[2] == 0 && sensor_s[3] == 0 && sensor_s[4] == 0) || 
            (sensor_s[0] == 1 && sensor_s[1] == 1 && sensor_s[2] == 0 && sensor_s[3] == 0 && sensor_s[4] == 0))
        return 2;
    /*Xe lech trai*/
    else if(sensor_s[0] == 0 && sensor_s[1] == 0 && sensor_s[2] == 0 && sensor_s[3] == 1 && sensor_s[4] == 0)
        return -1;
    else if((sensor_s[0] == 0 && sensor_s[1] == 0 && sensor_s[2] == 0 && sensor_s[3] == 1 && sensor_s[4] == 1) || 
             (sensor_s[0] == 0 && sensor_s[1] == 0 && sensor_s[2] == 0 && sensor_s[3] == 0 && sensor_s[4] == 1))
        return -2;
    /*Khong co sensor nao trong vach*/
    else if(sensor_s[0] == 0 && sensor_s[1] == 0 && sensor_s[2] == 0 && sensor_s[3] == 0 && sensor_s[4] == 0)
        return  3;
    return 5;
}

/*Mode 1: line trang */
int Get_Error_Mode1(){
    Read_Sensor();
    /*Xe bam duong*/
    if(sensor_s[0] == 1 && sensor_s[1] == 1 && sensor_s[2] == 0 && sensor_s[3] == 1 && sensor_s[4] == 1)
        return 0;
    
    /*Xe lech phai*/
    else if((sensor_s[0] == 1 && sensor_s[1] == 0 && sensor_s[2] == 1 && sensor_s[3] == 1 && sensor_s[4] == 1) ||
            (sensor_s[0] == 1 && sensor_s[1] == 0 && sensor_s[2] == 0 && sensor_s[3] == 1 && sensor_s[4] == 1))
        return 1;
    else if((sensor_s[0] == 0 && sensor_s[1] == 1 && sensor_s[2] == 1 && sensor_s[3] == 1 && sensor_s[4] == 1) || 
            (sensor_s[0] == 0 && sensor_s[1] == 0 && sensor_s[2] == 1 && sensor_s[3] == 1 && sensor_s[4] == 1))
        return 2;

    /*Xe lech trai*/
    else if((sensor_s[0] == 1 && sensor_s[1] == 1 && sensor_s[2] == 1 && sensor_s[3] == 0 && sensor_s[4] == 1) ||
            (sensor_s[0] == 1 && sensor_s[1] == 1 && sensor_s[2] == 0 && sensor_s[3] == 0 && sensor_s[4] == 1))
        return -1;
    else if((sensor_s[0] == 1 && sensor_s[1] == 1 && sensor_s[2] == 1 && sensor_s[3] == 0 && sensor_s[4] == 0) || 
             (sensor_s[0] == 1 && sensor_s[1] == 1 && sensor_s[2] == 1 && sensor_s[3] == 1 && sensor_s[4] == 0))
        return -2;

    else if(sensor_s[0] == 0 && sensor_s[1] == 0 && sensor_s[2] == 0 && sensor_s[3] == 0 && sensor_s[4] == 0)
        return  3;
    return 5;
}
/*Cac ham di chuyen xe*/
void Turn_Left(int value){    
  analogWrite(motor_right, value);
  analogWrite(motor_left, 0);
}

void Turn_Right(int value){
  analogWrite(motor_right, 0);
  analogWrite(motor_left, value);
}

void To_FordWard(){
  analogWrite(motor_right, 255);
  analogWrite(motor_left, 247);
}

void setup() {
  Serial.begin(9600);
  pinMode(motor_left, OUTPUT);
  pinMode(motor_right, OUTPUT);
  pinMode(button_port, INPUT);
  for(int i = 0; i < 5; i++){
    pinMode(sensor_port[i], INPUT);
  }
}
void loop(){
    button_s = digitalRead(button_port);
    int error = 0;
    int error_old = 0;
    int check = 0;
    int check_old = 0;
    int i = 0;
    if (button_s == 1){
        while(1){      
            check = Check_Mode();
            if(check == 10){	// Neu khong nhan ra mode nao thi chay theo trang thai cu
                check = check_old;
            }
             if(check == 1){
                 error = Get_Error_Mode1();
             }
             else if(check == 0){
                 error = Get_Error_Mode0();
            }
            check_old = check;
            switch(error){
                case 0: 
                    To_FordWard();
                    break;
                case 1:
                    Turn_Left(100);
                    break;
                case 2:
                    Turn_Left(70);
                    break;
                case -1:
                    Turn_Right(100);
                    break;
                case -2:
                    Turn_Right(70);
                    break;
                 default:
                    if(check == 1){ /*Neu line trang*/
                        if(sensor_s_old[0] == 0 || sensor_s_old[1] == 0)
                            Turn_Left(100);
                        else if(sensor_s_old[3] == 0 || sensor_s_old[4] == 0)
                            Turn_Right(100);
                    }
                    else if(check == 0){
                        if(sensor_s_old[0] == 1 || sensor_s_old[1] == 1)
                            Turn_Left(100);
                        else if(sensor_s_old[3] == 1 || sensor_s_old[4] == 1)
                            Turn_Right(100);
                    }
                    break;
            }
            for(i = 0; i < 5; i++)
                sensor_s_old[i] = sensor_s[i];
        }
    }
}
