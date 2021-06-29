#include "platform.h"
#include "core/logger.h"

#if KPLATFORM_WINDOWS
#include <windows.h>
#include <windowsx.h>

typedef struct internal_state {
	HINSTANCE h_instance;
	HWND hwnd;
} internal_state;

static f64 clock_frequency;
static LARGE_INTEGER start_time;

LRESULT CALLBACK win32_process_message(HWND hwnd, u32 message, WPARAM w_param, LPARAM l_param);

b8 platform_startup(platform_state* plat_state, const char* app_name, int x, int y, int width, int height) {
	plat_state->internal_state = malloc(sizeof(internal_state));
	internal_state* state = (internal_state*)plat_state->internal_state;
	state->h_instance = GetModuleHandleA(0);

	HICON icon = LoadIcon(state->h_instance, IDI_APPLICATION);
	WNDCLASSA wc;
	memset(&wc, 0, sizeof(wc));
	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = win32_process_message;
	wc.cbClsExtra = 0;
	wc.hInstance = state->h_instance;
	wc.hIcon = icon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszClassName = "kohi_window_class";

	if (!RegisterClassA(&wc)) {
		MessageBox(0, "Window registration failede", "Error", MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}

	u32 window_x = x;
	u32 window_y = y;
	u32 window_width = width;
	u32 window_height = height;

	u32 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
	u32 window_ex_style = WS_EX_APPWINDOW;

	window_style |= WS_MAXIMIZEBOX;
	window_style |= WS_MINIMIZEBOX;
	window_style |= WS_THICKFRAME;

	RECT border_rect = { 0, 0, 0, 0 };
	AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

	// In this case, the border rectangle is negative.
	window_x += border_rect.left;
	window_y += border_rect.top;

	// Grow by the size of the OS border.
	window_width += border_rect.right - border_rect.left;
	window_height += border_rect.bottom - border_rect.top;

	HWND handle = CreateWindowExA(window_ex_style, wc.lpszClassName, app_name, window_style, window_x, window_y, window_width, window_height, 0, 0, state->h_instance, 0);

	if (handle == 0) {
		MessageBoxA(NULL, "Window creation failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);

		EN_FATAL("Window creation Failed");
		return FALSE;
	}
	else {
		state->hwnd = handle;
	}

	b32 should_activate = 1;
	i32 show_window_command_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;
	ShowWindow(state->hwnd, show_window_command_flags);

	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	clock_frequency = 1.0f / (f64)frequency.QuadPart;
	QueryPerformanceCounter(&start_time);
	return TRUE;
}

void platform_shutdown(platform_state* plat_state) {
	internal_state* state = (internal_state*)plat_state->internal_state;

	if (state->hwnd) {
		DestroyWindow(state->hwnd);
		state->hwnd = 0;
	}
}

b8 platform_pump_messages(platform_state* plat_state) {
	MSG message;
	while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessageA(&message);
	}

	return TRUE;
}

void* platform_allocate(unsigned long long size, b8 aligned) {
	return malloc(size);
}

void  platform_free(void* block, b8 aligned) {
	free(block);
}

void* platform_zero_memory(void* block, unsigned long long size) {
	return memset(block, 0, size);
}

void* platform_copy_memory(void* dest, const void* source, unsigned long long size) {
	return memcpy(dest, source, size);
}

void* platform_set_memory(void* dest, int value, unsigned long long size) {
	return memset(dest, value, size);
}

void platform_console_write(const char* message, char colour) {
	HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	static u8 levels[6] = { 64, 4, 6, 2, 1, 8 };
	SetConsoleTextAttribute(console_handle, levels[colour]);

	OutputDebugStringA(message);
	u64 length = strlen(message);
	LPDWORD number_written = 0;
	WriteConsoleA(console_handle, message, (DWORD)length, number_written, 0);
}

void platform_console_write_error(const char* message, char colour) {
	HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
	static u8 levels[6] = { 64, 4, 6, 2, 1, 8 };
	SetConsoleTextAttribute(console_handle, levels[colour]);

	OutputDebugStringA(message);
	u64 length = strlen(message);
	LPDWORD number_written = 0;
	WriteConsoleA(console_handle, message, (DWORD)length, number_written, 0);
}

f64 platform_get_absolute_time() {
	LARGE_INTEGER now_time;
	QueryPerformanceCounter(&now_time);
	return (f64)now_time.QuadPart * clock_frequency;
}

void platform_sleep(unsigned long long ms) {
	Sleep((DWORD) ms);
}


LRESULT CALLBACK win32_process_message(HWND hwnd, u32 message, WPARAM w_param, LPARAM l_param) {

	switch (message) {
	case WM_ERASEBKGND:
		return 1;
	case WM_CLOSE:
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE: {
		//RECT r;
		//GetClientRect(hwnd, &r);
		//u32 width = r.right - r.left;
		//u32 height = r.bottom - r.top;

	} break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP: {

	} break;

	case WM_MOUSEMOVE: {
		i32 x_position = GET_X_LPARAM(l_param);
		i32 y_position = GET_Y_LPARAM(l_param);
	}
	case WM_MOUSEWHEEL: {
		i32 z_delta = GET_WHEEL_DELTA_WPARAM(w_param);
		if (z_delta != 0) {
			z_delta = (z_delta < 0) ? -1 : 1;
		}
	} break;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP: {
		b8 pressed = message == WM_LBUTTONDOWN || message == WM_RBUTTONDOWN || message == WM_MBUTTONDOWN;
	} break;

	}
	return DefWindowProc(hwnd, message, w_param, l_param);
}

#endif