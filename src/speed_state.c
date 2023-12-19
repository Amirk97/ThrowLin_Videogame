#include "other_functions.h"
#include "speed_state.h"
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <graphics_lib.h>
#include <amio_lib.h>
#include <conio.h>

void Speed_state_draw(void)
{
    // initializations
    SYSTEM.current_score = 0;
    SYSTEM.stick_person_x_pos = Head_center_def_x;
    // Clear everything from previous plays
    cleardevice();

    // mute the sound from last stage
    Play_sound(pitch_3rd, 0);

    //Draw static figures for speed state
    Draw_SpeedS_static();
	//Draw Dynamic figures and wait for the input to state machine
    Draw_SpeedS_dynamic();
}


void Draw_SpeedS_static(void)
{
    //Drawing the ground line
    line(0, Ground_Line_y_pos, Window_width_size, Ground_Line_y_pos, Thickness_size);

    //Paint the sky
    filled_rectangle(0, 0, Window_width_size, Ground_Line_y_pos, Sky_bkg_color);

    //paint the soil
    filled_rectangle(0, Ground_Line_y_pos, Window_width_size, Window_height_size, Soil_bkg_color);

    //Draw the sun
    filled_rectangle(0, 0, Sun_side, Sun_side, YELLOW);

    //Draw the stick person at its default position
    draw_stick_person(Head_center_def_x, Head_center_def_y, Stick_person_color, Japoline_color);

    //Draw the flag
    setcolor(Flag_color);
    line(Flag_x_position, Ground_Line_y_pos, Flag_x_position, Ground_Line_y_pos - Flag_height, Thickness_size);

    //Print Score
    print_score(Score_text_color, Score_text_color);

    // Draw the speed slider
    draw_speed_slider();

    // Draw the wind indicator
    draw_wind_indicator();
}


// This function draws the oscillating speed indicator and waits for user response
void Draw_SpeedS_dynamic(void)
{
    int Slider_V = Slider_V_def;

    while(1)
    {
        // erase indicator at  old position
        setcolor(Soil_bkg_color);
        line(SYSTEM.speed_indicator_x_pos, Speed_slider_dy +  Ground_Line_y_pos + 2*small_vertical_dis - splitter_width, SYSTEM.speed_indicator_x_pos, Speed_slider_dy + 2*small_vertical_dis +  Ground_Line_y_pos + splitter_width, Thickness_size);
        // Updating the new position for slider
        SYSTEM.speed_indicator_x_pos = SYSTEM.speed_indicator_x_pos + Slider_V * TICK;
        // Check if the new position is legal and if there should be updates for speed
        Slider_V = Check_pos_slider(Slider_V);
        // draw indicator at new position
        setcolor(Speed_slider_color);
        line(SYSTEM.speed_indicator_x_pos, Speed_slider_dy +  Ground_Line_y_pos + 2*small_vertical_dis - splitter_width, SYSTEM.speed_indicator_x_pos, Speed_slider_dy + 2*small_vertical_dis +  Ground_Line_y_pos + splitter_width, Thickness_size);

        // move the contents of the screen buffer to the display
        update_display();

        //Check there is a response from the user
        if (check_if_event())
        {
            /* wait for event  */
            wait_for_event();

            if(event_mouse_left_button_down())
            {
                Play_sound(pitch_1st, velocity_1st);
                // The corresponding score for slider position will be calculated by following function
                convert_pos_to_vel();
                SYSTEM.state = Position_s;
                break;
            }
        }

        pausefor(screen_refresh_rate); // wait 8 milliseconds
    }


}

void draw_wind_indicator(void)
{
    char  wind_label[11];

    // Generate the wind properties randomly
    SYSTEM.wind_speed = rand_number(1, MAX_wind_speed);
    SYSTEM.wind_angle = (double)(rand_number(0, MAX_Wind_angle));

    //Assign a text label to wind according to generated wind
    if(SYSTEM.wind_speed <= Wind_minor_value)
    {
        sprintf(wind_label, "Minor wind");
    }
    else
    {
        if(SYSTEM.wind_speed > Wind_minor_value && SYSTEM.wind_speed <= Wind_mild_value)
        {
            sprintf(wind_label, "Mild wind");
        }
        else
        {
            sprintf(wind_label, "Major wind");
        }
    }

    // Draw the indicator to display its angle
    setcolor(Wind_color);
    filled_circle(Wind_Indicator_x_pos, Wind_Indicator_y_pos, 4, Wind_color);
    circle(Wind_Indicator_x_pos, Wind_Indicator_y_pos, Wind_vector_coeff *MAX_wind_speed,Thickness_size);
    line(Wind_Indicator_x_pos, Wind_Indicator_y_pos, Wind_Indicator_x_pos + Wind_vector_coeff * SYSTEM.wind_speed * cos(SYSTEM.wind_angle), Wind_Indicator_y_pos + Wind_vector_coeff * SYSTEM.wind_speed * sin(SYSTEM.wind_angle), Thickness_size);

    // Print the wind speed below the indicator
    outtextxy(Wind_Indicator_x_pos - 4*small_horizontal_dis, Wind_Indicator_y_pos + 4*small_vertical_dis, wind_label);

}

// Handles what happens at boundaries of speed range
int Check_pos_slider(int Slider_V)
{
    if(SYSTEM.speed_indicator_x_pos > Speed_slider_dx + Speed_range_width )
    {
        SYSTEM.speed_indicator_x_pos = Speed_slider_dx + Speed_range_width;
        Slider_V = Slider_V * -1;
    }
    else
    {
        if(SYSTEM.speed_indicator_x_pos < Speed_slider_dx )
        {
            SYSTEM.speed_indicator_x_pos = Speed_slider_dx ;
            Slider_V = Slider_V * -1;
        }
    }

    return Slider_V;

}

// This function maps the position of slider to a corresponding velocity for stick person and projectile
void convert_pos_to_vel(void)
{

    // The mapping between the speed of stickperson and position of slider will be a linear function defined in below
    int x = (-MAX_speed_projectile/(Speed_range_width/2.0))*(abs(SYSTEM.speed_indicator_x_pos - (Speed_slider_dx +(Speed_range_width/2.0))));
     SYSTEM.initial_projectile_speed = x + MAX_speed_projectile;
     SYSTEM.stick_person_speed = SYSTEM.initial_projectile_speed/coeff_speed;
     if(SYSTEM.stick_person_speed < 1)
     {
         SYSTEM.stick_person_speed = 1;
     }
}

void print_score(int Score_color, int Highscore_color)
{
    char current_score_char[5];
    char total_score_char[5];
    char highest_score_char[5];
    char game_counter_char[2];

    // Create a string with the label in it
    sprintf(current_score_char, "%d", SYSTEM.current_score);
    sprintf(total_score_char, "%d", SYSTEM.total_score);
    sprintf(game_counter_char, "%d", SYSTEM.game_counter);
    sprintf(highest_score_char, "%d", SYSTEM.highest_score);

    // Printing the texts
    setcolor(Score_text_color);
    outtextxy(SCORE_x_position, SCORE_y_position, game_counter_char);
    outtextxy(SCORE_x_position + small_horizontal_dis, SCORE_y_position, "out of 6 throw   current score :  total score :");
    outtextxy(High_Score_X_pos, SCORE_y_position, "high score :");

    // Printing the numbers
    setcolor(Score_color);
    outtextxy(SCORE_x_position + 20*small_horizontal_dis, SCORE_y_position + 2*small_vertical_dis, current_score_char);
    outtextxy(SCORE_x_position + 33*small_horizontal_dis, SCORE_y_position + 2*small_vertical_dis, total_score_char);
    setcolor(Highscore_color);
    outtextxy(High_Score_X_pos, SCORE_y_position + 2 * small_vertical_dis, highest_score_char);

    /* move the contents of the screen buffer to the display */
    update_display();
}

void draw_stick_person(int x_position, int y_position, int person_color, int jap_color)
{
    setcolor(person_color);
    //Drawing the head
    circle(x_position, y_position, Head_radius, Thickness_size);
    //Drawing the torso
    line(x_position, y_position + Head_radius + Thickness_size, x_position, y_position + Head_radius + Thickness_size + Torso_height, Thickness_size);
    // Drawing the hands
    line(x_position - Start_dx_pos_hand, y_position + Start_dy_pos_hand, x_position + End_dx_pos_hand, y_position + End_dy_pos_hand, Thickness_size);
    // Drawing the left leg
    line(x_position, y_position +  Head_radius + Thickness_size + Torso_height, x_position - End_dx_pos_leg, Ground_Line_y_pos, Thickness_size);
    // Drawing the right leg
    line(x_position, y_position +  Head_radius + Thickness_size + Torso_height, x_position + End_dx_pos_leg, Ground_Line_y_pos, Thickness_size);

    setcolor(jap_color);
    // Drawing the trampoline
    line(x_position - Start_dx_pos_hand, y_position + Start_dy_pos_hand, x_position - Start_dx_pos_hand - Trampoline_dx, y_position + Start_dy_pos_hand + Trampoline_dy, Thickness_size);
    line(x_position - Start_dx_pos_hand, y_position + Start_dy_pos_hand, x_position - Start_dx_pos_hand + Trampoline_dx, y_position + Start_dy_pos_hand - Trampoline_dy, Thickness_size);

}

void draw_speed_slider(void)
{

    setcolor(Speed_slider_color);
    outtextxy(Speed_slider_dx, Speed_slider_dy +  Ground_Line_y_pos, "Speed :");
    // Draw the oscillating slider
    line(Speed_slider_dx, Speed_slider_dy +  Ground_Line_y_pos + 2*small_vertical_dis - splitter_width, Speed_slider_dx, Speed_slider_dy + 2*small_vertical_dis +  Ground_Line_y_pos + splitter_width, Thickness_size);
    // Draw the start of range splitter
    line(Speed_slider_dx, Speed_slider_dy +  Ground_Line_y_pos + 4*small_vertical_dis - splitter_width, Speed_slider_dx, Speed_slider_dy + 4*small_vertical_dis +  Ground_Line_y_pos + splitter_width, Thickness_size);
    // Draw the  range
    line(Speed_slider_dx, Speed_slider_dy +  Ground_Line_y_pos + 4*small_vertical_dis , Speed_slider_dx + Speed_range_width, Speed_slider_dy + 4*small_vertical_dis +  Ground_Line_y_pos, Thickness_size);
    // Draw the end of range splitter
    line(Speed_slider_dx + Speed_range_width, Speed_slider_dy + 4*small_vertical_dis +  Ground_Line_y_pos - splitter_width, Speed_slider_dx + Speed_range_width, Speed_slider_dy + 4*small_vertical_dis +  Ground_Line_y_pos + splitter_width, Thickness_size);
    // Draw the middle of range splitter
    line(Speed_slider_dx + Speed_range_width/2, Speed_slider_dy + 4*small_vertical_dis +  Ground_Line_y_pos - splitter_width, Speed_slider_dx + Speed_range_width/2, Speed_slider_dy + 4*small_vertical_dis +  Ground_Line_y_pos + splitter_width, Thickness_size);
    // Print the first slow label
    outtextxy(Speed_slider_dx, Speed_slider_dy +  Ground_Line_y_pos + 5*small_vertical_dis, "Slow");
    // Print the second slow label
    outtextxy(Speed_slider_dx + Speed_range_width, Speed_slider_dy +  Ground_Line_y_pos + 5*small_vertical_dis, "Slow");
    // Print the fast label
    outtextxy(Speed_slider_dx + Speed_range_width/2, Speed_slider_dy +  Ground_Line_y_pos + 5*small_vertical_dis, "Fast");
}

 void Play_sound(int pitch, int velocity)
{
    // Start playing a note with certain pitch and velocity
    midi_note(pitch, channel_music, velocity);

}
