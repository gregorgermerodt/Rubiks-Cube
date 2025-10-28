#pragma once

struct GLFWwindow;
class KeyboardObserver
{
public:
	KeyboardObserver(); // needed for the stl-map
	KeyboardObserver(GLFWwindow* window, int key);
	void Update(); // asks for states of the keyboard and updates the boolean variables

	bool m_isDown;
	bool m_wasPressed;
	bool m_wasReleased;

private:
	GLFWwindow* m_window;
	int m_key;
};