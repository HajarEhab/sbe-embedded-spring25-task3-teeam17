//
// Created by hagar on 24/04/2025.
//
#ifndef KEYPAD_H
#define KEYPAD_H

// Initializes the GPIO pins connected to the keypad.
void Keypad_Init(void);

// Manages keypad scanning logic.
// This function is called in the main loop to detect key presses.
void Keypad_Manage(void);

// Returns the character of the key that was last pressed.
char Keypad_GetKey(void);

// This is a callback function I define in my main.
// It gets triggered when a key is successfully detected.
extern void KeypadCallout_KeyPressNotification(void);

#endif // KEYPAD_H

