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
vex::competition Competition;

int drivespeed = 75;
int liftspeed = 25;
double pi = 3.1415926535897;
int autonspeed = 25;
  
void pre_auton(void) {

}

void go(double inch) {

  LeftMotor.setVelocity(autonspeed, vex::velocityUnits::pct);
  RightMotor.setVelocity(autonspeed, vex::velocityUnits::pct);
  // double dist = 4 * pi * 900 * inch;
  double dist = (360 * inch) / (4 * pi); // Convert distance to degrees
  LeftMotor.rotateFor(dist, vex::rotationUnits::deg, false);
  RightMotor.rotateFor(-dist, vex::rotationUnits::deg, false);

}

void turn(int deg, std::string lr) {

  LeftMotor.setVelocity(autonspeed, vex::velocityUnits::pct);
  RightMotor.setVelocity(autonspeed, vex::velocityUnits::pct);
  if (lr == "Left") {

    double dist = (360 * 15 * pi * deg / 360) / (4 * pi); // Convert distance to degrees
    LeftMotor.rotateFor(-dist, vex::rotationUnits::deg, false);
    RightMotor.rotateFor(-dist, vex::rotationUnits::deg, false);

  } 
  else if (lr == "Right") {

    double dist = (360 * 15 * pi * deg / 360) / (4 * pi); // Convert distance to degrees
    LeftMotor.rotateFor(dist, vex::rotationUnits::deg, false);
    RightMotor.rotateFor(dist, vex::rotationUnits::deg, false);

  }

}

void autonomous(void) {
    
  go(72);
  turn(90, "Left");
    
}

void usercontrol(void) { 

  while (1) {
    LeftMotor.spin(vex::directionType::fwd,
                  vex::controller().Axis3.position() / 1.5,
                  vex::velocityUnits::pct);
    RightMotor.spin(vex::directionType::rev,
                      vex::controller().Axis2.position() / 1.5,
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
  }
  // Gives the processor time to not burn battery out. NEVER DELETE
  this_thread::sleep_for(20);
}

int main() {
    //Set up callbacks for autonomous and driver control periods.
    Competition.autonomous( autonomous );
    Competition.drivercontrol( usercontrol );
    
    //Run the pre-autonomous function. 
    pre_auton();
       
    //Prevent main from exiting with an infinite loop.                        
    while(1) {
      vex::task::sleep(100);//Sleep the task for a short amount of time to prevent wasted resources.
    }    
}