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
vex::motor LiftMotorL1 = vex::motor(vex::PORT20);
vex::motor LiftMotorR1 = vex::motor(vex::PORT1);
vex::motor LiftMotorL2 = vex::motor(vex::PORT19);
vex::motor LiftMotorR2 = vex::motor(vex::PORT2);
vex::motor Feed1 = vex::motor(vex::PORT4);
vex::motor Feed2 = vex::motor(vex::PORT17);
vex::competition Competition;

int drivespeed = 75;
int liftspeed = 65;
double pi = 3.1415926535897;
int autonspeed = 20;
int pinchspeed = 50;
int feedspeed = 100;

void pulsedown() {
  Feed1.rotateFor(800, vex::rotationUnits::deg, false);
  Feed2.rotateFor(-800, vex::rotationUnits::deg, false);
  LiftMotorL1.rotateFor(-25, vex::rotationUnits::deg, false);
  LiftMotorR1.rotateFor(25, vex::rotationUnits::deg, false);
  LiftMotorL2.rotateFor(-25, vex::rotationUnits::deg, false);
  LiftMotorR2.rotateFor(25, vex::rotationUnits::deg, true);
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
    LeftMotor.rotateFor(-dist, vex::rotationUnits::deg, false);
    RightMotor.rotateFor(-dist, vex::rotationUnits::deg, true);

  } else if (lr == "Right") {

    double dist =
        (360 * 15 * pi * deg / 360) / (4 * pi); // Convert distance to degrees
    LeftMotor.rotateFor(dist, vex::rotationUnits::deg, false);
    RightMotor.rotateFor(dist, vex::rotationUnits::deg, true);
  }
}

void autonomous(void) {

  if (autoSelect == 1) {

    // pick up preload, grab two blocks (orange), and set another orange in
    // tower first
    go(20);
    turn(90, "Left");
    LiftMotorL1.rotateFor(1275, vex::rotationUnits::deg, false);
    LiftMotorR1.rotateFor(-1275, vex::rotationUnits::deg, false);
    LiftMotorL2.rotateFor(1275, vex::rotationUnits::deg, false);
    LiftMotorR2.rotateFor(-1275, vex::rotationUnits::deg, true);
    go(12);
    pulsedown();
    // go(-12);
    // LiftMotorL1.rotateFor(1475, vex::rotationUnits::deg, false);
    // LiftMotorR1.rotateFor(-1475, vex::rotationUnits::deg, false);
    // LiftMotorL2.rotateFor(1475, vex::rotationUnits::deg, false);
    // LiftMotorR2.rotateFor(-1475, vex::rotationUnits::deg, true);
  }
  if (autoSelect == 2) {
    // pick up preload, grab one block (orange), and set another orange in tower
    go(20);
    turn(90, "Right");
    LiftMotorL1.rotateFor(1275, vex::rotationUnits::deg, false);
    LiftMotorR1.rotateFor(-1275, vex::rotationUnits::deg, false);
    LiftMotorL2.rotateFor(1275, vex::rotationUnits::deg, false);
    LiftMotorR2.rotateFor(-1275, vex::rotationUnits::deg, true);
    go(12);
    pulsedown();
    // go(-12);
    // LiftMotorL1.rotateFor(1475, vex::rotationUnits::deg, false);
    // LiftMotorR1.rotateFor(-1475, vex::rotationUnits::deg, false);
    // LiftMotorL2.rotateFor(1475, vex::rotationUnits::deg, false);
    // LiftMotorR2.rotateFor(-1475, vex::rotationUnits::deg, true);
  }
  if (autoSelect == 3) {
    // pick up preload, grab two blocks (green), and set another green in tower
    // first
    go(20);
    turn(90, "Left");
    LiftMotorL1.rotateFor(-1275, vex::rotationUnits::deg, false);
    LiftMotorR1.rotateFor(1275, vex::rotationUnits::deg, false);
    LiftMotorL2.rotateFor(-1275, vex::rotationUnits::deg, false);
    LiftMotorR2.rotateFor(1275, vex::rotationUnits::deg, true);
    go(12);
    pulsedown();
    // go(-12);
    // LiftMotorL1.rotateFor(1475, vex::rotationUnits::deg, false);
    // LiftMotorR1.rotateFor(-1475, vex::rotationUnits::deg, false);
    // LiftMotorL2.rotateFor(1475, vex::rotationUnits::deg, false);
    // LiftMotorR2.rotateFor(-1475, vex::rotationUnits::deg, true);
  }
  if (autoSelect == 4) {
    // pick up preload, grab one block (green), and set another green in tower
    go(20);
    turn(90, "Right");
    LiftMotorL1.rotateFor(-1275, vex::rotationUnits::deg, false);
    LiftMotorR1.rotateFor(1275, vex::rotationUnits::deg, false);
    LiftMotorL2.rotateFor(-1275, vex::rotationUnits::deg, false);
    LiftMotorR2.rotateFor(1275, vex::rotationUnits::deg, true);
    go(12);
    pulsedown();
    // go(-12);
    // LiftMotorL1.rotateFor(1475, vex::rotationUnits::deg, false);
    // LiftMotorR1.rotateFor(-1475, vex::rotationUnits::deg, false);
    // LiftMotorL2.rotateFor(1475, vex::rotationUnits::deg, false);
    // LiftMotorR2.rotateFor(-1475, vex::rotationUnits::deg, true);
  }
}

void usercontrol(void) {
  bool buttonup;
  bool buttondown;
  bool lastbuttonup = false;
  bool lastbuttondown = false;
  bool buttonpickup;
  bool buttonsetdown;
  bool buttonstop;

  while (1) {
    buttonup = vex::controller().ButtonL1.pressing();
    buttondown = vex::controller().ButtonL2.pressing();
    buttonpickup = vex::controller().ButtonR1.pressing();
    buttonsetdown = vex::controller().ButtonR2.pressing();
    buttonstop = vex::controller().ButtonA.pressing();

    LeftMotor.spin(vex::directionType::fwd,
                   vex::controller().Axis3.position() / 1.2,
                   vex::velocityUnits::pct);
    RightMotor.spin(vex::directionType::rev,
                    vex::controller().Axis2.position() / 1.2,
                    vex::velocityUnits::pct);
    if ((buttonup == true) && (lastbuttonup == false)) {
      LiftMotorL1.spin(vex::directionType::rev, liftspeed,
                      vex::velocityUnits::pct);
      LiftMotorR1.spin(vex::directionType::fwd, liftspeed,
                      vex::velocityUnits::pct);
      LiftMotorL2.spin(vex::directionType::rev, liftspeed,
                      vex::velocityUnits::pct);
      LiftMotorR2.spin(vex::directionType::fwd, liftspeed,
                      vex::velocityUnits::pct);
    } else if ((buttondown == true) && (lastbuttondown == false)) {
      LiftMotorL1.spin(vex::directionType::fwd, liftspeed,
                      vex::velocityUnits::pct);
      LiftMotorR1.spin(vex::directionType::rev, liftspeed,
                      vex::velocityUnits::pct);
      LiftMotorL2.spin(vex::directionType::fwd, liftspeed,
                      vex::velocityUnits::pct);
      LiftMotorR2.spin(vex::directionType::rev, liftspeed,
                      vex::velocityUnits::pct);
    } else if ((buttonup == false) && (buttondown == false)) {
      LiftMotorL1.spin(vex::directionType::rev, .5, vex::velocityUnits::pct);
      LiftMotorR1.spin(vex::directionType::fwd, .5, vex::velocityUnits::pct);
      LiftMotorL2.spin(vex::directionType::rev, .5, vex::velocityUnits::pct);
      LiftMotorR2.spin(vex::directionType::fwd, .5, vex::velocityUnits::pct);
      // LiftMotorL1.stop(brake);
      // LiftMotorR1.stop(brake);
      // LiftMotorL2.stop(brake);
      // LiftMotorR2.stop(brake);

    }
    vex::controller().ButtonB.pressed(pulsedown);
    if (buttonpickup == true) {
      Feed1.spin(vex::directionType::rev, feedspeed, vex::velocityUnits::pct);
      Feed2.spin(vex::directionType::fwd, feedspeed, vex::velocityUnits::pct);
    }
    if (buttonsetdown == true) {
      Feed1.spin(vex::directionType::fwd, feedspeed / 2, vex::velocityUnits::pct);
      Feed2.spin(vex::directionType::rev, feedspeed / 2, vex::velocityUnits::pct);
    }
    if (buttonstop == true) {
      Feed1.stop(brake);
      Feed2.stop(brake);
    }
    if (vex::controller().ButtonB.pressing()) {
      buttonpickup = false;
      buttonsetdown = false;
      buttonstop = false;
    }
    // if ((buttonopen == true) && (lastbuttonopen == false)) {
    //   Pinch1.spin(vex::directionType::fwd, pinchspeed,
    //   vex::velocityUnits::pct); Pinch2.spin(vex::directionType::rev,
    //   pinchspeed, vex::velocityUnits::pct);
    // } else if ((buttonclose == true) && (lastbuttonclose == false)) {
    //   Pinch1.spin(vex::directionType::rev, pinchspeed,
    //   vex::velocityUnits::pct); Pinch2.spin(vex::directionType::fwd,
    //   pinchspeed, vex::velocityUnits::pct);
    // } else if ((buttonopen == false) && (buttonclose == false)) {
    //   Pinch1.stop(brake);
    //   Pinch2.stop(brake);
    // }
    buttonup = lastbuttonup;
    buttondown = lastbuttondown;
  }
  // Gives the processor time to not burn battery out. NEVER DELETE
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
                           // prevent wasted resources.
  }
}