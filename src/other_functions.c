#include "other_functions.h"
#include "speed_state.h"
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <graphics_lib.h>
#include <amio_lib.h>
#include <conio.h>

void Menu_display(void)
{
    const char* Begining_Text = "Welcome to ThrowLin! \n \n"
    "In this game, you should try to throw the javelin as far as you can to score the best. \n"
    "To start first you should try to choose the speed with which the runner will be moving, \n \n"

    "First, Try to stop the oscillating slider by holding down the left button on your mouse, the closer to middle the slider is, the higher your speed will be. \n"
    "Stopping the slider at a fast speed will give your athlete more power to throw the javelin, \n \n"

    "Then try to stop the runner before he passes the limit flag in the middle by removing your finger form the button, \n"
    "Closer to middle line you stop the runner, your chances for scoring better will be increased! \n \n"

    "At the end you should choose the angle of your throw, \n"
    "Do so by stopping the oscillating indicator at your desired angle by doing a left click, consider the wind direction! \n \n"

    "After your shot you can watch the result of your throw and then by doing a right click you can start throwing again. \n \n"

    "press any key to start the game";


    printf(Begining_Text);

    getch();

    // This function will initialize the graphical and UI functions
    initialize_setting();
    // state transition
    SYSTEM.state = Speed_S;

}

void initialize_setting(void)
{
    // Open a graphics window
    initwindow(Window_width_size, Window_height_size);
    // sets up the font used by outtextxy
    initfont();
    // allow mouse operations
	initmouse();
	// allow keyboard operations
    initkeyboard();
	// initialize the midi functions
    midi_start();
    // create an event queue
    create_event_queue();
    // register display, mouse and keyboard as event sources
    reg_display_events();
    reg_mouse_events();
    reg_keyboard_events();
}

void Display_scores(void)
{
    // Clearing the previous scores
    print_score(Sky_bkg_color, Sky_bkg_color);

    // calculating the current score
    if(SYSTEM.failed_throw_flag == 1)
    {
        SYSTEM.failed_throw_flag = 0;
        SYSTEM.current_score = 0;
    }
    else
    {
        if((SYSTEM.traj_pos_x - Flag_x_position) > 0)
        {
            SYSTEM.current_score = (SYSTEM.traj_pos_x - Flag_x_position) * MAX_current_score / (Window_width_size - Flag_x_position);
        }
        else
        {
            SYSTEM.current_score = 0;
        }
    }

    // calculating the total score
    SYSTEM.total_score += SYSTEM.current_score;
    print_score(New_Score_color, Score_text_color);

    //State transition rule check and calculating high score
    SYSTEM.game_counter ++;
    if(SYSTEM.game_counter <= Total_Game_set)
    {
        // Wait for left click from the user
        wait_right_click_response();
        // reset the score for the next round
        SYSTEM.state = Speed_S;
    }
    else
    {
        if(SYSTEM.total_score > SYSTEM.highest_score)
        {
            // Clearing the previous scores
            print_score(Sky_bkg_color, Sky_bkg_color);
            // printing the new score
            SYSTEM.highest_score = SYSTEM.total_score;
            print_score(New_Score_color, New_Score_color);
        }

        // mute the sound from last stage
       // Play_sound(pitch_3rd, 0);

        //ask for decision from user and wait for the response to decide  about next state
        Print_prompts();
        while(true)
        {
            //Check there is a response from the user
            if (check_if_event())
            {
                /* wait for event */
                wait_for_event();
                if (event_key_down())
                {
                    if (event_key('R'))
                    {
                        SYSTEM.game_counter = 1;
                        SYSTEM.total_score = 0;
                        SYSTEM.state = Speed_S;
                        break;
                    }

                    if (event_key('Q'))
                    {

                        SYSTEM.state = Menu_S;
                        /* remove the display */
                        closegraph();

                        /* close down all midi functions */
                        midi_close();
                        break;
                    }
                }
            }
        }
    }
}

void Print_prompts(void)
{
    setcolor(RED);
    outtextxy(small_horizontal_dis, Window_height_size - 2*small_vertical_dis, "Press R to restart the game");
    outtextxy(Window_width_size/2, Window_height_size - 2*small_vertical_dis, "Press Q to close the game");
    /* move the contents of the screen buffer to the display */
    update_display();
}

void wait_right_click_response(void)
{
    while(true)
    {
        //Check there is a response from the user
        if (check_if_event())
        {
            // wait for event
            wait_for_event();

            if(event_mouse_right_button_down())
            {
                break;
            }
        }
    }

}
// This function draws the trajectory of projectile
void Draw_Trajectory(void)
{
    double time;
    double vel_x, vel_y;
    int initial_pos_x = SYSTEM.stick_person_x_pos - Start_dx_pos_hand + Trampoline_dx;
    int initial_pos_y = Head_center_def_y + Start_dy_pos_hand - Trampoline_dy;
    int traj_pos_y;
    SYSTEM.traj_pos_x = initial_pos_x;

    vel_x = SYSTEM.initial_projectile_speed * cos(SYSTEM.angle) + SYSTEM.wind_speed * cos(SYSTEM.wind_angle);
    vel_y = SYSTEM.initial_projectile_speed * sin(SYSTEM.angle) + SYSTEM.wind_speed * sin(SYSTEM.wind_angle);

    // Move the start point for drawing the trajectory to the end of japolin in hand of the person
    moveto( initial_pos_x, initial_pos_y);

    //The following while loop draws the trajectory based on physics of falling objects in gravity
    setcolor(RED);
    do
    {
        SYSTEM.traj_pos_x++;
        time = (SYSTEM.traj_pos_x-initial_pos_x) / vel_x;
        traj_pos_y = (int)(initial_pos_y - (vel_y*time) + ((Gravity_Acceleration*time*time)/2));
        lineto(SYSTEM.traj_pos_x, traj_pos_y, Thickness_size);
    }
    while(traj_pos_y <= Ground_Line_y_pos );

    /* move the contents of the screen buffer to the display */
    update_display();

    SYSTEM.state = Score_s;

}

void Angle_state_draw(void)
{
    // Drawing the boundaries for angle indicator with its boundaries
    Draw_AngleS_static(Angle_Boundry_color);

    // Draw the oscillating angle indicator
    Draw_AngleS_dynamic();
}

void Draw_AngleS_dynamic(void)
{
    double Angle_V = Angle_V_def;


    while(1)
    {

        // erase indicator at old position
        setcolor(Sky_bkg_color);
        line(Angle_dx_pos, Ground_Line_y_pos - Angle_dy_pos , (int)(Angle_dx_pos + Angle_side * cos(SYSTEM.angle)), (int)(Ground_Line_y_pos - Angle_dy_pos - Angle_side * sin(SYSTEM.angle)), Thickness_size);
        // make sure boundaries doesn't get erased
        Draw_AngleS_static(Angle_Boundry_color);

        // Updating the new position for slider
        SYSTEM.angle = SYSTEM.angle + Angle_V * TICK;
        // Check if the new position is legal and if there should be updates for speed
        Check_angle_indicator(&Angle_V, &SYSTEM.angle);
        // draw indicator at new position
        setcolor(Angle_Boundry_color);
        line(Angle_dx_pos, Ground_Line_y_pos - Angle_dy_pos , (int)(Angle_dx_pos + Angle_side * cos(SYSTEM.angle)), (int)(Ground_Line_y_pos - Angle_dy_pos - Angle_side * sin(SYSTEM.angle)), Thickness_size);

        //redraw the stickperson to make sure it doesnt get interfered by angle indicator
        draw_stick_person(SYSTEM.stick_person_x_pos, Head_center_def_y, Stick_person_color, Japoline_color);

        // move the contents of the screen buffer to the display
        update_display();

        //Check there is a response from the user
        if (check_if_event())
        {
            // wait for event
            wait_for_event();

            if(event_mouse_left_button_down())
            {
                Play_sound(pitch_2nd, 0);
                Play_sound(pitch_3rd, velocity_3rd);
                SYSTEM.state = Trajectory_s;
                break;
            }
        }

        pausefor(screen_refresh_rate); // wait 8 milliseconds
    }

}

void Check_angle_indicator(double* Angle_speed, double* current_angle)
{
    if(*current_angle > Angle_Max)
    {
        *current_angle = Angle_Max;
        *Angle_speed *= -1;
    }
    else
    {
        if(*current_angle < Angle_Min)
        {
           *current_angle = Angle_Min;
           *Angle_speed *= -1;
        }
    }
}

void Draw_AngleS_static(int Boundry_color)
{
    setcolor(Boundry_color);
    // Draw the vertical line
    line(Angle_dx_pos, Ground_Line_y_pos - Angle_dy_pos, Angle_dx_pos, Ground_Line_y_pos - Angle_dy_pos - Angle_side, Thickness_size);

    // Draw the horizontal line
    line(Angle_dx_pos, Ground_Line_y_pos - Angle_dy_pos, Angle_dx_pos + Angle_side, Ground_Line_y_pos - Angle_dy_pos, Thickness_size);

    //Printing the Label
    outtextxy(Angle_dx_pos , Ground_Line_y_pos - Angle_dy_pos + small_vertical_dis, "Angle");

    // move the contents of the screen buffer to the display
    update_display();
}

//Draws the dynamic object in this stage which is the stickperson
void Draw_PositionS_dynamic(void)
{

    while(1)
    {
        if(SYSTEM.stick_person_x_pos + End_dx_pos_leg < Flag_x_position) // Check if the stick person has passed the flag
        {
            // erase stick person at old position
            draw_stick_person(SYSTEM.stick_person_x_pos, Head_center_def_y, Sky_bkg_color, Sky_bkg_color);
            // Updating the new position for stick person
            SYSTEM.stick_person_x_pos = SYSTEM.stick_person_x_pos + SYSTEM.stick_person_speed * TICK;
            // draw indicator at new position
            draw_stick_person(SYSTEM.stick_person_x_pos, Head_center_def_y, Stick_person_color, Japoline_color);
            // move the contents of the screen buffer to the display
            update_display();
        }
        else
        {
            SYSTEM.failed_throw_flag = 1;
            // turn off the the first note
            Play_sound(pitch_1st, 0);
        }

        //Check there is a response from the user
        if (check_if_event())
        {
            // wait for event
            wait_for_event();

            if(event_mouse_left_button_down())
            {
                if(SYSTEM.failed_throw_flag == 0)
                {
                    //Play_sound(pitch_2nd, velocity_2nd);
                    SYSTEM.state = Angle_s;
                    // turn off the the first note
                    Play_sound(pitch_1st, 0);
                    Play_sound(pitch_2nd, velocity_2nd);
                    break;
                }
                else
                {
                    //Play_sound(pitch_2nd, velocity_2nd);
                    SYSTEM.state = Score_s;
                    break;
                }

            }
        }
        pausefor(screen_refresh_rate); // wait 8 milliseconds
    }

}


