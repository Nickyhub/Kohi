#include "platform.h"
#include "core/logger.h"
#include <string.h>
#include "core/input.h"
#include "containers/darray.h"
#include "core/event.h"

#if KPLATFORM_WINDOWS
#include <windows.h>
#include <windowsx.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>
#include "renderer/vulkan/vulkan_types.inl"

typedef struct platform_state {
	HINSTANCE h_instance;
	HWND hwnd;
	VkSurfaceKHR surface;
} platform_state;

static platform_state* state_ptr;

static f64 clock_frequency;
static LARGE_INTEGER start_time;

LRESULT CALLBACK win32_process_message(HWND hwnd, u32 message, WPARAM w_param, LPARAM l_param);

void clock_setup() {
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	clock_frequency = 1.0 / (f64)frequency.QuadPart;
	QueryPerformanceCounter(&start_time);
}

b8 platform_system_startup(
	u64* memory_requirement,
	void* state,
	const char* application_name,
	i32 x,
	i32 y,
	i32 width,
	i32 height) {

	*memory_requirement = sizeof(platform_state);
	if (state == 0) {
		return true;
	}
	state_ptr = state;
	state_ptr->h_instance = GetModuleHandleA(0);

	HICON icon = LoadIcon(state_ptr->h_instance, IDI_APPLICATION);
	WNDCLASSA wc;
	memset(&wc, 0, sizeof(wc));
	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = win32_process_message;
	wc.cbClsExtra = 0;
	wc.hInstance = state_ptr->h_instance;
	wc.hIcon = icon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszClassName = "kohi_window_class";

	if (!RegisterClassA(&wc)) {
		MessageBox(0, "Window registration failed", "Error", MB_ICONEXCLAMATION | MB_OK);
		return false;
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

	HWND handle = CreateWindowExA(window_ex_style, wc.lpszClassName, application_name, window_style, window_x, window_y, window_width, window_height, 0, 0, state_ptr->h_instance, 0);

	if (handle == 0) {
		MessageBoxA(NULL, "Window creation failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);

		EN_FATAL("Window creation Failed");
		return false;
	}
	else {
		state_ptr->hwnd = handle;
	}

	b32 should_activate = 1;
	i32 show_window_command_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;
	ShowWindow(state_ptr->hwnd, show_window_command_flags);

	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	clock_frequency = 1.0f / (f64)frequency.QuadPart;
	QueryPerformanceCounter(&start_time);
	return true;
}

void platform_system_shutdown(platform_state* plat_state) {
	if (state_ptr && state_ptr->hwnd) {
		DestroyWindow(state_ptr->hwnd);
		state_ptr->hwnd = 0;
	}
}

b8 platform_pump_messages(platform_state* plat_state) {
	MSG message;
	while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessageA(&message);
	}

	return true;
}

void* platform_allocate(unsigned long long size, b8 aligned) {
	void* block = malloc(size);
	return block;
}

void  platform_free(void* block, b8 aligned) {
	if (block != NULL) {
		free(block);
		block = NULL;
	}
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
	if (!clock_frequency) {
		clock_setup();
	}

	LARGE_INTEGER now_time;
	QueryPerformanceCounter(&now_time);
	return (f64)now_time.QuadPart * clock_frequency;

}

void platform_sleep(unsigned long long ms) {
	Sleep((DWORD) ms);
}

void platform_get_required_extension_names(const char*** names_darray) {
	char* extension_name = &"VK_KHR_win32_surface";
	darray_push(*names_darray, extension_name);
}

b8 platform_create_vulkan_surface(platform_state* plat_state, vulkan_context* context) {
	if (!state_ptr) {
		return false;
	}


	VkWin32SurfaceCreateInfoKHR create_info = { VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
	create_info.hinstance = state_ptr->h_instance;
	create_info.hwnd = state_ptr->hwnd;

	VkResult result = vkCreateWin32SurfaceKHR(context->instance, &create_info, context->allocator, &state_ptr->surface);

	if (result != VK_SUCCESS) {
		EN_FATAL("Vulkan surface creation failed.");
		return false;
	}
	context->surface = state_ptr->surface;
	return true;
}


LRESULT CALLBACK win32_process_message(HWND hwnd, u32 message, WPARAM w_param, LPARAM l_param) {

	switch (message) {
	case WM_ERASEBKGND:
		return 1;
	case WM_CLOSE:
		event_context data = {0};
		event_fire(EVENT_CODE_APPLICATION_QUIT, 0, data);
		return true;
		
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE: {
		RECT r;
		GetClientRect(hwnd, &r);
		u32 width = r.right - r.left;
		u32 height = r.bottom - r.top;

		event_context context;
		context.data.u16[0] = (u16)width;
		context.data.u16[1] = (u16)height;
		event_fire(EVENT_CODE_RESIZED, 0, context);
	} break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP: {
		b8 pressed = (message == WM_KEYDOWN || message == WM_SYSKEYDOWN);
		keys key = (u16)w_param;

		//Alt key
		if (w_param == VK_MENU) {
			if (GetKeyState(VK_RMENU) & 0x8000) {
				key = KEY_RALT;
			}
			else if (GetKeyState(VK_LMENU) & 0x8000) {
				key = KEY_LALT;
			}
		}
		else if (w_param == VK_SHIFT) {
			if (GetKeyState(VK_RSHIFT) & 0x8000) {
				key = KEY_RSHIFT;
			}
			else if (GetKeyState(VK_LSHIFT) & 0x8000) {
				key = KEY_LSHIFT;
			}
		}
		else if (w_param == VK_CONTROL) {
			if (GetKeyState(VK_RCONTROL) & 0x8000) {
				key = KEY_RCONTROL;
			}
			else if (GetKeyState(VK_LCONTROL) & 0x8000) {
				key = KEY_LCONTROL;
			}
		}

		input_process_key(key, pressed);

	} break;

	case WM_MOUSEMOVE: {
		i32 x_position = GET_X_LPARAM(l_param);
		i32 y_position = GET_Y_LPARAM(l_param);

		input_process_mouse_move(x_position, y_position);
	}
	case WM_MOUSEWHEEL: {
		i32 z_delta = GET_WHEEL_DELTA_WPARAM(w_param);
		if (z_delta != 0) {
			z_delta = (z_delta < 0) ? -1 : 1;
		}
		input_process_mouse_wheel((i8)z_delta);
	} break;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP: {
		b8 pressed = message == WM_LBUTTONDOWN || message == WM_RBUTTONDOWN || message == WM_MBUTTONDOWN;
		buttons mouse_button = BUTTON_MAX_BUTTONS;
		switch (message)
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
			mouse_button = BUTTON_LEFT;
			break;
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
			mouse_button = BUTTON_MIDDLE;
			break;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			mouse_button = BUTTON_RIGHT;
			break;
		}
		if (mouse_button != BUTTON_MAX_BUTTONS) {
			input_process_button(mouse_button, pressed);
		}
	} break;

	}
	return DefWindowProc(hwnd, message, w_param, l_param);
}

#endif