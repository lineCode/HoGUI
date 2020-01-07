#define HOGL_IMPLEMENT
#define GRAPHICS_MATH_IMPLEMENT
#define USTRING_IMPLEMENT
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <ho_gl.h>
#include <light_array.h>
#include <gm.h>
#include "os.h"
#include "input.h"
#include "event.h"
#include "renderer/renderer_imm.h"
#include "font/font_load.h"
#include "hogui/hogui.h"
#include "hogui/immgui_input.h"
#include "hogui/immgui.h"

Font_Info font_info = {0};

int main() {
    if (!glfwInit()) {
		printf("Could not initialize GLFW\n");
		return EXIT_FAILURE;
	}

	// Create a windowed mode window and its OpenGL context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	GLFWwindow* window = glfwCreateWindow(1024, 768, "Squiggly", 0, 0);
    glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	input_init(window);
	input_set_callbacks(window);

    if (hogl_init_gl_extensions() == -1) {
		printf("Could not initialize OpenGL extensions. Make sure you have OpenGL drivers 3.3 or latest\n");
		return EXIT_FAILURE;
	}

	Font_Load_Status status = font_load(OS_DEFAULT_FONT, &font_info, 16);
	if(status != FONT_LOAD_OK) {
		printf("Could not load font %s\n", OS_DEFAULT_FONT);
		return 1;
	}

	hogui_init();

	r64 dt = 1.0/120.0;
	s32 frames = 0;
	r64 total_time = 0.0;
	r64 start_time = os_time_us();

	glClearColor(0.2f, 0.2f, 0.23f, 1.0f);

	HG_Context ctx = {0};

	bool running = true;
	char buffer[256];
	int length = 0;
	int cursor_index = 0;
	int selection_distance = 0;
    while (!glfwWindowShouldClose(window) && running) {
		glfwPollEvents();

		input_immgui(window);

		int width, height;
		window_get_size(&width, &height);

		input_immgui_set_window_size(width, height);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		vec4 colors[4] = {
			(vec4){0.0f, 0.0f, 0.0f, 1.0f},
			(vec4){0.0f, 0.0f, 0.0f, 1.0f},
			(vec4){0.4f, 0.4f, 0.4f, 1.0f},
			(vec4){0.4f, 0.4f, 0.4f, 1.0f},
		};
		Quad_2D qq = quad_new_clipped_gradient((vec2){0,0}, 300, 300, colors, clipping_rect_new(0,0, 10000.0f, 10000.0f));
		renderer_imm_quad(&qq);

		hg_window_begin(&ctx, 0, (vec2){300,200}, 300, 200, "Foo", 2);

		if(hg_do_button(&ctx, 1, "Hello World", sizeof("Hello World") -1)) {
			printf("Hello\n");
		}
		hg_do_input(&ctx, 2, buffer, 256, &length, &cursor_index, &selection_distance);
		hg_do_input(&ctx, 3, buffer, 256, &length, &cursor_index, &selection_distance);
		hg_do_input(&ctx, 4, buffer, 256, &length, &cursor_index, &selection_distance);
		hg_do_input(&ctx, 5, buffer, 256, &length, &cursor_index, &selection_distance);
		hg_window_next_column(&ctx);
		hg_do_input(&ctx, 6, buffer, 256, &length, &cursor_index, &selection_distance);
		hg_do_input(&ctx, 7, buffer, 256, &length, &cursor_index, &selection_distance);
		hg_do_input(&ctx, 8, buffer, 256, &length, &cursor_index, &selection_distance);
		hg_do_input(&ctx, 9, buffer, 256, &length, &cursor_index, &selection_distance);
		hg_do_input(&ctx, 10, buffer, 256, &length, &cursor_index, &selection_distance);

		hg_end_frame(&ctx);

		renderer_imm_enable_blending();
		glDisable(GL_DEPTH_TEST);
		renderer_imm_flush(font_info.atlas_full_id);
		renderer_imm_disable_blending();	

		// Calculate elapsed time
		// TODO(psv): make an OS level function here instead of ifdefs
		r64 elapsed_us = os_time_us() - start_time;
		r64 sleep_time = (dt * 1000000.0) - elapsed_us;
#if defined(__linux__)
		if (sleep_time > 0.0) {
			os_usleep((u64)sleep_time);
		}
#endif
		frames++;

		total_time += os_time_us() - start_time;
		if (total_time > 1000000.0) {
			printf("Rendered %d frames per second\n", frames);
			total_time = 0;
			frames = 0;
		}

		start_time = os_time_us();

        glfwSwapBuffers(window);
    }

	glfwTerminate();

    return 0;
}