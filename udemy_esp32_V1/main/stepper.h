#ifndef MAIN_STEPPER_H_
#define MAIN_STEPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

void move_stepper();
void init_base();

_Bool based;
int blind_actual_state;
int LED_R;
int LED_G;
int LED_B;
#ifdef __cplusplus
}
#endif

#endif
