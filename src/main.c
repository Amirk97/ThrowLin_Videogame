#include "other_functions.h"
#include "speed_state.h"

Globals SYSTEM;

int main(void)
{

    // The initialization for global variables
    initialize_global_struct();

    // Enter to the main state machine
    run_state_machine();

	return 0;
}

void initialize_global_struct(void)
{
     SYSTEM.stick_person_x_pos = Head_center_def_x; // Holds the final position of stick person
     SYSTEM.speed_indicator_x_pos = Speed_slider_dx;
     SYSTEM.stick_person_speed;
     SYSTEM.initial_projectile_speed;
     SYSTEM.angle = 0.01;    // The angle for indicator, in radians
     SYSTEM.traj_pos_x;      // The variable which gonna hold the final landing position for projectile
     SYSTEM.wind_speed;
     SYSTEM.wind_angle;
     SYSTEM.failed_throw_flag = 0;
     SYSTEM.highest_score = 0;
     SYSTEM.total_score = 0;
     SYSTEM.current_score = 0;
     SYSTEM.game_counter = 1;
     SYSTEM.state = Menu_S;
}

void run_state_machine(void)
{
    while(1)
    {
        switch(SYSTEM.state)
        {
            case Menu_S:
                Menu_display();
                break;

            case Speed_S :
                Speed_state_draw();
                break;

            case Position_s:
                Draw_PositionS_dynamic();
                break;

            case Angle_s:
                Angle_state_draw();
                break;

            case Trajectory_s:
                Draw_Trajectory();
                break;

            case Score_s:
                Display_scores();
                break;
        }
    }
}

