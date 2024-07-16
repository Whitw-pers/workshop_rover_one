/* 
By Whit Whittall
COSGC New Robotics Workshop code for lesson 5 advanced sensing

Demonstrates use of IMU sensor with I2C communication protocol

left and right directions referenced in comments are from the robot's perspective
*/ 

// include the QuadratureEncoder library
#include <QuadratureEncoder.h>

// include communications libraries
#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20948.h>
#include <Adafruit_Sensor.h>
#include <Wire.h> // A4 is SDA, A5 is SCL

// create icm object from ICM20948 class
Adafruit_ICM20948 icm;

//--------------------rover geometry parameters--------------------
// motor_controller() uses these parameters to calculate wheel velocities
// get_odom() uses them to track distance travelled
const float r = 0.030;   // radius of drive wheels in meters
const float L = 0.146;   // width separating the drive wheels in meters

//--------------------declare motor pins--------------------
// setup() and drive() use these variables to control Arduino pins
// declare pins to control right motor
const int R1 = 8;    //AI1  -> D8
const int R2 = 7;    //AI2  -> D7
const int pwmR = 6;  //PWMA -> D6

// declare pins to control left motor
const int L1 = 5;    //BI1  -> D5
const int L2 = 4;    //BI2  -> D4
const int pwmL = 3;  //PWMB -> D3

//--------------------declare sensor pins--------------------
// declare echo and trig pins for ultrasonic sensor

const int echo = 11; // Echo  -> D11
const int trig = 12; // Trig  -> D12

// declare pin for infrared sensor

const int ir = 13;  // OUT  -> D13

// declare a and b pins for encoders (can be analog pins)
// right encoder
const int a_r = A3; // A  -> A3
const int b_r = A2; // B  -> A2
// left encoder
const int a_l = A1; // A  -> A1
const int b_l = A0; // B  -> A0

// lets also create our encoder objects
Encoders right_encoder(a_r, b_r);
Encoders left_encoder(a_l, b_l);

//--------------------set up FSM--------------------
enum STATE {drive_forward, turn_right, turn_left};
STATE last_state;
STATE current_state = drive_forward;  // give it a value so we can enter the switch:case on first loop
STATE next_state = current_state;     // give this a value so our switch:case behaves

void setup() {
  // put your setup code here, to run once:

  //--------------------set up motor pins--------------------
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(pwmR, OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(pwmL, OUTPUT);

  //--------------------set up sensor pins--------------------
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(ir, INPUT);
  // no need to setup our encoder pins, the library takes care of that

  //--------------------set up magnetometer--------------------
  Serial.begin(115200);

  while (!Serial) {
    delay(10);
  }

  Serial.println("Adafruit ICM20948 test");

  // try to initialize
  if (!icm.begin_I2C()) {
    Serial.println("failed to find ICM20948 chip");
    while (1) { // enter infinite loop if chip is not found
      delay(10);
    }
  }

  Serial.println("ICM20948 found");

  // icm.setMagDataRate(AK09916_MAG_DATARATE_100_HZ); 
  Serial.print("Magnetometer data rate set to: ");
  switch (icm.getMagDataRate()) {
    case AK09916_MAG_DATARATE_SHUTDOWN:
      Serial.println("Shutdown");
      break;
    case AK09916_MAG_DATARATE_SINGLE:
      Serial.println("Single/One shot");
      break;
    case AK09916_MAG_DATARATE_10_HZ:
      Serial.println("10 Hz");
      break;
    case AK09916_MAG_DATARATE_20_HZ:
      Serial.println("20 Hz");
      break;
    case AK09916_MAG_DATARATE_50_HZ:
      Serial.println("50 Hz");
      break;
    case AK09916_MAG_DATARATE_100_HZ:
      Serial.println("100 Hz");
      break;
  }
  Serial.println();

}

void loop() {
  // put your main code here, to run repeatedly:

  // Put your FSM in here:
  switch (current_state) {
    case drive_forward :
      // check for events
      if (get_distance() < 0.1) {
        next_state = turn_right;
        break;
      }
      if (get_odom() > 0.2 && last_state == turn_right) {
        next_state = turn_left;
        break;
      }
      // perform actions
      motor_controller(0.1, 0);
      break;
    case turn_right :
      // check for events
      if (abs(get_angle() + 90) < 5) {  // think I'll need to play with the logic of this statement
        next_state = drive_forward;
        break;
      }
      // perform actions
      motor_controller(0, -3);
      right_encoder.setEncoderCount(0);
      left_encoder.setEncoderCount(0);
      break;
    case turn_left :
      // check for events
      if (abs(get_angle()) < 5) {
        next_state = drive_forward;
        break;
      }
      if (get_distance() < 0.1) {
        next_state = turn_right;
        break;
      }
      // perform actions
      motor_controller(0, 3);
      break;
  }

  last_state = current_state;
  current_state = next_state;
}


//--------------------CUSTOM FXNS--------------------
// float get_angle();
// float get_distance();
// bool get_line();
// long get_odom();
// void motor_controller(v, w)
// void drive(vel_L, vel_R)

float get_angle() {
  // get a new normalized sensor event
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t mag;
  sensors_event_t temp;
  icm.getEvent(&accel, &gyro, &temp, &mag);

  // determine angular position
  float angle = atan2(-mag.magnetic.y, mag.magnetic.x) * RAD_TO_DEG;

  return angle;
}

float get_distance() {
  // finds the distance of an object in front of the ultrasonic sensor
  // if this returns 0.00, check that the sensor is getting 5V
  // also check trig and echo pins aren't reversed
  float echo_time;             // var to store time of flight
  float calculated_distance;   // var to store distance calculated from time of flight

  // send out an ultrasonic pulse thats 10ms long
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // use pulseIn function to see how long it takes for the pulse to return to the sensor
  echo_time = pulseIn(echo, HIGH);

  // calculate distance using formula from ToF sensor section
  return calculated_distance = (echo_time / 2) / (346 * 10); 
  //*10 is to scale units appropriately since pulseIn returns in microseconds
}

bool get_line() {
  // returns 0 (false) if no reflection seen (over black surface)
  // returns 1 (true) if reflection seen (over white surface)
  return !digitalRead(ir);
}

float get_odom() {
  // get encoder counts using getEncoderCount method from the Encoders class
  long left_encoder_count = left_encoder.getEncoderCount();
  long right_encoder_count = right_encoder.getEncoderCount();

  float left_wheel_pos = left_encoder_count * ((2 * 3.14) / 3575.04);
  float right_wheel_pos = right_encoder_count * ((2 * 3.14) / 3575.04);

  float odom = (r / 2) * (left_wheel_pos + right_wheel_pos);
  return odom;
}

void motor_controller(float v, float w) {
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
  if (duty_L == 0) {  // left motor stop
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
  if (duty_R == 0) {  // right motor stop
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);
  }
  analogWrite(pwmL, abs(duty_L));
  analogWrite(pwmR, abs(duty_R));
}