/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// LeftFront            motor         2               
// rightFront           motor         3               
// leftBack             motor         4               
// rightBack            motor         5               
// rightwm              motor         9               
// leftwm               motor         7               
// wupupe               motor         8               
// Controller2          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "field_250.h"  // Field graphic (250x250 pixels PNG)

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

motor_group LeftDrive(LeftFront, leftBack);
motor_group RightDrive(rightFront, rightBack);
drivetrain Drivetrain =
    drivetrain(LeftDrive, RightDrive, 4.0 * M_PI, 0, 9.0, inches);

typedef enum { Stop, Up, Down } squidtype;

typedef enum { Red, Blue } allianceSelType;
const char *allianceText[] = {"Red", "Blue"};
allianceSelType allianceSelect = Red;

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
#define NUM_AUTO 2
hotspotType autoHotspots[NUM_AUTO] = {
  {"   YEET   ", (FIELD_GRAPHIC_HEIGHT / 2) - 25, (FIELD_GRAPHIC_WIDTH / 2) - 25, 50, 50},
  {" Regular  ", 0, 0, 50, 50},
};

// Which autonomous was selected (defaults to "none")
int autoSelect = 0;

// Button pressed (or not) math
bool isPressed(int idx, int X, int Y)
{
  if ((X >= autoHotspots[idx].x) && (X <= autoHotspots[idx].x + autoHotspots[idx].width) &&
      (Y >= autoHotspots[idx].y) && (Y <= autoHotspots[idx].y + autoHotspots[idx].width)) {
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

  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...

  // "Accept" button
  Brain.Screen.drawCircle(55, FIELD_LOC_Y + (FIELD_GRAPHIC_HEIGHT / 2), 50, color::green);

  // Set the screen origin to where we want the field grapic. Makes the math easier.
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
  Brain.Screen.drawRectangle(autoHotspots[autoSelect].x, autoHotspots[autoSelect].y,
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

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) 
{
  if (autoSelect == 0)
  {
    Drivetrain.stop();
  }

  if (autoSelect == 1)
  {
    rightwm.setVelocity(40, percent);
    rightwm.spin(forward);
    leftwm.setVelocity(40, percent);
    leftwm.spin(forward);
    Drivetrain.driveFor(24.0 * 1, inches);
    rightwm.setVelocity(40, percent);
    rightwm.spin(reverse);
    leftwm.setVelocity(40, percent);
    leftwm.spin(reverse);
    Drivetrain.driveFor(-24.0 * 1, inches);
  }
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  squidtype squidwheels = Stop;
  while (1) {

    int LeftDrivespeed = Controller1.Axis3.position() * 0.50;
    int RightDrivespeed = Controller1.Axis2.position() * 0.50;
    LeftDrive.setVelocity(LeftDrivespeed, percent);
    LeftDrive.spin(forward);
    RightDrive.setVelocity(RightDrivespeed, percent);
    RightDrive.spin(forward);

    if (Controller2.ButtonUp.pressing()) {
      squidwheels = Up;
    }

    else if (Controller2.ButtonDown.pressing()) {
      squidwheels = Down;
    }

    else if (Controller2.ButtonLeft.pressing()) {
      squidwheels = Stop;
    }

    if (squidwheels == Up) {
      rightwm.setVelocity(40, percent);
      rightwm.spin(forward);
      leftwm.setVelocity(40, percent);
      leftwm.spin(forward);

    }

    else if (squidwheels == Down) {
      rightwm.setVelocity(40, percent);
      rightwm.spin(reverse);
      leftwm.setVelocity(40, percent);
      leftwm.spin(reverse);

    }

    else if (squidwheels == Stop) {
      rightwm.stop();
      leftwm.stop();
    }

    // control for the wupupe

    if (Controller2.ButtonR1.pressing())
    {
      wupupe.setVelocity(30, percent);
      wupupe.spin(forward);
    }
    
    else if (Controller2.ButtonR2.pressing())
    {
      wupupe.setVelocity(30, percent);
      wupupe.spin(reverse);
    }

    else 
    {
      wupupe.stop();
    }

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
