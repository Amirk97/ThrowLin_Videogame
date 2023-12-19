#ifndef __OTHER_FUNCTIONS_H__
#define __OTHER_FUNCTIONS_H__

#define Sky_bkg_color       LIGHTBLUE
#define Soil_bkg_color      BROWN
#define Japoline_color      MAGENTA
#define Score_text_color    DARKGRAY
#define Speed_slider_color  LIGHTGREEN
#define Stick_person_color  WHITE
#define Flag_color          RED
#define Angle_Boundry_color WHITE
#define New_Score_color     RED
#define Prompt_Color        RED
#define Wind_color          CYAN

#define Ground_Line_y_pos   540
#define Window_width_size   1280
#define Window_height_size  960
#define Thickness_size      2
#define Sun_side            200
#define Head_center_def_x	75
#define Head_center_def_y	340
#define Head_radius	        10
#define Torso_height	    125
#define Start_dx_pos_hand	37
#define Start_dy_pos_hand	25
#define End_dx_pos_hand	    37
#define End_dy_pos_hand	    100
#define End_dx_pos_leg	    37
#define Trampoline_dx       25
#define Trampoline_dy       75
#define Flag_height         175
#define Flag_x_position     640
#define SCORE_x_position    400
#define SCORE_y_position    125
#define small_horizontal_dis 10
#define small_vertical_dis  10
#define Speed_slider_dx     12
#define Speed_slider_dy     100
#define Speed_range_width   150
#define splitter_width      5
#define Slider_V_def        9
#define TICK                0.5
#define channel_music       1
#define pitch_1st           60
#define velocity_1st        64
#define pitch_2nd           64
#define velocity_2nd        84
#define pitch_3rd           67
#define velocity_3rd        104
#define MAX_speed_projectile 68
#define Angle_dx_pos        12
#define Angle_dy_pos        100
#define Angle_side          100
#define Angle_V_def         0.1
#define Angle_Max           1.57    // corresponds to 90 degrees in radian
#define Angle_Min           0
#define Angle_Extreme_margin 0.01
#define screen_refresh_rate  8
#define Gravity_Acceleration 9.81
#define coeff_speed          3
#define High_Score_X_pos     1150
#define MAX_current_score    100
#define Total_Game_set        6
#define Wind_Indicator_x_pos  250
#define Wind_Indicator_y_pos  160
#define MAX_wind_speed        6
#define MAX_Wind_angle        25  // roughly equivalent to 8*pi
#define Wind_Indicator_length 10
#define Wind_minor_value      2
#define Wind_mild_value       4
#define Wind_vector_coeff     5


typedef enum  {
    Menu_S,
    Speed_S,
    Position_s,
    Angle_s,
    Trajectory_s,
    Score_s

}state_machine;

typedef struct  {
    int stick_person_x_pos;
    int speed_indicator_x_pos;
    int stick_person_speed;
    int initial_projectile_speed;
    double angle;
    int traj_pos_x;
    int wind_speed;
    double wind_angle;
    int failed_throw_flag;
    int highest_score;
    int total_score;
    int current_score;
    int game_counter;
    state_machine state;

}Globals;

extern Globals SYSTEM;

void Menu_display(void);
void initialize_setting(void);
void Display_scores(void);
void Print_prompts(void);
void wait_right_click_response(void);
void Draw_Trajectory(void);
void Angle_state_draw(void);
void Draw_AngleS_dynamic(void);
void Check_angle_indicator(double* Angle_speed, double* current_angle);
void Draw_AngleS_static(int Boundry_color);
void Draw_PositionS_dynamic(void);

#endif // __OTHER_FUNCTIONS_H__
