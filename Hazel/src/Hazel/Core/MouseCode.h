#pragma once

// From glfw3.h
constexpr int HZ_MOUSE_BUTTON_1 = 0;
constexpr int HZ_MOUSE_BUTTON_2 = 1;
constexpr int HZ_MOUSE_BUTTON_3 = 2;
constexpr int HZ_MOUSE_BUTTON_4 = 3;
constexpr int HZ_MOUSE_BUTTON_5 = 4;
constexpr int HZ_MOUSE_BUTTON_6 = 5;
constexpr int HZ_MOUSE_BUTTON_7 = 6;
constexpr int HZ_MOUSE_BUTTON_8 = 7;
constexpr int HZ_MOUSE_BUTTON_LAST = HZ_MOUSE_BUTTON_8;
constexpr int HZ_MOUSE_BUTTON_LEFT = HZ_MOUSE_BUTTON_1;
constexpr int HZ_MOUSE_BUTTON_RIGHT = HZ_MOUSE_BUTTON_2;
constexpr int HZ_MOUSE_BUTTON_MIDDLE = HZ_MOUSE_BUTTON_3;

//#include <iostream>
//
//namespace Hazel
//{
//	typedef enum class MouseCode :int
//	{
//		// From glfw3.h
//		Button1 = 0,
//		Button2 = 1,
//		Button3 = 2,
//		Button4 = 3,
//		Button5 = 4,
//		Button6 = 5,
//		Button7 = 6,
//		Button8 = 7,
//		ButtonLast = Button8,
//		ButtonLeft = Button1,
//		ButtonRight = Button2,
//		ButtonMiddle = Button3
//	}Mouse;
//
//	inline std::ostream& operator<<(std::ostream& os, MouseCode mousecode)
//	{
//		return os << static_cast<int>(mousecode);
//	}
//}
