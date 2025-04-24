//
// Created by hagar on 24/04/2025.
//
#include "Keypad.h"
#include "Gpio.h"

//  CONFIGURATION
// Define the ports used for keypad rows and columns
#define KEYPAD_ROW_PORT      GPIO_B
#define KEYPAD_COL_PORT      GPIO_A

#define ROWS 4
#define COLS 3
// Define the specific GPIO pins used for each row
#define KEYPAD_ROW0_PIN      4
#define KEYPAD_ROW1_PIN      5
#define KEYPAD_ROW2_PIN      6
#define KEYPAD_ROW3_PIN      7
// Define the specific GPIO pins used for each column
#define KEYPAD_COL0_PIN      0
#define KEYPAD_COL1_PIN      1
#define KEYPAD_COL2_PIN      2


//STATIC VARIABLES
// Arrays holding the pin numbers for rows
static const uint8 rowPins[ROWS] = {
    KEYPAD_ROW0_PIN, KEYPAD_ROW1_PIN, KEYPAD_ROW2_PIN, KEYPAD_ROW3_PIN
};
// Arrays holding the pin numbers for columns
static const uint8 colPins[COLS] = {
    KEYPAD_COL0_PIN, KEYPAD_COL1_PIN, KEYPAD_COL2_PIN
};
// 2D array representing the keypad layout( Each row and column intersection maps to a key value).
static const char keypadMap[ROWS][COLS] = {
    {'3', '2', '1'},
    {'6', '5', '4'},
    {'9', '8', '7'},
    {'0', '0', '0'}
};
// Stores the last detected key
static char storedKey = 0;
// Flag indicating whether a key has been pressed
static uint8 keyPressed = 0;
// Initializes GPIO pins for keypad input/output
void Keypad_Init(void) {
    // Initialize row pins as output and set them HIGH (inactive state)
    for (int i = 0; i < ROWS; i++) {
        Gpio_Init(KEYPAD_ROW_PORT, rowPins[i], GPIO_OUTPUT, GPIO_PUSH_PULL);
        Gpio_WritePin(KEYPAD_ROW_PORT, rowPins[i], HIGH);//(inactive state)
    }
    // Initialize column pins as input with pull-up resistors
    for (int i = 0; i < COLS; i++) {
        Gpio_Init(KEYPAD_COL_PORT, colPins[i], GPIO_INPUT, GPIO_PULL_UP);
    }
}
// Scans the keypad and stores the detected key
void Keypad_Manage(void) {
    static uint8 previousState = 0;// Tracks the previous scan state to avoid multiple detections

    for (int row = 0; row < ROWS; row++) {
        Gpio_WritePin(KEYPAD_ROW_PORT, rowPins[row], LOW);   // Activate one row by setting it LOW

        for (int col = 0; col < COLS; col++) {
            // If a key is pressed, the corresponding column will go LOW
            if (Gpio_ReadPin(KEYPAD_COL_PORT, colPins[col]) == LOW) {
                if (!previousState) {  // Only register once when key is pressed
                    storedKey = keypadMap[row][col]; // Get the key character from the map
                    keyPressed = 1;   // Mark key as pressed
                    previousState = 1;    // Update previous state to prevent re-trigger
                }

                Gpio_WritePin(KEYPAD_ROW_PORT, rowPins[row], HIGH);  // Reset the row pin
                return; // Exit the function as soon as a key is detected
            }
        }

        Gpio_WritePin(KEYPAD_ROW_PORT, rowPins[row], HIGH); // Reset the row after scanning
    }

    previousState = 0;   // No key detected, reset the state
}
// Returns the last detected key and clears the keyPressed flag
char Keypad_GetKey(void) {
    if (keyPressed) {
        keyPressed = 0;// Reset the key press flag
        return storedKey; // Return the detected key
    }
    return 0;// No key pressed
}

