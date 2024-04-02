#include <Servo.h>
 //Declararea pinilor pentru modulul Bluetooth
#define HC05_RX 19  
#define HC05_TX 18  

//Declararea pinilor pentru senzorul ultrasonic
const int trigPin = 35;
const int echoPin = 34;

//Variablie folosite pentru modul automat
int Numar = 0;
int count = 0;
int spate = 0;
long duration;
int distance;
int Mers = 0;
//Declararea pinilor pentru motoare
#define enA 48
#define in1 50
#define in2 52
#define enB 49
#define in3 53
#define in4 51
//Variablia folosita pentru a retine timpul de start atat al arduinoului dar si a modului automat 
unsigned long startTime;

Servo servo_z_axis;
Servo servo_x_axis;
Servo servo_y_axis;
Servo servo_clamp;
//Declararea gradelor default a bratului pentru a porni dintr-o pozitie default
int x_axis_degree = 145;
int y_axis_degree = 35;
int z_axis_degree = 100;
int clamp_degree = 40;

//Variabila folosita pentru bluetooth
char data = NULL;

// #define VRX_PIN A15
// #define VRY_PIN A14
// int xValue = 0;  // To store value of the X axis
// int yValue = 0;  // To store value of the Y axis

//Declararea pinilor pentru joystickurile prezente pe placuta de expansiune
#define left_joystick_x A0
#define left_joystick_y A1
#define right_joystick_x A2
#define right_joystick_y A3
//****************************************************************************

void Miscare() {  //Modul in care se misca bratul in modul automat

  int step = 5;  //saritura de grade pe care o are in modul automat

  servo_clamp.write(50);
  delay(50);
  while (y_axis_degree < 80) {
    y_axis_degree = y_axis_degree + step;
    servo_y_axis.write(y_axis_degree);
    delay(50);
  }
  while (x_axis_degree > 95) {
    x_axis_degree = x_axis_degree - step;
    servo_x_axis.write(x_axis_degree);
    delay(50);
  }
  while (y_axis_degree < 125) {
    y_axis_degree = y_axis_degree + step;
    servo_y_axis.write(y_axis_degree);
    delay(50);
  }
  while (x_axis_degree < 140) {
    x_axis_degree = x_axis_degree + step;
    servo_x_axis.write(x_axis_degree);
    delay(50);
  }
  servo_clamp.write(40);
  delay(50);
  servo_clamp.write(30);
  delay(50);
  servo_clamp.write(20);  //Prinderea a fost efectuata
  delay(50);
  while (x_axis_degree > 95) {
    x_axis_degree = x_axis_degree - step;
    servo_x_axis.write(x_axis_degree);
    delay(50);
  }
  while (y_axis_degree > 50) {
    y_axis_degree = y_axis_degree - step;
    servo_y_axis.write(y_axis_degree);
    delay(50);
  }
  delay(100);
  while (z_axis_degree > 0) {
    z_axis_degree = z_axis_degree - step * 2;
    servo_z_axis.write(z_axis_degree);
    delay(50);
  }  //Robotul a miscat bratul in dreapta pentru a fi usor de depozitat

  clamp_degree = 20;
}
void Default_Pos() {  //Aduce bratul la pozitia default
  x_axis_degree = 145;
  y_axis_degree = 35;
  z_axis_degree = 100;
  clamp_degree = 40;
}
void Auto() {            //Modul automat
  startTime = millis();  //introducerea timpului de start a modului automat

  do {  //Se actioneaza senzorul ultrasonic
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;
    if (distance < 5 && count < 1 && spate == 0) {  //daca este foarte aproape de obiect v-a da cu spatele
      standBy();
      goBackwards();
      delay(18);
      standBy();
      spate = 1;
    } else if (distance < 10 && count < 1 && spate == 0) {  //daca nu a dat cu spatele deja v-a da cu spatele
      standBy();
      goBackwards();
      delay(10);
      standBy();
      spate = 1;
    } else if (distance <= 10 && count % 2 == 0 && Mers == 1) {  //Daca masinuta a mers si a ajuns prea aproape de obiect acesta v-a da cu spatele
      standBy();
      goBackwards();
      delay(100);
      standBy();
      Default_Pos();
      delay(10);
      Miscare();
      count += 1;

    }

    else if (distance == 14 && count % 2 == 0) {  //Daca distanta este adecvata pentru a prinde obiectul direct se va efectua prinderea
      standBy();
      Default_Pos();
      delay(10);
      Miscare();
      count += 1;

    }

    else if (distance > 14) {  //Daca distanta este mai mare va merge inainte pana va da de obiect
      goForward();
      Mers = 1;
    } else if (distance < 10 && count == 1) {  //Daca s-a realizat deja prinderea si a intalnit alt obiect/perete se va oprii
      standBy();
      startTime = 10000;
    } else if (distance < 10 && count < 1) {  //Daca este prea aproape de obiect v-a da cu spatele
      standBy();
      goBackwards();
      delay(100);
      standBy();
    }
  } while (millis() - startTime < 10000);  //Daca timpul alocat de 10 secunde v-a trece acesta se va oprii
}
void goForward() {  //Functie pentru mers in fata
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void goBackwards() {  //Functie pentru mers in spate
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnLeft() {  //Functie pentru mers fata stanga
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnRight() {  //Functie pentru mers fata dreapta

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
}

void standBy() {  //Functie pentru oprirea motoarelor
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

// functia pentru joystick roti
// void checkMode(){
// if((xValue > 700 && yValue > 700)  ||
// (xValue <250  && yValue <250) ||
// (xValue <250 && yValue > 750) ||
// (xValue >750 && yValue < 250) ||
// ( (xValue > 400 && xValue <  550) && (yValue > 400  && yValue < 550)  ) ){ // check up for diagonal and origin
//   standBy();
//  }
// else if(xValue > 700 && (yValue < 720 && yValue > 300)){ // check up for reverse
//   goBackwards();

// }
// else if( xValue < 250 && (yValue > 300 && yValue < 720)){ // checkup for forward
//    goForward();

// }
// else if( yValue > 720 && (xValue > 300 && xValue < 750)){ // check for left
//  turnLeft();

// }
// else if( yValue < 300 && (xValue > 300 && xValue < 750)){ // check for right
//   turnRight();

// }

// }

void setup() {
  Serial.begin(9600);
  startTime = millis();

  Serial1.begin(9600);
  Serial.println("Serial1 Initialized");
  //Declararea pinilor pentru servourile bratului
  servo_z_axis.attach(9);
  servo_clamp.attach(5);
  servo_x_axis.attach(7);
  servo_y_axis.attach(11);
  //Declararea pinilor pentru motoare
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
 
  pinMode(HC05_RX, INPUT);
  pinMode(HC05_TX, OUTPUT);
}

void loop() {
  if (Serial1.available() > 0) {
    // Citirea din modulul Bluetooth
    data = Serial1.read();

 
    Serial.print("Received from HC-05: ");
    Serial.println(data);


    Serial1.print("Echo: ");
    Serial1.println(data);
  }
  if (isdigit(data)) {
    Numar = data - '0';  // Convert character to integer
  }

  if (Numar > 7) {  //motoare
    switch (data) {
      case 'F':
        goForward();
        delay(10);
        break;
      case 'B':
        goBackwards();
        delay(10);

        break;
      case 'L':
        turnLeft();
        delay(10);

        break;
      case 'R':
        turnRight();
        delay(10);

        break;
      case 'S':
        standBy();
        delay(10);

        break;
      default:
        standBy();
        delay(10);

        break;
    }
  } else if (Numar == 0) {  //Stanga-dreapta si fata spate
    switch (data) {
      case 'F':
        x_axis_degree += 1;
        delay(10);
        break;
      case 'B':
        x_axis_degree -= 1;
        delay(10);
        break;
      case 'L':
        z_axis_degree += 1;
        delay(10);
        break;
      case 'R':
        z_axis_degree -= 1;
        delay(10);
        break;
      default:
        standBy();
        break;
    }
    // brat sus-jos si gheara
  } else if (Numar > 0 && Numar < 7) {
    switch (data) {
      case 'F':
        y_axis_degree += 1;
        delay(10);
        break;
      case 'B':
        y_axis_degree -= 1;
        delay(10);
        break;
      case 'L':
        clamp_degree += 1;
        delay(10);
        break;
      case 'R':
        clamp_degree -= 1;
        delay(10);
        break;
      default:
        standBy();
        break;
    }
  }
  switch (data) {
    case 'W':
      count = 0;
      Auto();
      break;
    case 'U':
      Default_Pos();
      break;
    default:
      standBy();
      break;
  }
  //Citirea valorilor Joystickurilor
  int left_joystick_x_value = analogRead(left_joystick_x);
  int left_joystick_y_value = analogRead(left_joystick_y);
  int right_joystick_x_value = analogRead(right_joystick_x);
  int right_joystick_y_value = analogRead(right_joystick_y);

  // xValue = analogRead(VRX_PIN);
  // yValue = analogRead(VRY_PIN);

  if (left_joystick_x_value < 340) {
    y_axis_degree -= 1;
    delay(10);
  } else if (left_joystick_x_value > 680) {
    y_axis_degree += 1;
    delay(10);
  }

  if (left_joystick_y_value < 340) {
    clamp_degree -= 1;
    delay(10);
  } else if (left_joystick_y_value > 680) {
    clamp_degree += 1;
    delay(10);
  }

  if (right_joystick_x_value < 340) {
    x_axis_degree -= 1;
    delay(10);
  } else if (right_joystick_x_value > 680) {
    x_axis_degree += 1;
    delay(10);
  }

  if (right_joystick_y_value < 340) {
    z_axis_degree -= 1;
    delay(10);
  } else if (right_joystick_y_value > 680) {
    z_axis_degree += 1;
    delay(10);
  }


//Setarea unui min/max de grade pentru servouri
  z_axis_degree = min(180, max(0, z_axis_degree));
  x_axis_degree = min(145, max(45, x_axis_degree));
  y_axis_degree = min(140, max(0, y_axis_degree));
  clamp_degree = min(50, max(20, clamp_degree));

//Setarea vitezei pentru motoare
  int pwmOutput = 150;
  digitalWrite(enA, pwmOutput);
  digitalWrite(enB, pwmOutput);


//Trimiterea gradelor catre servouri
  servo_clamp.write(clamp_degree);
  servo_x_axis.write(x_axis_degree);
  servo_y_axis.write(y_axis_degree);
  servo_z_axis.write(z_axis_degree);
}