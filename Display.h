// #ifndef Gamer_h
// #define Gamer_h

// #include "mbed.h"

// class Display {
// public:
// 	// Constructor
// 	Display();

// 	// Keywords
// 	#define UP 0
// 	#define LEFT 1
// 	#define RIGHT 2
// 	#define DOWN 3
// 	#define START 4
	
// 	// Setup
// 	void begin();
// 	void update();
	
// 	// Inputs
// 	bool isPressed(uint8_t input);
// 	bool isHeld(uint8_t input);

// 	// Outputs
// 	void setRefreshRate(uint16_t refreshRate);
// 	void updateDisplay();
// 	void allOn();
// 	void clear();
// 	void printImage(uint8_t* img);
// 	void printImage(uint8_t* img, int x, int y);
// 	void setLED(bool value);
// 	void toggleLED();
// 	void playTone(int note);
// 	void stopTone();
// 	void printString(string string);
// 	void appendColumn(uint8_t* screen, uint8_t col);
// 	void showScore(int n);
	
// 	// Variables
// 	uint8_t display[8][8];
// 	uint8_t pulseCount;
// 	uint8_t buzzerCount;
// 	uint8_t nextRow;
// 	uint8_t currentRow;
// 	uint8_t counter;
// 	uint8_t image[8];
	
// 	// Routines attached to the timer's ISR
// 	void isrRoutine();
	
// private:
	
// 	// Keywords
// 	#define CLK1 6
// 	#define DAT 8
// 	#define LAT 9
// 	#define CLK2 7
// 	#define DAT 8
// 	#define LAT 9
// 	#define OE 10
// 	#define LED 13
// 	#define BUZZER 2
// 	#define RX 5
// 	#define TX 4
// 	#define DEBOUNCETIME 50
	
// 	// Variables
// 	uint16_t _refreshRate;
// 	bool buttonFlags[6];
// 	unsigned long buttonLastPressed[6];
// 	int lastInputState[6];
// 	uint16_t ldrThreshold;
	
// 	// Functions
// 	void setupLetters();
// 	void writeToDriver(uint8_t dataOut);
// 	void writeToRegister(uint8_t dataOut);
// 	void checkSerial();
// 	void checkInputs();
// 	void updateRow();
// 	int currentInputState[6];
// 	bool tog;

// 	// Numbers and letters for printString
// 	#define LETEND B10101010
// 	const static uint8_t allLetters[85][9];
// 	const static uint8_t allNumbers[10][8];
// };

// #endif