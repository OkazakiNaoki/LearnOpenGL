#ifndef CUSTOMKEYSTATE_H
#define CUSTOMKEYSTATE_H

#include <GLFW/glfw3.h>
#include <map>

class CustomKeyState {
public:
	CustomKeyState() {}
	void RegisterKey(int key);
	void UpdateRelease(GLFWwindow* window, int key);
	void UpdatePress(GLFWwindow* window, int key);
	bool GetPressState(int key);
	bool GetKeyDown(GLFWwindow* window, int key);
	bool GetKeyUp(GLFWwindow* window, int key);
	
private:
	std::map<int, bool> isPressed;
	void SetPressState(int key, bool state);
};
#endif