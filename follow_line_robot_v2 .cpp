/*Bien luu gia tri tin hieu doc duoc*/
int sensor[5] = {0, 0, 0, 0, 0};    // tin hieu doc tu sensor, thu tu sensor tinh tu trai qua phai voi chieu cua xe huong ve phia truoc
int button = 0;  // tin hieu doc tu button


/*Cac cong cau hinh*/
int sensor_port[5] = {12, 11, 10, 9, 8};  // cong doc sensor
int button_port = 4;   // cong doc button
int motor_left = 6, motor_right = 5;  // cong dieu khien dong co bang PWM

/*Loi - do lech cua xe*/
int error = 0;
int error_old = 0;
int delay_ = 50;

/*Doc cam bien */
int Read_Sensor(){
  int sensor_s = 0;
  int sensor_status = 0;
  for(int i = 0; i < 5; i++){
    sensor[i] = digitalRead(sensor_port[i]);
    sensor_s = sensor_s << 1;
    sensor_s = sensor_s | sensor[i];
  }
  return sensor_s;
}

/*Thiet lap he so loi*/
int Get_Error(){
  int sensor_v = Read_Sensor();
  /*Cac truong hop xe can quay motor ben phai*/
  if(sensor_v == 0b10011 || sensor_v == 0b11001 || sensor_v == 0b11011 || sensor_v == 0b10111 || sensor_v == 0b11101 ||
     sensor_v == 0b01111 || sensor_v == 0b11110 || sensor_v == 0b11101 || sensor_v == 0b10111 || sensor_v == 0b11000 ||
     sensor_v == 0b00011)
    sensor_v = ~sensor_v;

  if(sensor_v == 0b00100)
    return 0;
  else if(sensor_v == 0b01100) // 0 1 1 0 0
    return 1;
  else if(sensor_v == 0b01000) // 0 1 0 0 0
    return 2;
  else if(sensor_v == 0b10000 || sensor_v == 0b11000)  // 1 0 0 0 0 or 1 1 0 0 0
    return 3;
  else if(sensor_v == 0b11100)  // 0 0 1 1 1 
    return 4;

  /*Cac truong hop xe can quay motor ben trai*/
  else if(sensor_v == 0b00110)  // 0 0 1 1 0
    return -1;
  else if(sensor_v == 0b00010)  // 0 0 0 1 0
    return -2;
  else if(sensor_v == 0b00001 || sensor_v == 0b00011) // 0 0 0 0 1 or 0 0 0 1 1
    return -3;
  else if(sensor_v == 0b00111)
    return -4;
  
  /*Khong co cam bien nao nam trong duong di*/
  else if(sensor_v == 0b00000)
    return 5;
}
/*Kiem tra cam bien 2 co trong vach den hay khong*/
void Check_Sensor_2(){
  Read_Sensor();
  while(sensor[2] != 1){
    Read_Sensor();
  }
}

void Turn_Left(int value){    
  analogWrite(motor_right, value);
  analogWrite(motor_left, 0);
}

void Turn_Right(int value){
  analogWrite(motor_right, 0);
  analogWrite(motor_left, value);
}

void To_FordWard(){
  analogWrite(motor_right, 200);
  analogWrite(motor_left, 200);
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

void Move_Mode_1(int error){    /*Line mau den*/

}

void Move_Mode_2(int error){  /*Line mau trang*/

}
void loop() {
  button = digitalRead(button_port);  /*Doc button*/
  if(button == 1){
    while(1){
        error = Get_Error();
        if(error == 0){
          To_FordWard();
        }
        /* Quay motor phai - Turn_Left*/
        else if(error == 4){
          Turn_Left(150);
          Check_Sensor_2();
        }

        else if(error == 3){
          Turn_Left(120);
          Check_Sensor_2();        
        }
        
        else if(error == 2){
          Turn_Left(100);
          Check_Sensor_2();
        }

        else if(error == 1){
          Turn_Left(80);
          Check_Sensor_2();
        }

        /*Quay motor trai - Turn_Right*/

        else if(error == -4){
          Turn_Right(150);
          Check_Sensor_2(); 
        }

        else if(error == -3){
          Turn_Right(120);
          Check_Sensor_2();
        }
        else if(error == -2){
          Turn_Right(100);
          Check_Sensor_2();          
        }
        else if(error == - 1){
          Turn_Right(80);
          Check_Sensor_2();
        }
        /*Khong co sensor nao trong vach den*/
        else if(error == 5 ){                         // 00000
            if(error_old < 0){
              Turn_Right(100);
              Check_Sensor_2();
            }
            else if(error_old > 0){
                Turn_Left(100);
                Check_Sensor_2();
            }
        }
        
        error_old = error;
        delay(50); 
  
    }
  }
}

