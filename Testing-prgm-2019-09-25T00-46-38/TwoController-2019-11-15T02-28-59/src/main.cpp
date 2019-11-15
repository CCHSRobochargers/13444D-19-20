// To complete the VEXcode V5 Text project upgrade process, please follow the
// steps below.

// 1. You can use the Robot Configuration window to recreate your V5 devices
//   - including any motors, sensors, 3-wire devices, and controllers.

// 2. All previous code located in main.cpp has now been commented out. You
//   will need to migrate this code to the new "int main" structure created
//   below and keep in mind any new device names you may have set from the
//   Robot Configuration window.

// If you would like to go back to your original project, a complete backup
// of your original (pre-upgraded) project was created in a backup folder
// inside of this project's folder.

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "field-250.h"
#include "vex.h"

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\CCHSRoboChargers                                 */
/*    Created:      Tue Sep 17 2019                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// ---- END VEXCODE CONFIGURED DEVICES ----

using namespace vex;

// define your global instances of motors and other devices here
vex::motor LeftMotor = vex::motor(vex::PORT3);
vex::motor RightMotor = vex::motor(vex::PORT18);
vex::motor LiftMotorL1 = vex::motor(vex::PORT20, true);
vex::motor LiftMotorR1 = vex::motor(vex::PORT1);
vex::motor LiftMotorL2 = vex::motor(vex::PORT19, true);
vex::motor LiftMotorR2 = vex::motor(vex::PORT2);
vex::motor Pinch1 = vex::motor(vex::PORT4);
vex::motor Pinch2 = vex::motor(vex::PORT15, true);
vex::motor_group Pinch = vex::motor_group(Pinch1, Pinch2);
vex::motor_group Lift =
    vex::motor_group(LiftMotorL1, LiftMotorR1, LiftMotorL2, LiftMotorR2);

vex::controller Controller1 = vex::controller(vex::controllerType::primary);
vex::controller Controller2 = vex::controller(vex::controllerType::partner);
// vex::motor Feed1 = vex::motor(vex::PORT4);
// vex::motor Feed2 = vex::motor(vex::PORT16);
vex::competition Competition;

int drivespeed = 75;
int liftspeed = 65;
double pi = 3.1415926535897;
int autonspeed = 20;
int pinchspeed = 50;
int feedspeed = 100;

// void pulsedown() {
//   Feed1.rotateFor(540, vex::rotationUnits::deg, false);
//   Feed2.rotateFor(-540, vex::rotationUnits::deg, false);
//   LiftMotorL1.rotateFor(200, vex::rotationUnits::deg, false);
//   LiftMotorR1.rotateFor(-200, vex::rotationUnits::deg, false);
//   LiftMotorL2.rotateFor(200, vex::rotationUnits::deg, false);
//   LiftMotorR2.rotateFor(-200, vex::rotationUnits::deg, false);
// }
bool open = false;
void R1() {
  if (open == true) {
    Pinch.setVelocity(pinchspeed, vex::velocityUnits::pct);
    Pinch.rotateFor(50, vex::rotationUnits::deg, true);
    open = false;
  }
}

void R2() {
  if (open == false) {
    Pinch.setVelocity(pinchspeed, vex::velocityUnits::pct);
    Pinch.rotateFor(-50, vex::rotationUnits::deg, true);
    open = true;
  }
}

void stop() {
  Pinch.stop(brake);
}

#define FIELD_GRAPHIC_WIDTH 250
#define FIELD_GRAPHIC_HEIGHT 250
#define BRAIN_SCREEN_WIDTH 480
#define BRAIN_SCREEN_HEIGHT 272
// Center on the X axis
#define FIELD_LOC_X ((BRAIN_SCREEN_WIDTH / 2) - (FIELD_GRAPHIC_WIDTH / 2))
// Put on the bottom on the Y axis
#define FIELD_LOC_Y ((BRAIN_SCREEN_HEIGHT - 25) - FIELD_GRAPHIC_HEIGHT)

typedef struct {
  const char *label;
  int x;
  int y;
  int width;
  int height;
} hotspotType;

// The (0,0) origin of the screen is top left
#define NUM_AUTO 5
hotspotType autoHotspots[NUM_AUTO] = {
    {"   None   ", (FIELD_GRAPHIC_HEIGHT / 2) - 25,
     (FIELD_GRAPHIC_WIDTH / 2) - 25, 50, 50},
    {" Red Big  ", 0, 0, 50, 50},
    {"Red Small ", 0, FIELD_GRAPHIC_WIDTH - 50, 50, 50},
    {" Blue Big ", FIELD_GRAPHIC_HEIGHT - 50, 0, 50, 50},
    {"Blue Small", FIELD_GRAPHIC_HEIGHT - 50, FIELD_GRAPHIC_WIDTH - 50, 50, 50},
};

// Which autonomous was selected (defaults to "none")
int autoSelect = 0;

// Button pressed (or not) math
bool isPressed(int idx, int X, int Y) {
  if ((X >= autoHotspots[idx].x) &&
      (X <= autoHotspots[idx].x + autoHotspots[idx].width) &&
      (Y >= autoHotspots[idx].y) &&
      (Y <= autoHotspots[idx].y + autoHotspots[idx].width)) {
    return true;
  } else {
    return false;
  }
}

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  int fingerX;
  int fingerY;
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...

  // "Accept" button
  Brain.Screen.drawCircle(55, FIELD_LOC_Y + (FIELD_GRAPHIC_HEIGHT / 2), 50,
                          color::green);

  // Set the screen origin to where we want the field grapic. Makes the math
  // easier.
  Brain.Screen.setOrigin(FIELD_LOC_X, FIELD_LOC_Y);

  Brain.Screen.drawImageFromBuffer(field_250_png, 0, 0, field_250_png_len);
  // Draw squares for the buttons
  for (int j = 0; j < NUM_AUTO; j++) {
    Brain.Screen.drawRectangle(autoHotspots[j].x, autoHotspots[j].y,
                               autoHotspots[j].width, autoHotspots[j].height,
                               color::transparent);
  }

  while (true) {
    if (Brain.Screen.pressing()) {
      fingerX = Brain.Screen.xPosition();
      fingerY = Brain.Screen.yPosition();
      // Press to the left of the field graphic to accept
      if (fingerX < -10) {
        break;
      }
      for (int j = 0; j < NUM_AUTO; j++) {
        if (isPressed(j, fingerX, fingerY)) {
          autoSelect = j;
          // Need to reset the origin to print the label under the green button
          Brain.Screen.setOrigin(0, 0);
          Brain.Screen.setCursor(11, 1);
          Brain.Screen.print(autoHotspots[j].label);
          Brain.Screen.setOrigin(FIELD_LOC_X, FIELD_LOC_Y);
        }
      }
    }
    wait(20, msec);
  }

  // Redraw with only the selected routine
  Brain.Screen.drawImageFromBuffer(field_250_png, 0, 0, field_250_png_len);
  Brain.Screen.drawRectangle(
      autoHotspots[autoSelect].x, autoHotspots[autoSelect].y,
      autoHotspots[autoSelect].width, autoHotspots[autoSelect].height,
      color::transparent);

  Brain.Screen.setOrigin(0, 0);
  // Clear the temporary label
  Brain.Screen.setCursor(11, 1);
  Brain.Screen.print("          ");
  // Print the selected autonomous on the button
  Brain.Screen.setCursor(2, 1);
  Brain.Screen.print(autoHotspots[autoSelect].label);
}

void go(double inch) {

  LeftMotor.setVelocity(autonspeed, vex::velocityUnits::pct);
  RightMotor.setVelocity(autonspeed, vex::velocityUnits::pct);
  double dist = (360 * inch) / (4 * pi); // Convert distance to degrees
  LeftMotor.rotateFor(dist, vex::rotationUnits::deg, false);
  RightMotor.rotateFor(-dist, vex::rotationUnits::deg, true);
}

void turn(int deg, std::string lr) {

  LeftMotor.setVelocity(autonspeed, vex::velocityUnits::pct);
  RightMotor.setVelocity(autonspeed, vex::velocityUnits::pct);

  if (lr == "Left") {

    double dist =
        (360 * 15 * pi * deg / 360) / (4 * pi); // Convert distance to degrees
    LeftMotor.rotateFor(dist, vex::rotationUnits::deg, false);
    RightMotor.rotateFor(dist, vex::rotationUnits::deg, true);

  } else if (lr == "Right") {

    double dist =
        (360 * 15 * pi * deg / 360) / (4 * pi); // Convert distance to degrees
    LeftMotor.rotateFor(-dist, vex::rotationUnits::deg, false);
    RightMotor.rotateFor(-dist, vex::rotationUnits::deg, true);
  }
}

void autonomous(void) {

  Lift.setTimeout(5, vex::timeUnits::sec);

  if (autoSelect == 0) {
    go(-24);
    go(-24);
  }
  if (autoSelect == 1) {
    // Red Big
    Pinch.rotateFor(-80, vex::rotationUnits::deg, true);
    go(6);
    Pinch.rotateFor(50, vex::rotationUnits::deg, true);
    Lift.rotateFor(100, vex::rotationUnits::deg, true);
    go(14);
    turn(95, "Left");
    Lift.rotateFor(1200, vex::rotationUnits::deg, true);
    go(18);
    Pinch.rotateFor(-50, vex::rotationUnits::deg, false);
    go(-29);
    Lift.rotateFor(-1200, vex::rotationUnits::deg, true);
  }
  if (autoSelect == 2) {

    // Red Small
    Pinch.rotateFor(-80, vex::rotationUnits::deg, true);
    go(6);
    Pinch.rotateFor(50, vex::rotationUnits::deg, true);
    Lift.rotateFor(100, vex::rotationUnits::deg, true);
    go(14);
    turn(95, "Right");
    Lift.rotateFor(1200, vex::rotationUnits::deg, true);
    go(18);
    Pinch.rotateFor(-50, vex::rotationUnits::deg, false);
    go(-29);
    Lift.rotateFor(-1200, vex::rotationUnits::deg, true);
  }
  if (autoSelect == 3) {

    // Blue Big
    Pinch.rotateFor(-80, vex::rotationUnits::deg, true);
    go(6);
    Pinch.rotateFor(50, vex::rotationUnits::deg, true);
    Lift.rotateFor(100, vex::rotationUnits::deg, true);
    go(14);
    turn(95, "Right");
    Lift.rotateFor(1200, vex::rotationUnits::deg, true);
    go(18);
    Pinch.rotateFor(-50, vex::rotationUnits::deg, false);
    go(-29);
    Lift.rotateFor(-1200, vex::rotationUnits::deg, true);
  }
  if (autoSelect == 4) {

    // Blue Small
    Pinch.rotateFor(-80, vex::rotationUnits::deg, true);
    go(6);
    Pinch.rotateFor(50, vex::rotationUnits::deg, true);
    Lift.rotateFor(100, vex::rotationUnits::deg, true);
    go(14);
    turn(95, "Right");
    Lift.rotateFor(1200, vex::rotationUnits::deg, true);
    go(18);
    Pinch.rotateFor(-50, vex::rotationUnits::deg, false);
    go(-29);
    Lift.rotateFor(-1200, vex::rotationUnits::deg, true);
  }
}

void usercontrol(void) {
  // bool buttonup;
  // bool buttondown;
  // bool lastbuttonup = false;
  // bool lastbuttondown = false;
  // bool buttonpickup;
  // bool buttonsetdown;
  // bool buttonstop;
  bool buttonopen;
  bool buttonclose;
  bool lastbuttonopen = false;
  bool lastbuttonclose = false;
  // Controller2.ButtonR1.pressed(R1);
  // Controller2.ButtonR2.pressed(R2);
  Controller2.ButtonA.pressed(stop);

  while (1) {
    // buttonup = Controller2.ButtonL1.pressing();
    // buttondown = Controller2.ButtonL2.pressing();
    // buttonpickup = Controller2.ButtonR1.pressing();
    // buttonsetdown = Controller2.ButtonR2.pressing();
    // buttonstop = Controller2.ButtonA.pressing();
    buttonopen = Controller2.ButtonR2.pressing();
    buttonclose = Controller2.ButtonR1.pressing();
    if ((buttonopen == true) && (lastbuttonopen == false)) {
      Pinch.spin(vex::directionType::fwd, pinchspeed, vex::velocityUnits::pct);
    }
    if ((buttonclose == true) &&(lastbuttonclose == false)) {
      Pinch.spin(vex::directionType::rev, pinchspeed, vex::velocityUnits::pct);
    }
    LeftMotor.spin(vex::directionType::fwd, Controller1.Axis3.position() / 1.2,
                   vex::velocityUnits::pct);
    RightMotor.spin(vex::directionType::rev, Controller1.Axis2.position() / 1.2,
                    vex::velocityUnits::pct);
    Lift.spin(vex::directionType::fwd, Controller2.Axis3.position() / 1.6,
              vex::velocityUnits::pct);
    lastbuttonopen = buttonopen;
    lastbuttonclose = buttonclose;
  }
  // Gives the processor time to not burn battery out. NEVER DELETE. Like EVER
  this_thread::sleep_for(20);
}

int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (1) {
    vex::task::sleep(100); // Sleep the task for a short amount of time to
                           // prevent wasted resources. i.e. battery and processing 
  }
}