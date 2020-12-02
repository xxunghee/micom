// 2020-2 마이컴 프로그래밍 응용
// 8조 박하이디, 김성희 

#include <Servo.h>
int Left_motor_back = 8;    //Left_motor_back(IN1)
int Left_motor_go = 9;      //Left_motor_go(IN2)
int Right_motor_go = 10;    //Right_motor_go(IN3)
int Right_motor_back = 11;  //Right_motor_back(IN4)

int Echo = A1;
int Trig = A0;
int Front_Distance = 0;
int Left_Distance = 0;
int Right_Distance = 0;
int servopin = 2;
int myangle;
int pulsewidth;
int val;
int SL; // SensorLeft
int SR; // SensorRight


const int SensorRight = 3;   //(P3.2 OUT1)
const int SensorLeft = 4;    //(P3.3 OUT2)
const int SensorRight_2 = 5; //(P3.27 OUT1)
const int SensorLeft_2 = 6;  //(P3.3 OUT2)

void setup()
{
  pinMode(SensorRight, INPUT);
  pinMode(SensorLeft, INPUT);
  pinMode(SensorRight_2, INPUT);
  pinMode(SensorLeft_2, INPUT);

  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
  pinMode(servopin, OUTPUT);

  pinMode(Left_motor_go, OUTPUT);
  pinMode(Left_motor_back, OUTPUT);
  pinMode(Right_motor_go, OUTPUT);
  pinMode(Right_motor_back, OUTPUT);
  Serial.begin(9600);

}

// 전진
void run(int time)
{
  digitalWrite(Right_motor_go, HIGH);
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, 100); //PWM은 0~255속도조절, 좌우 차이가 증감
  analogWrite(Right_motor_back, 0);
  digitalWrite(Left_motor_go, HIGH);
  digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_go, 100); //PWM은 0~255속도조절， 좌우 차이가 증감
  analogWrite(Left_motor_back, 0);
  delay(time * 20);
}

// 브레이크
void brake(int time)
{
  digitalWrite(Right_motor_go, LOW); //Right, Left 모두 LOW
  digitalWrite(Right_motor_back, LOW);
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, LOW);
  delay(time * 2);
}

// 오른쪽 바퀴로 좌회전
void left(int time)
{
  digitalWrite(Right_motor_go, HIGH);   // Right_motor_go가 HIGH
  digitalWrite(Right_motor_back, LOW);  // Right_motor_back이 LOW
  analogWrite(Right_motor_go, 90 );     // Right_motor_go의 속도 90
  analogWrite(Right_motor_back, 0);     // PWM0~255속도조절
  digitalWrite(Left_motor_go, LOW);     // Left는 LOW
  digitalWrite(Left_motor_back, LOW);   // Left는 LOW
  analogWrite(Left_motor_go, 0);        // Left_motor_go의 속도 0
  analogWrite(Left_motor_back, 0);      // PWM0~255 속도조절
  delay(time * 50);
}

// 양쪽 바퀴로 좌회전
void spin_left(int time)
{
  digitalWrite(Right_motor_go, HIGH);
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, 85);    // Right_motor_go의 속도 85
  analogWrite(Right_motor_back, 0);
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, HIGH);
  analogWrite(Left_motor_go, 0);
  analogWrite(Left_motor_back, 75);  // Left_motor_back의 속도 75
  delay(time);
}

// 왼쪽 바퀴로 우회전
void right(int time)
{
  digitalWrite(Right_motor_go, LOW);    // Right_motor_go가 LOW
  digitalWrite(Right_motor_back, LOW);  // Right_motor_back이 LOW
  analogWrite(Right_motor_go, 0);       // Right_motor_go의 속도 0
  analogWrite(Right_motor_back, 0);     // PWM0~255속도조절
  digitalWrite(Left_motor_go, HIGH);    // Left_motor_go HIGH
  digitalWrite(Left_motor_back, LOW);   // Left_motor_go LOW
  analogWrite(Left_motor_go, 150);      // Left_motor_go의 속도 150
  analogWrite(Left_motor_back, 0);      // PWM0~255속도조절
  delay(time * 50);
}

// 두 바퀴로 우회전
void spin_right(int time)
{
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, HIGH);
  analogWrite(Right_motor_go, 0);
  analogWrite(Right_motor_back, 150);  // Right_motor_go의 속도 150
  digitalWrite(Left_motor_go, HIGH);
  digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_go, 150);    // Left_motor_go의 속도 150
  analogWrite(Left_motor_back, 0);    // Left_motor_back의 속도 0
  delay(time);
}

// 거리 측정
float Distance_test()
{
  digitalWrite(Trig, LOW);
  delayMicroseconds(50);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(2000);
  digitalWrite(Trig, LOW);
  float Fdistance = pulseIn(Echo, HIGH);
  Fdistance = Fdistance / 58;
  return Fdistance;
}

// 서보모터 제어
void servopulse(int servopin, int myangle)
{
  pulsewidth = (myangle * 11) + 500;
  digitalWrite(servopin, HIGH);
  delayMicroseconds(pulsewidth);
  digitalWrite(servopin, LOW);
  delay(20 - pulsewidth / 1000);
}

// 앞에 있는 장애물 감지
void front_detection()
{
  for (int i = 0; i <= 5; i++)
  {
    servopulse(servopin, 80);
    delay(5);
  }
  Front_Distance = Distance_test();
}

// 왼쪽에 있는 장애물 감지
void left_detection()
{
  for (int i = 0; i <= 15; i++)
  {
    servopulse(servopin, 140);
    delay(5);
  }
  Left_Distance = Distance_test();
}

// 오른쪽에 있는 장애물 감지
void right_detection()
{
  for (int i = 0; i <= 15; i++)
  {
    servopulse(servopin, 30);
    delay(5);
  }
  Right_Distance = Distance_test();
}



void loop() //while(1) 지움
{
  // 라인트레이서 감지
  SR = digitalRead(SensorRight);
  SL = digitalRead(SensorLeft);

  // 흰-흰
  if (SL == LOW && SR == LOW)
  {
    run(5);
  }
  // 검-흰
  else if (SL == HIGH & SR == LOW)
  {
    spin_left(5);
  }
  // 흰-검
  else if (SR == HIGH & SL == LOW)
  {
    spin_right(5);
  }
  // 검-검
  else if (SL == HIGH && SR == HIGH )
  {
    brake(10);

    // 거리 측정
    left_detection();
    delay(500);
    front_detection();
    delay(500);
    right_detection();

    brake(10);

    // 앞에 장애물이 있는 경우 한 바퀴 회전
    if ( Front_Distance < 50)
    {
      spin_right(820);
      Serial.print("front ");
      Serial.println(Front_Distance);
    }
    // 왼쪽에 장애물이 있는 경우 왼쪽으로 회전
    else if (Left_Distance < 50)
    {
      left(17);
      Serial.print("left ");
      Serial.println(Left_Distance);
    }
    //오른쪽에 장애물이 있는 경우 오른쪽으로 회전
    else if ( Right_Distance < 50)
    {
      right(14);
      Serial.print("right ");
      Serial.println(Right_Distance);
    }
    // 장애물이 없는 경우 직진
    else
    {
      run(5);
    }
  }
}
