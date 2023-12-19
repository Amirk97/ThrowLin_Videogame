
#ifndef  __SPEED_STATE_H__
#define  __SPEED_STATE_H__

void Speed_state_draw(void);
void Draw_SpeedS_static(void);
void Draw_SpeedS_dynamic(void);
void draw_wind_indicator(void);
int Check_pos_slider(int Slider_V);
void convert_pos_to_vel(void);
void print_score(int Score_color, int Highscore_color);
void draw_stick_person(int x_position, int y_position, int person_color, int jap_color);
void draw_speed_slider(void);
void Play_sound(int pitch, int velocity);

#endif // __SPEED_STATE_H__


