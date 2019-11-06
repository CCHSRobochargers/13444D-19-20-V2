using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor LeftFront;
extern motor rightFront;
extern motor leftBack;
extern motor rightBack;
extern motor rightwm;
extern motor leftwm;
extern motor wupupe;
extern controller Controller2;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );