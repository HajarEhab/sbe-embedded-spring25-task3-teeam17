
#include "RCC.h"
#include "Gpio.h"

#include "Keypad.h"

    // CONFIGURATION
#define SEGMENT_PORT        GPIO_A // All 7-segment display pins connected to GPIOA


#define SEG_A_PIN            5 // Segment A connected to PA5
#define SEG_B_PIN            6 // Segment B connected to PA6
#define SEG_C_PIN            7 // Segment C connected to PA7
#define SEG_D_PIN            8 // Segment D connected to PA8
#define SEG_E_PIN            9 // Segment E connected to PA9
#define SEG_F_PIN           10 // Segment F connected to PA10
#define SEG_G_PIN           11 // Segment G connected to PA11

#define SEGMENT_DIGITS       7   // Total 7 segments

// Array holding the segment pin order: A, B, C, D, E, F, G
    const uint8 segmentPins[SEGMENT_DIGITS] = {
        SEG_A_PIN, SEG_B_PIN, SEG_C_PIN,
     SEG_D_PIN, SEG_E_PIN, SEG_F_PIN, SEG_G_PIN
    };
// Lookup table defining which segments to turn ON for digits 0-9
const uint8 SEGMENT_LOOKUP[10] = {
    0x3F, // 0: A B C D E F
    0x30, // 1: F E
    0x6D, // 2: A F G C D
    0x79, // 3: A F G E D
    0x72, // 4: B G F E
    0x5B, // 5: A B G E D
    0x5F, // 6: A B G C D
    0x31, // 7: A F E
    0x7F, // 8: A B C D E F G
    0x7B  // 9: A B G E D
    };

//FUNCTION TO INITIALIZE SEGMENT PINS
void Segment_Init(void) {
    for (int i = 0; i < SEGMENT_DIGITS; i++) {
        Gpio_Init(SEGMENT_PORT, segmentPins[i], GPIO_OUTPUT, GPIO_PUSH_PULL); // Configure each pin as output
        Gpio_WritePin(SEGMENT_PORT, segmentPins[i], LOW);  // Turn OFF all segments initially
    }
}
//FUNCTION TO DISPLAY A DIGIT
void Segment_DisplayDigit(char keyChar) {
    if (keyChar >= '0' && keyChar <= '9') {// Check if input character is a valid digit
        uint8 pattern = SEGMENT_LOOKUP[keyChar - '0'];  // Get pattern for corresponding digit
        for (int i = 0; i < SEGMENT_DIGITS; i++) {
            uint8 bit = (pattern >> i) & 0x01;  // Extract bit for segment i
            Gpio_WritePin(SEGMENT_PORT, segmentPins[i], bit);
        }
    }
}
//FUNCTION CALLED WHEN KEY IS PRESSED
void KeypadCallout_KeyPressNotification(void) {
    char key = Keypad_GetKey();  // Get the latest key pressed from the keypad
    Segment_DisplayDigit(key); // Display corresponding number on the 7-segment
}

int main(void) {
    /* Local Variables */
    Rcc_Init();          // Initialize RCC system

    Rcc_Enable(RCC_GPIOA); // Enable clock to GPIOA (used for 7-segment)
    Rcc_Enable(RCC_GPIOB);    // Enable clock to GPIOB (used for keypad rows)



    Keypad_Init();  // Initialize keypad GPIO
    Segment_Init();  // Initialize segment display GPIO

    while (1) {
        Keypad_Manage();  // Scan for key press
        KeypadCallout_KeyPressNotification();  // Display key if pressed

    }
}

