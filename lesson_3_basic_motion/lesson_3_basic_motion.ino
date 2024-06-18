/* 
By Whit Whittall
COSGC New Robotics Workshop code for lesson 3 basic motion

robot is oriented correctly when the USB in of the arduino is_____
left and right directions referenced in comments are from the robot's perspective
*/ 

//--------------------rover geometry parameters--------------------
// motor_controller() uses these parameters to calculate wheel velocities
const float r = 0.030;   // radius of drive wheels in meters
const float L = 0.146;   // width separating the drive wheels in meters

//--------------------define motor pins--------------------
// setup() and drive() use these variables to control Arduino pins
// define pins to control right motor
const int R1 = 3;    //AI1
const int R2 = 4;    //AI2
const int pwmR = 5;  //PWMA

// define pins to control right motor
const int L1 = 7;    //BI1
const int L2 = 8;    //BI2
const int pwmL = 9;  //PWMB

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

  //--------------------declare velocity variables--------------------
  // declaring v and w here limits their scope to loop() so there aren't conflicts with motor_controller()
  int v = 0.346;        // linear velocity (0.346 is forward full, -0.346 is back full)
  int w = 0;        // angular velocity (4.73 is rotate left full, -4.73 is rotate right full)
  motor_controller(v, w);
  delay(2000);

  v = 0;
  w = 4.73;
  motor_controller(v,w);
  delay(2000);

  // play around with the velocities, motor_controller() function, and the delay() function and see what you can get your robot to do
}

//--------------------CUSTOM FXNS--------------------
// void motor_controller(v, w)
// void drive(vel_L, vel_R)

void motor_controller(int v, int w) {
// gets the desired linear and angular velocity of our robot
// expects -0.346 < v < 0.346 m/s, -4.73 < w < 4.73 rad/s
// motors will saturate if desired velocity vector is too large, best to keep desired velocities low
// determines required wheel speeds (in rad/s) based on linear and angular velocities (m/s, rad/s)
// maps required wheel speeds to PWM duty cycle

  int dphi_L = (v/r) - (L * w)/(2 * r);
  int dphi_R = (v/r) + (L * w)/(2 * r);

  // use the constrain function to keep dphi_L and dphi_R within certain boundaries
  // this prevents unintended behavior of the map function
  dphi_L = constrain(dphi_L, -11.52, 11.52);
  dphi_R = constrain(dphi_R, -11.52, 11.52);

  dphi_L = map(dphi_L, -11.52, 11.52, -255, 255);
  dphi_R = map(dphi_R, -11.52, 11.52, -255, 255);

  drive(dphi_L, dphi_R);
}

void drive(int vel_L, int vel_R) {
// based on PWM duty cycle setting, assigns motor driver pin values
// expects vel_L and vel_R to be between -255 and 255

  // left motor
  if (vel_L > 0) {  // left motor forward
    digitalWrite(L1, HIGH);
    digitalWrite(L2, LOW);
  }
  if (vel_L < 0) {  // left motor backward
    digitalWrite(L1, LOW);
    digitalWrite(L2, HIGH);
  }
  if (vel_L == 0) {  // left motor forward
    digitalWrite(L1, LOW);
    digitalWrite(L2, LOW);
  }
  // right motor
if (vel_R > 0) {  // right motor forward
    digitalWrite(R1, HIGH);
    digitalWrite(R2, LOW);
  }
  if (vel_R < 0) {  // right motor backward
    digitalWrite(R1, LOW);
    digitalWrite(R2, HIGH);
  }
  if (vel_R == 0) {  // right motor forward
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);
  }
  analogWrite(pwmL, abs(vel_L));
  analogWrite(pwmR, abs(vel_R));
}