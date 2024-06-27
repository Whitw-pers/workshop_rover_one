/* 
By Whit Whittall
COSGC New Robotics Workshop code for lesson 4 basic sensing

robot is oriented correctly when the USB in of the arduino is_____
left and right directions referenced in comments are from the robot's perspective
*/ 

// include the QuadratureEncoder library
#include <QuadratureEncoder.h>

//--------------------rover geometry parameters--------------------
// motor_controller() uses these parameters to calculate wheel velocities
// get_odom() uses them to track distance travelled
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

// declare a and b pins for encoders (can be analog pins)

const int a_r = ;
const int b_r = ;
const int a_l = ;
const int b_l = ;

// lets also create our encoder objects
Encoders right_encoder(a_r, b_r);
Encoders left_encoder(a_l, b_l);

//--------------------set up FSM--------------------
enum STATE {follow_right, turn_right, follow_left, turn_left, stop};
STATE last_state;
STATE current_state = follow_right;   // give it a value so we can enter the switch:case on first loop
STATE next_state = current_state;     // give this a value so our switch:case behaves

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600); // initialize the serial monitor so we can use it to check our work

  //--------------------setup motor pins--------------------
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(pwmR, OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(pwmL, OUTPUT);

  //--------------------setup sensor pins--------------------
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(ir, INPUT);
  // no need to setup our encoder pins, the library takes care of that

}

void loop() {
  // put your main code here, to run repeatedly:

  // uncomment to test get_distance()
  // Serial.println(get_distance());

  // uncomment to test get_line()
  // Serial.println(get_line());

  // uncomment to test get_odom()
  // motor_controller(0.346, 0);
  // delay(2000);
  // Serial.println(get_odom());

  // Put your FSM in here:
  switch (current_state) {

    case follow_right :
      // check for events
      if (get_odom() > 10) {        // our odometer hits 10m
        next_state = stop;
        break;
      }
      if (get_distance() < 0.1) {   // we come w/in 10cm (0.1m) of a box 
        next_state = turn_right;
        break;
      }
      // perform set of actions
      if (get_line()) {             // if robot sees the line
        motor_controller(0.1, -1);  // slowly forward, turning right
      }
      if (!get_line()) {            // if robot DOES NOT see the line
        motor_controller(0.1, 1);   // slowly forward, turning left
      }
      break;

    case turn_right :
      // perform set of actions
      motor_controller(0, -3);
      if (last_state != turn_right) { // debouncer to make sure we actually turn around
        delay(500); // will need to play with this value for effective debouncing
      }
      // check for events
      if (get_line()) {
        next_state = follow_left;
        break;
      }
      break;

    case follow_left :
      // check for events
      if (get_odom() > 10) {
        next_state = stop;
        break;
      }
      if (get_distance() < 0.1) {
        next_state = turn_left;
        break;
      }
      // perform set of actions
      if (get_line()) {             // if robot sees the line
        motor_controller(0.1, 1);   // slowly forward, turning right
      }
      if (!get_line()) {            // if robot DOES NOT see the line
        motor_controller(0.1, -1);  // slowly forward, turning left
      }
      break;

    case turn_left :
      // perform set of actions
      motor_controller(0, 3);
      if (last_state != turn_left) { // debouncer to make sure we actually turn around
        delay(500); 
      }
      // check for events
      if (get_line()) {
        next_state = follow_right;
        break;
      }
      break;

    case stop :
      // perform set of actions
      motor_controller(0, 0);
      right_encoder.setEncoderCount(0);
      left_encoder.setEncoderCount(0);
      delay(3000);
      // set next_state
      if (last_state == follow_right) {
        next_state = turn_right;
      }
      if (last_state == follow_left) {
        next_state = turn_left;
      }
      break;
    
    // could include a default that throws an error and exits?
  }

  last_state = current_state;
  current_state = next_state;
}


//--------------------CUSTOM FXNS--------------------
// float get_distance();
// bool get_line();
// long get_odom();
// void motor_controller(v, w)
// void drive(vel_L, vel_R)


float get_distance() {
  float echo_time;             // var to store time of flight
  float calculated_distance;   // var to store distance calculated from time of flight

  // send out an ultrasonic pulse thats 10ms long
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // use pulseIn function to see how long it takes for the pulse to return to the sensor
  echo_time = pulseIn(echo, HIGH);

  // calculate distance using formula from ToF sensor section
  return calculated_distance = echo_time * 346 / 2; // make sure this fxn returns reasonable vals
}

bool get_line() {
  return digitalRead(ir);
}

long get_odom() {
  // get encoder counts using getEncoderCount method from the Encoders class
  long left_encoder_count = left_encoder.getEncoderCount();
  long right_encoder_count = right_encoder.getEncoderCount();

  long left_wheel_pos = left_encoder_count * ((2 * 3.14) / 3575.04);
  long right_wheel_pos = right_encoder_count * ((2 * 3.14) / 3575.04);

  long odom = (r / 2) * (left_wheel_pos + right_wheel_pos);
  return odom;
}

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