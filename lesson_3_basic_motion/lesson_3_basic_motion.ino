/* 
By Whit Whittall
COSGC New Robotics Workshop code for lesson 3 basic motion

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
  // declaring v and w here limits their scope to the loop() function so there aren't conflicts with motor_controller()

  //--------------------declare velocity variables--------------------
  int v = 0;        // linear velocity (100 forward full, -100 is back full)
  int w = 0;        // angular velocity (100 is rotate left full, -100 is rotate right full)

  motor_controller(v, w);
}

//--------------------CUSTOM FXNS--------------------
// void motor_controller(v, w)
// void drive(vel_L, vel_R)

void motor_controller(int v, int w) {
// gets the desired linear and angular velocity of our robot
// expects -1 < v, w < 1
// determines required wheel speeds based on linear and angular velocities
// maps required wheel speeds to PWM duty cycle

  int dphi_L = (v/r) - (L * w)/(2 * r);
  int dphi_R = (v/r) + (L * w)/(2 * r);

  dphi_L = constrain(dphi_L, -100, 100);
  dphi_R = constrain(dphi_R, -100, 100);

  dphi_L = map(dphi_L, -100, 100, -255, 255);
  dphi_R = map(dphi_R, -100, 100, -255, 255);

  drive(dphi_L, dphi_R);
}

void drive(int vel_L, int vel_R) {
// expects vel_L and vel_R to be between -255 and 255

  // left motor
  if (spd_L > 0) {  // left motor forward
    digitalWrite(L1, HIGH);
    digitalWrite(L2, LOW);
  }
  if (spd_L < 0) {  // left motor backward
    digitalWrite(L1, LOW);
    digitalWrite(L2, HIGH);
  }
  if (spd_L == 0) {  // left motor forward
    digitalWrite(L1, LOW);
    digitalWrite(L2, LOW);
  }
  // right motor
if (spd_R > 0) {  // right motor forward
    digitalWrite(R1, HIGH);
    digitalWrite(R2, LOW);
  }
  if (spd_R < 0) {  // right motor backward
    digitalWrite(R1, LOW);
    digitalWrite(R2, HIGH);
  }
  if (spd_R == 0) {  // right motor forward
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);
  }
  analogWrite(pwmL, abs(spd_L));
  analogWrite(pwmR, abs(spd_R));
}