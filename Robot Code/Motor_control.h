/*
 * Motor_control.h
 *
 * Created: 04/07/2018 00:02:46
 *  Author: Manuel
 */ 


#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

#define WHEEL_CIRCUMF (65 * 3.1415927)
#define TICKS_PER_CIRCUMF 16
#define DISTANCE_COEFF (WHEEL_CIRCUMF / TICKS_PER_CIRCUMF)
#define SAMPLE_TIME_IN_10MS 30
#define WHEEL_SEPARATION_LENGTH 140
#define RADS_2_DEG_CONV 57.2958
#define _90DEG_TURN_TICKS 17

#define LEFT_MOTOR_DEFAULT_DUTY_CYCLE 150
#define RIGHT_MOTOR_DEFAULT_DUTY_CYCLE 150

extern unsigned char left_motor_timer_A_reg_state;
extern unsigned char left_motor_timer_B_reg_state;
extern unsigned char right_motor_timer_A_reg_state;
extern unsigned char right_motor_timer_B_reg_state;
extern int right_ticks; //count right wheel ticks
extern int left_ticks; //count left wheel ticks
extern float distance_mm;

void init_IR_sensors();
void init_motors();
void error_analysis();
void Control();
void left_motor_forward_set_up();
void left_motor_reverse_set_up();
void left_motor_break();
void right_motor_forward_set_up();
void right_motor_reverse_set_up();
void right_motor_break();
void forward(float distance_threshold);
void reverse(int run_time_10ms);
void forward_right_turn();
void forward_left_turn();
void reverse_left_turn();
void reverse_right_turn();



#endif /* MOTOR_CONTROL_H_ */