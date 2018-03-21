#include "header/CustomKeyState.h"

void CustomKeyState::RegisterKey(int key) {
	std::map<int, bool>::iterator iter = isPressed.find(key);
	if (iter != isPressed.end()) {
		isPressed[key] = false;
	}
}
void CustomKeyState::UpdateRelease(GLFWwindow* window, int key) {
	if (glfwGetKey(window, key) == GLFW_RELEASE) {
		SetPressState(key, false);
	}
}
void CustomKeyState::UpdatePress(GLFWwindow* window, int key) {
	if (glfwGetKey(window, key) == GLFW_PRESS) {
		SetPressState(key, true);
	}
}
bool CustomKeyState::GetPressState(int key) {
	return isPressed[key];
}
bool CustomKeyState::GetKeyDown(GLFWwindow* window, int key) {
	UpdateRelease(window, key);
	if (glfwGetKey(window, key) == GLFW_PRESS && !GetPressState(key)) {
		UpdatePress(window, key);
		return true;
	}
	else {
		UpdatePress(window, key);
		return false;
	}
}
bool CustomKeyState::GetKeyUp(GLFWwindow* window, int key) {
	UpdatePress(window, key);
	if (glfwGetKey(window, key) == GLFW_RELEASE && GetPressState(key)) {
		UpdateRelease(window, key);
		return true;
	}
	else {
		UpdateRelease(window, key);
		return false;
	}
}
void CustomKeyState::SetPressState(int key, bool state) {
	isPressed[key] = state;
}