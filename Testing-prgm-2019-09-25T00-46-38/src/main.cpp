/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\CCHSRoboChargers                                 */
/*    Created:      Tue Sep 17 2019                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain Brain;

// define your global instances of motors and other devices here
vex::motor LeftMotor = vex::motor(vex::PORT1);
vex::motor RightMotor = vex::motor(vex::PORT2);
vex::motor LiftMotor1 = vex::motor(vex::PORT3);
vex::motor LiftMotor2 = vex::motor(vex::PORT4);

int drivespeed = 75;
int liftspeed = 25;

int main() {
  while (1) {
    LeftMotor.spin(vex::directionType::fwd, vex::controller().Axis3.position()/1.5,
                   vex::velocityUnits::pct);
    RightMotor.spin(vex::directionType::rev, vex::controller().Axis2.position()/1.5,
                    vex::velocityUnits::pct);
    if (vex::controller().ButtonUp.pressing()) {
      LiftMotor1.spin(vex::directionType::fwd, liftspeed,
                      vex::velocityUnits::pct);
      LiftMotor2.spin(vex::directionType::fwd, liftspeed,
                      vex::velocityUnits::pct);
    } else if (vex::controller().ButtonDown.pressing()) {
      LiftMotor1.spin(vex::directionType::rev, liftspeed,
                      vex::velocityUnits::pct);
      LiftMotor2.spin(vex::directionType::rev, liftspeed,
                      vex::velocityUnits::pct);
    }
    // Allow other tasks to run
    this_thread::sleep_for(20);
  }
}