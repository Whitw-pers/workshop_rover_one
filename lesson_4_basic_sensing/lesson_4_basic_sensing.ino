/* 
By Whit Whittall
COSGC New Robotics Workshop code for lesson 4 basic sensing

robot is oriented correctly when the USB in of the arduino is_____
left and right directions referenced in comments are from the robot's perspective
*/ 

//--------------------rover geometry parameters--------------------
// motor_controller() uses these parameters to calculate wheel velocities
const float r = 0.030;   // radius of drive wheels in meters
const float L = 0.146;   // width separating the drive wheels in meters

//--------------------declare motor pins--------------------
// setup() and drive() use these variables to control Arduino pins
// declare pins to control right motor
const int R1 = 3;    //AI1
const int R2 = 4;    //AI2
const int pwmR = 5;  //PWMA

// declare pins to control right motor
const int L1 = 7;    //BI1
const int L2 = 8;    //BI2
const int pwmL = 9;  //PWMB

//--------------------declare sensor pins--------------------
// declare echo and trig pins for ultrasonic sensor

const int echo = ;
const int trig = ;

// declare pin for infrared sensor

const int ir = ;

// declare a and b pins for encoders

const int a_r = ;
const int b_r = ;
const int a_l = ;
const int b_l = ;

void setup() {
  // put your setup code here, to run once:

  //--------------------setup motor pins--------------------
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(pwmR, OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(pwmL, OUTPUT);

  //--------------------setup sensor pins--------------------
  pinMode(echo, );
  pinMode(trig, );
  pinMode(ir, );
  pinMode(a_r, );
  pinMode(b_r, );
  pinMode(a_l, );
  pinMode(b_l, );

}

void loop() {
  // put your main code here, to run repeatedly:

  // going to put our FSM in here

}

//--------------------CUSTOM FXNS--------------------
// float get_distance();
// bool get_line();
// float get_odom();
// void motor_controller(v, w)
// void drive(vel_L, vel_R)

void motor_controller(int v, int w) {
// determines required wheel speeds (in rad/s) based on linear and angular velocities (m/s, rad/s)
// maps required wheel speeds to PWM duty cycle
// expects -0.346 < v < 0.346 m/s, -4.73 < w < 4.73 rad/s
// motors will saturate if desired velocity vector is too large, best to keep desired velocities low

  float dphi_L = (v/r) - (L * w)/(2 * r);
  float dphi_R = (v/r) + (L * w)/(2 * r);

  // use the constrain function to keep dphi_L and dphi_R within certain boundaries
  // this prevents unintended behavior of the map function
  dphi_L = constrain(dphi_L, -11.52, 11.52);
  dphi_R = constrain(dphi_R, -11.52, 11.52);

  // need to confirm map() behaves well when given non-int input
  // map() uses integer math, returns only integers which is not a problem in this case
  // would be a problem if it misbehaves with float input
  int duty_L = map(dphi_L, -11.52, 11.52, -255, 255);
  int duty_R = map(dphi_R, -11.52, 11.52, -255, 255);

  drive(duty_L, duty_R);
}

void drive(int duty_L, int duty_R) {
// based on PWM duty cycle setting, assigns motor driver pin values
// expects duty_L and duty_R to be between -255 and 255

  // left motor
  if (duty_L > 0) {  // left motor forward
    digitalWrite(L1, HIGH);
    digitalWrite(L2, LOW);
  }
  if (duty_L < 0) {  // left motor backward
    digitalWrite(L1, LOW);
    digitalWrite(L2, HIGH);
  }
  if (duty_L == 0) {  // left motor forward
    digitalWrite(L1, LOW);
    digitalWrite(L2, LOW);
  }
  // right motor
if (duty_R > 0) {  // right motor forward
    digitalWrite(R1, HIGH);
    digitalWrite(R2, LOW);
  }
  if (duty_R < 0) {  // right motor backward
    digitalWrite(R1, LOW);
    digitalWrite(R2, HIGH);
  }
  if (duty_R == 0) {  // right motor forward
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);
  }
  analogWrite(pwmL, abs(duty_L));
  analogWrite(pwmR, abs(duty_R));
}