/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       lingboz                                                   */
/*    Created:      9/12/2025, 4:01:33 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain       Brain;

// VEX V5 devices
vex::controller Controller1 = vex::controller(vex::controllerType::primary);

// Drive motors (4 motor drivetrain)
vex::motor LeftFront = vex::motor(vex::PORT1, vex::ratio18_1, false);
vex::motor LeftBack = vex::motor(vex::PORT2, vex::ratio18_1, false);
vex::motor RightFront = vex::motor(vex::PORT3, vex::ratio18_1, true);
vex::motor RightBack = vex::motor(vex::PORT4, vex::ratio18_1, true);

// Claw motor
vex::motor ClawMotor = vex::motor(vex::PORT5, vex::ratio36_1, false);

// Arm motor
vex::motor ArmMotor = vex::motor(vex::PORT6, vex::ratio36_1, false);

// Drivetrain
vex::drivetrain Drivetrain = vex::drivetrain(LeftFront, RightFront, LeftBack, RightBack, 319.19, 295, 40, vex::mm, 1);

void openClaw() {
    ClawMotor.spin(vex::forward, 100, vex::percent);
    vex::wait(500, vex::msec);
    ClawMotor.stop();
}

void closeClaw() {
    ClawMotor.spin(vex::reverse, 100, vex::percent);
    vex::wait(500, vex::msec);
    ClawMotor.stop();
}

void raiseArm() {
    ArmMotor.spin(vex::forward, 50, vex::percent);
    vex::wait(1000, vex::msec);
    ArmMotor.stop();
}

void lowerArm() {
    ArmMotor.spin(vex::reverse, 50, vex::percent);
    vex::wait(1000, vex::msec);
    ArmMotor.stop();
}

void autonomous() {
    Drivetrain.driveFor(vex::forward, 24, vex::inches);
    
    openClaw();
    vex::wait(500, vex::msec);
    
    Drivetrain.driveFor(vex::reverse, 12, vex::inches);
    
    closeClaw();
    
    raiseArm();
    
    Drivetrain.turnFor(vex::right, 90, vex::degrees);
    
    Drivetrain.driveFor(vex::forward, 36, vex::inches);
    
    lowerArm();
    
    openClaw();
    
    Drivetrain.driveFor(vex::reverse, 12, vex::inches);
}

void driverControl() {
    while (true) {
        int leftSpeed = Controller1.Axis3.position() + Controller1.Axis1.position();
        int rightSpeed = Controller1.Axis3.position() - Controller1.Axis1.position();
        
        LeftFront.spin(vex::forward, leftSpeed, vex::percent);
        LeftBack.spin(vex::forward, leftSpeed, vex::percent);
        RightFront.spin(vex::forward, rightSpeed, vex::percent);
        RightBack.spin(vex::forward, rightSpeed, vex::percent);
        
        if (Controller1.ButtonR1.pressing()) {
            ClawMotor.spin(vex::forward, 100, vex::percent);
        } else if (Controller1.ButtonR2.pressing()) {
            ClawMotor.spin(vex::reverse, 100, vex::percent);
        } else {
            ClawMotor.stop();
        }
        
        if (Controller1.ButtonL1.pressing()) {
            ArmMotor.spin(vex::forward, 50, vex::percent);
        } else if (Controller1.ButtonL2.pressing()) {
            ArmMotor.spin(vex::reverse, 50, vex::percent);
        } else {
            ArmMotor.stop();
        }
        
        vex::wait(20, vex::msec);
    }
}

int main() {
    vex::competition Competition;
    
    Competition.autonomous(autonomous);
    Competition.drivercontrol(driverControl);
    
    Brain.Screen.printAt(10, 50, "ClawBot Ready!");
    
    while (true) {
        this_thread::sleep_for(10);
    }
}
