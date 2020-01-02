#pragma once
#include <gm.h>
#include <GLFW/glfw3.h>

#define MOUSE_LEFT_BUTTON GLFW_MOUSE_BUTTON_1
#define MOUSE_RIGHT_BUTTON GLFW_MOUSE_BUTTON_2
#define MOUSE_MIDDLE_BUTTON GLFW_MOUSE_BUTTON_3

#define INPUT_SELECTING_BORDER_LEFT (1 << 0)
#define INPUT_SELECTING_BORDER_RIGHT (1 << 1)
#define INPUT_SELECTING_BORDER_TOP (1 << 2)
#define INPUT_SELECTING_BORDER_BOTTOM (1 << 3)

void input_immgui();
void input_immgui_set_window_size(int width, int height);
bool input_is_key_down(u32 key);
bool input_is_mouse_button_down(int button);
int  input_key_went_down(u32 key);
int  input_key_went_up(u32 key);
int  input_mouse_button_went_down(int button, int* x, int* y);
int  input_mouse_button_went_up(int button, int* x, int* y);
vec2 input_mouse_position();
bool input_inside(vec2 p, vec4 clipping);