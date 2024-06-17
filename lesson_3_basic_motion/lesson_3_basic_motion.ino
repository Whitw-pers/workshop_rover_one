/* 
Whit Whittall
COSGC New Robotics Workshop template code for lesson 3 basic motion

robot is oriented correctly when the USB in of the arduino is_____
left and right directions referenced in comments are from the robot's perspective
*/ 

//--------------------rover geometry parameters--------------------
const float r = ;   // radius of drive wheels
const float L = ;   // width separating the drive wheels

//--------------------define motor pins--------------------
// define pins to control right motor
const int R1 = 3;    //AI1
const int R2 = 4;    //AI2
const int pwmR = 5;  //PWMA

// define pins to control right motor
const int L1 = 7;    //BI1
const int L2 = 8;    //BI2
const int pwmL = 9;  //PWMB

//--------------------initialize velocity variables--------------------
float v = 0;      // linear velocity (positive is drive forward, negative is drive backwards)
float w = 0;      // angular velocity (positive is rotate left, negative is rotate right)
float dphi_R = 0;  // right wheel velocity
float dphi_L = 0;  // left wheel velocity

void setup() {
  // put your setup code here, to run once:

  //--------------------setup motor pins--------------------
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(pwmR, OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(pwmL, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

}

//--------------------CUSTOM FXNS--------------------
// void motor_controller(v, w)

void motor_controller(float v, float w) {
// gets the desired linear and angular velocity of our robot
// expects -1 < v, w < 1
// determines required wheel speeds based on linear and angular velocities
// maps required wheel speeds to PWM duty cycle

}