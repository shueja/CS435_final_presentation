// #include "Display.h"
// #include "mbed.h"

// int count;
// bool toggleVal = false;
// int split = 0;
// bool ir = false;
// bool irTog = false;
// bool toneIsPlaying = false;
// bool playTog = false;
// bool toneStopped = false;
// char prevChar;

// Display *thisDisplay = NULL;

// // // Interrupt service routine.
// // ISR(TIMER2_COMPB_vect)
// // {
// //   if(ir == 1) {
// //     thisDisplay->isrRoutine();
// //   }

// // }

// // // Interrupt service routine for simultaneous IR & buzzer.
// // ISR(TIMER2_COMPA_vect)
// // {
// //   if(ir == 0) {
// //     if(toneIsPlaying) {
// //       if (toggleVal) {
// //         PORTD |= _BV(PORTD2);
// //         toggleVal = 0;
// //         if(split % 10 == 0 ){
// //           thisDisplay->isrRoutine();
// //         }
// //         split++;
// //       }
// //       else {
// //         PORTD &= ~_BV(PORTD2);
// //         toggleVal = 1;
// //         if(split % 10== 0 ) {
// //           thisDisplay->isrRoutine();
// //         }
// //         split++;
// //       }
// //       split++;
// //     }
// //     else {
// //       PORTD &= ~_BV(PORTD2);
// //       if(split % 10 == 0 ) {
// //         thisDisplay->isrRoutine();
// //       }
// //     }
// //     split++;
// //   }
// // }

// /**
//   Constructor. Also initiates software serial for IR
// 	communiation.
//  */
// //#ifdef MULTIPLAYER
// //Display::Display() : _serial(5,4) {
// //}
// //#else
// Display::Display() {
// }
// //#endif
// /**
//   Plays a tone on the buzzer.
//   @param note the desired frequency
//  */
// void Display::playTone(int note)
// {
//   TIMSK2 &= (1<<OCIE2A);

//   if(playTog == false){
//     toneStopped = false;
//     irTog = true;
//     ir = 0;
//     noInterrupts();

//     TCCR2A = ~(_BV(COM20b1)) | ~(_BV(WGM21)) | ~(_BV(WGM20));
//     TCCR2B = ~(_BV(WGM22)) | ~(_BV(CS22));
//     TCCR2B = (TCCR2B & 0b0000000) | 0;
//     TIMSK2 = ~(_BV(OCIE2B));

//     OCR2A = 0;
//     TIMSK2 = 0;
//     OCR2B = 0;
//     TCCR2A = 0;
//     TCCR2B = 0;
//     TCNT2  = 0;
//     TCCR2A |= (1 << WGM21);
//     OCR2A = 180;
//     TCCR2B |= (1 << CS21);
//     TIMSK2 |= (1 << OCIE2A);
//     playTog = true;
//   }
//   toneIsPlaying = true;
//   OCR2A = note;
//   interrupts();
// }

// /**
//   Stops any frequency on the buzzer pin.
//  */
// void Display::stopTone()
// {
//   if(toneStopped == false){
//     TIMSK2 &= (1<<OCIE1A);
//     toneStopped = true;
//     toneIsPlaying = false;
//     playTog = false;
//     OCR2A = 180;
//     digitalWrite(2,LOW);
//     split = 0;
//     toggleVal = 0;
//     irTog = false;
//     irBegin();
//   }

//   OCR1A = 14;
//   toneIsPlaying = false;
// }

// /**
//   Stops the 38KHz wave for the IR transmitter.
//  */
// void Display::irEnd()
// {
//   irTog = false;
// }

// /**
//   Creates a 38KHz wave for the IR transmitter.
//  */
// void Display::irBegin()
// {
//   TIMSK2 &= ~(1<<OCIE2A);
//   TIMSK2 &= (1<<OCIE1B);

//   if(irTog == false) {

//     irTog = true;

//     noInterrupts();
//     TCCR2A |=  ~(_BV(WGM21));
//     TCCR2B |= ~(_BV(CS21));
//     TIMSK2 |=  ~(_BV(OCIE2A));
//     TCCR2A = 0;
//     TCCR2B = 0;
//     TCNT2  = 0;
//     TCCR2A = _BV(COM20b1) | _BV(WGM21) | _BV(WGM20);
//     TCCR2B = _BV(WGM22) | _BV(CS22);
//     OCR2A = 51;
//     OCR2B = 26;
//     TCCR2B = (TCCR2B & 0b00111000) | 0x2;
//     TIMSK2 = _BV(OCIE2B);
//     interrupts();

//     ir = 1;
//   }
// }

// /**
//   Initialises the library, starts timers, and sets pins. 
//  */
// void Display::begin()
// {
//   ::thisDisplay = this;
	
//   _refreshRate = 50;
//   ldrThreshold = 300;

//   // Setup outputs
//   pinMode(3, OUTPUT);
//   for(int i=6; i<=10; i++) pinMode(i, OUTPUT);
//   pinMode(2, OUTPUT);
//   pinMode(13, OUTPUT);
//   pinMode(5,INPUT);

//   // Setup inputs
//   DDRC = 0b00000;
//   PORTC = 0b11111;

//   //Analog Read 16 divisor
//   ADCSRA &= ~(1 << ADPS2);

//   irBegin();
// }

// // Inputs --------------------------------

// /**
//   Checks if a button has been pressed (unique press)
//   @param input the button to check
//   @return The state of the button
//  */
// bool Display::isPressed(uint8_t input)
// {
//   if(buttonFlags[input]) {
//     buttonFlags[input] = 0;
//     return true;
//   }
//   else return false;
// }

// /**
//   Checks if the button is currently held down.
//   @param input the button to check
//   @return the state of the button
//  */
// bool Display::isHeld(uint8_t input)
// {
//   bool result = (PINC & (1<<input)) >> input;
//   return !result;
// }
// /**
//   Returns the current value of the LDR
//   @return LDR value between 0 and 1023
//  */
// int Display::ldrValue()
// {
//   return analogRead(LDR);
// }
// /**
//   Changes the "pressed" event threshold for the LDR.
//   @param threshold the difference in light that triggers the event
//  */
// void Display::setldrThreshold(uint16_t threshold)
// {
//   ldrThreshold = threshold;
// }


// // Outputs -------------------------------

// /**
//   Sets the display's refresh rate. 1 = 1 row per timer cycle. 10 = 1 row every 10 timer cycles.
//   @param refreshRate the display's refresh rate modulo against the timer.
//  */
// void Display::setRefreshRate(uint16_t refreshRate)
// {
//   _refreshRate = refreshRate;
// }

// /**
//   Burns the display[][] array onto the display. Call this when you're done changing pixels in your game.
//  */
// void Display::updateDisplay()
// {
//   uint8_t newImage[8];
//   for(int j=0; j<8; j++) {
//     newImage[j] = 0x00;
//     for(int i=0; i<8; i++) {
//       newImage[j] <<= 1;
//       newImage[j] |= display[i][j];
//     }
//   }
//   if(newImage != image) {
//     for(int i=0; i<8; i++) image[i] = newImage[i];
//   }
// }

// /**
//   Turns on all the pixels on the display.
//  */
// void Display::allOn()
// {
//   for(int j=0; j<8; j++) {
//     for(int i=0; i<8; i++) display[i][j] = 1;
//   }
//   updateDisplay();
// }

// /**
//   Clears all pixels off the display.
//  */
// void Display::clear()
// {
//   for(int j=0; j<8; j++) {
//     for(int i=0; i<8; i++) display[i][j] = 0;
//   }
//   updateDisplay();
// }

// /**
//   Prints an 8 uint8_t array onto the display. 
//   @param img the 8 uint8_t array to display
//  */
// void Display::printImage(uint8_t* img)
// {
//   for(int j=0; j<8; j++) {
//     for(int i=0; i<8; i++) {
//       display[i][j] = (img[j] & (1 << (7-i))) != 0;
//     }
//   }
//   updateDisplay();
// }

// /**
//   Prints an 8 uint8_t array onto the display at an X/Y position.
//   @param img the 8 uint8_t array to display
//   @param x the image's x position
//   @param y the image's y position
//  */
// void Display::printImage(uint8_t* img, int x, int y)
// {
//   for(int j=0; j<8; j++) {
//     for(int i=0; i<8; i++) {
//       if(i+x >= 0 && i+x < 8 && j+y >= 0 && j+y < 8) {
//         display[i+x][j+y] = (img[j] & (1 << (7-i))) != 0;
//       }
//     }
//   }
//   updateDisplay();
// }

// /**
//   Sets the value of the red LED.
//   @param value the LED's boolean value
//  */
// void Display::setLED(bool value)
// {
//   digitalWrite(LED, value);
// }

// /**
//   Toggles the value of the red LED.
//  */
// void Display::toggleLED()
// {
//   digitalWrite(LED, !digitalRead(LED));
// }

// /**
//   Displays the next row on the display to achieve row scanning.
//  */
// void Display::updateRow()
// {
//   if(counter==8) {
//     counter = 0;
//     currentRow = 0x80;
//   }
//   writeToRegister(0);
//   writeToDriver(image[counter]);
//   writeToRegister(currentRow);
//   currentRow >>= 1;
//   counter++;
// }

// /**
//   Writes a uint8_t to the TLC5916 LED driver (cathodes).
//   @param dataOut the uint8_t to write to the driver
//  */
// void Display::writeToDriver(uint8_t dataOut)
// {
//   // Output enable HIGH
//   PORTB |= _BV(PORTB2);

//   // Send uint8_t to driver
//   for(int x=0; x<=7; x++) {
//     PORTD &= ~_BV(PORTD6);
//     if(((dataOut & (1<<x)) >> x)) PORTB |= _BV(PORT0b0);
//     else PORTB &= ~_BV(PORT0b0);
//     PORTD |= _BV(PORTD6);
//   }

//   PORTD &= ~_BV(PORTD6);
//   PORTB |= _BV(PORT0b1);
//   PORTB &= ~_BV(PORT0b1);
//   PORTB &= ~_BV(PORTB2);
// }

// /**
//   Writes a uint8_t to the MIC5891 shift register (anodes).
//   @param dataOut the uint8_t to write to the register
//  */
// void Display::writeToRegister(uint8_t dataOut)
// {
//   for(int y=0; y<=7; y++) {
//     if((dataOut & (1<<y)) >> y) PORTB |= _BV(PORT0b0);
//     else PORTB &= ~_BV(PORT0b0);
//     PORTD |= _BV(PORTD7);
//     PORTD &= ~_BV(PORTD7);
//   }

//   PORTB |= _BV(PORT0b1);
//   PORTB &= ~_BV(PORT0b1);
// }

// /**
//   Checks if inputs have changed to track button and LDR events.
//  */
// void Display::checkInputs()
// {
//   for(int i=0; i<6; i++) {
//     if(i != 5) {
//       currentInputState[i] = (PINC & (1<<i)) >> i;
//       if(currentInputState[i] != lastInputState[i]) {
//         if(currentInputState[i] == 0) {
//           buttonFlags[i] = 1;
//         }
//       }
//       lastInputState[i] = currentInputState[i];
//     }
//     else {
//       currentInputState[i] = analogRead(LDR);
//       if(currentInputState[i] - lastInputState[i] >= ldrThreshold) buttonFlags[i] = 1;
//       lastInputState[i] = currentInputState[i];
//     }
//   }
// }

// /**
//   Runs routines within the Interrupt Service Routine.
//   Display updating, button event tracking, LDR updating.
//  */
// void Display::isrRoutine()
// {
//   if(ir == 1){
//     pulseCount++;
//     if(pulseCount >= _refreshRate) {
//       updateRow();
//       pulseCount = 0;
//     }
//     if(pulseCount == _refreshRate/2) {
//       checkInputs();
//     }
//   }

//   if (ir == 0){
//     updateRow();
//     checkInputs();
//   }
// }

// /**
//   Scrolls a string across the display.
//   @param string the string that will be scrolled
//  */
// void Display::printString(String string)
// {
//   uint8_t screen[8]={0};
//   clear();
//   for( int index = 0; index<string.length(); index++ ){
//     char c = string[index];
//     int letIx = 0;
//     if( c>='A' && c<='Z' ) letIx = c-'A'+1;
//     else if( c>='a' && c<='z' ) letIx = c-'a'+1+26;
//     else if( c>='!' && c<='/' ) letIx = c+20;
//     else if( c>=':' && c<='@' ) letIx = c+10;
//     else if( c>='0' && c<='9' ) letIx = c+27;
//     int colIx = 0;
//     uint8_t col;
//     while( (col = allLetters[letIx][colIx]) != LETEND ){
//       appendColumn(screen, col);
//       colIx++;
//     }
//   }
//   for( int i=0; i<8; i++)
//     appendColumn(screen, 0);
// }

// /**
//   Appends the column for scrolling the string across the display.
//   @param screen the current screen to append.
//   @param col the column to add
//  */
// void Display::appendColumn(uint8_t* screen, uint8_t col)
// {
//   for( int i=0; i<8; i++){
//     screen[i]<<=1;
//     if( (col&(1<<(7-i)))!=0 ) screen[i]++;
//   }
//   printImage(screen);
//   ThisThread::sleep_for(70ms);
// }

// /**
//   Shows the score. Maximum 2 digits :(
//   @param n the score to be displayed
//  */
// void Display::showScore(int n)
// {
//   uint8_t result[8];
//   int dig1=n/10;
//   int dig2=n%10;
//   for(int p=0;p<8;p++) {
//     result[p]=allNumbers[dig2][p];
//     if( dig1>0 )
//     result[p]|=(allNumbers[dig1][p]<<4);
//   }
//   printImage(result);
// }

// /**
//   All the letters in the world.
//  */
// const uint8_t Display::allLetters[85][9] = {
//   {0b00000000,0b00000000,0b00000000,LETEND},   // space
//   {0b01111110,0b10010000,0b10010000,0b10010000,0b01111110,0b00000000,LETEND}, // A
//   {0b11111110,0b10010010,0b10010010,0b10010010,0b01101100,0b00000000,LETEND}, // B
//   {0b01111100,0b10000010,0b10000010,0b10000010,0b01000100,0b00000000,LETEND}, // C
//   {0b11111110,0b10000010,0b10000010,0b01000100,0b00111000,0b00000000,LETEND}, // D
//   {0b11111110,0b10010010,0b10010010,0b10010010,0b10000010,0b00000000,LETEND}, // E
//   {0b11111110,0b10010000,0b10010000,0b10010000,0b10000000,0b00000000,LETEND}, // F
//   {0b01111100,0b10000010,0b10001010,0b10001100,0b01001110,0b00000000,LETEND}, // G
//   {0b11111110,0b00010000,0b00010000,0b00010000,0b11111110,0b00000000,LETEND}, // H
//   {0b10000010,0b11111110,0b10000010,0b00000000,LETEND}, // I
//   {0b00000100,0b00000010,0b10000010,0b11111100,0b10000000,0b00000000,LETEND}, // J
//   {0b11111110,0b00010000,0b00101000,0b01000100,0b10000010,0b00000000,LETEND}, // K
//   {0b11111110,0b00000010,0b00000010,0b00000010,0b00000010,0b00000000,LETEND}, // L
//   {0b11111110,0b01000000,0b00100000,0b01000000,0b11111110,0b00000000,LETEND}, // M
//   {0b11111110,0b01100000,0b00010000,0b00001100,0b11111110,0b00000000,LETEND}, // N
//   {0b01111100,0b10000010,0b10000010,0b10000010,0b01111100,0b00000000,LETEND}, // O
//   {0b11111110,0b10010000,0b10010000,0b10010000,0b01100000,0b00000000,LETEND}, // P
//   {0b01111100,0b10000010,0b10000010,0b10000100,0b01111010,0b00000000,LETEND}, // Q
//   {0b11111110,0b10010000,0b10011000,0b10010100,0b01100010,0b00000000,LETEND}, // R
//   {0b01100100,0b10010010,0b10010010,0b10010010,0b01001100,0b00000000,LETEND}, // S
//   {0b10000000,0b10000000,0b11111110,0b10000000,0b10000000,0b00000000,LETEND}, // T
//   {0b11111100,0b00000010,0b00000010,0b00000010,0b11111100,0b00000000,LETEND}, // U
//   {0b11100000,0b00011000,0b00000110,0b00011000,0b11100000,0b00000000,LETEND}, // V
//   {0b11111100,0b00000010,0b00011100,0b00000010,0b11111100,0b00000000,LETEND}, // W
//   {0b10000010,0b01101100,0b00010000,0b01101100,0b10000010,0b00000000,LETEND}, // X
//   {0b11000000,0b00100000,0b00011110,0b00100000,0b11000000,0b00000000,LETEND}, // Y
//   {0b10000110,0b10001010,0b10010010,0b10100010,0b11000010,0b00000000,LETEND}, // Z
//   {0b00011100,0b00100010,0b00100010,0b00011110,0b00000000,LETEND}, // a
//   {0b01111110,0b00010010,0b00010010,0b00001100,0b00000000,LETEND}, // b
//   {0b00011100,0b00100010,0b00100010,0b00010010,0b00000000,LETEND}, // c
//   {0b00001100,0b00010010,0b00010010,0b11111110,0b00000000,LETEND}, // d
//   {0b00011100,0b00101010,0b00101010,0b00011010,0b00000000,LETEND}, // e
//   {0b00111110,0b01001000,0b01000000,0b00000000,LETEND}, // f
//   {0b00001101,0b00010101,0b00011110,0b00000000,LETEND}, // g
//   {0b01111110,0b00010000,0b00001110,0b00000000,LETEND}, // h
//   {0b01011110,0b00000000,LETEND}, // i
//   {0b00000010,0b00000010,0b01011100,0b00000000,LETEND}, // j
//   {0b11111110,0b00011000,0b00100110,0b00000000,LETEND}, // k
//   {0b01111100,0b00000010,0b00000010,0b00000000,LETEND}, // l
//   {0b00011110,0b00100000,0b00011000,0b00100000,0b00011110,0b00000000,LETEND}, // m
//   {0b00111110,0b00010000,0b00100000,0b00011110,0b00000000,LETEND}, // n
//   {0b00011100,0b00100010,0b00100010,0b00011100,0b00000000,LETEND}, // o
//   {0b00111111,0b00100100,0b00100100,0b00011000,0b00000000,LETEND}, // p
//   {0b00011000,0b00100100,0b00100100,0b00111111,0b00000000,LETEND}, // q
//   {0b00111110,0b00010000,0b00100000,0b00100000,0b00000000,LETEND}, // r
//   {0b00010010,0b00101010,0b00101010,0b00100100,0b00000000,LETEND}, // s
//   {0b01111100,0b00100010,0b00000010,0b00000000,LETEND}, // t
//   {0b00111100,0b00000010,0b00111110,0b00000000,LETEND}, // u
//   {0b00111000,0b00000110,0b00111000,0b00000000,LETEND}, // v
//   {0b00111100,0b00000010,0b00001100,0b00000010,0b00111100,0b00000000,LETEND}, // w
//   {0b00110110,0b00001000,0b00110110,0b00000000,LETEND}, // x
//   {0b00111001,0b00000101,0b00111110,0b00000000,LETEND}, // y
//   {0b00100110,0b00101010,0b00110010,0b00000000,LETEND}, // z
//   {0b11111010,0b00000000,LETEND}, // !
//   {0b11000000,0b00000000,0b11000000,0b00000000,LETEND}, // "
//   {0b00100100,0b01111110,0b00100100,0b01111110,0b00100100,0b00000000,LETEND}, // #
//   {0b01110100,0b01010100,0b11111110,0b01010100,0b01011100,0b00000000,LETEND}, // $
//   {0b01100010,0b01100100,0b00001000,0b00010000,0b00100110,0b01000110,0b00000000,LETEND}, // %
//   {0b00001100,0b01010010,0b10100010,0b01010010,0b00001100,0b00010010,0b00000000,LETEND}, // &
//   {0b10000000,0b01000000,0b00000000,LETEND}, // '
//   {0b01111110,0b10000001,0b00000000,LETEND}, // (
//   {0b10000001,0b01111110,0b00000000,LETEND}, // )
//   {0b00010000,0b01010100,0b00111000,0b01010100,0b00010000,0b00000000,LETEND}, // *
//   {0b00010000,0b00010000,0b01111100,0b00010000,0b00010000,0b00000000,LETEND}, // +
//   {0b00000001,0b00000010,0b00000000,LETEND}, // ,
//   {0b00010000,0b00010000,0b00010000,0b00010000,0b00000000,LETEND}, // -
//   {0b00000010,0b00000000,LETEND}, // .
//   {0b00000010,0b00000100,0b00001000,0b00010000,0b00100000,0b01000000,0b00000000,LETEND}, // /
//   {0b00100010,0b00000000,LETEND}, // :
//   {0b00000001,0b00100010,0b00000000,LETEND}, // ;
//   {0b00010000,0b00101000,0b01000100,0b10000010,0b00000000,LETEND}, // <
//   {0b00100010,0b00100010,0b00100010,0b00000000,LETEND}, // =
//   {0b10000010,0b01000010,0b00101000,0b00010000,0b00000000,LETEND}, // >
//   {0b01000000,0b10000000,0b10001101,0b10010000,0b01100000,0b00000000,LETEND}, // ?
//   {0b01111100,0b10000010,0b10111010,0b10111010,0b10001010,0b01111010,0b00000000,LETEND}, // @
//   {0b01111100,0b10000010,0b10000010,0b01111100,0b00000000,LETEND}, // 0
//   {0b00100010,0b01000010,0b11111110,0b00000010,0b00000010,0b00000000,LETEND}, // 1
//   {0b01000010,0b10000110,0b10001010,0b10010010,0b10100010,0b01000010,0b00000000,LETEND}, // 2
//   {0b01000100,0b10000010,0b10010010,0b01101100,0b00000000,LETEND}, // 3
//   {0b11110000,0b00010000,0b00111110,0b00000000,LETEND}, // 4
//   {0b11110010,0b10010010,0b10010010,0b10010010,0b10001100,0b00000000,LETEND}, // 5
//   {0b01111100,0b10010010,0b10010010,0b01001100,0b00000000,LETEND}, // 6
//   {0b10000000,0b10001110,0b10110000,0b11000000,0b00000000,LETEND}, // 7
//   {0b01101100,0b10010010,0b10010010,0b01101100,0b00000000,LETEND}, // 8
//   {0b01100000,0b10010010,0b10010010,0b01111100,0b00000000,LETEND}, // 9
// };

// /**
//   All the numbers in the world.
//  */
// const uint8_t Display::allNumbers[10][8] = {
//   { 0b00000010,0b00000101,0b00000101,0b00000101,0b00000101,0b00000101,0b00000101,0b00000010 },
//   { 0b00000010,0b00000110,0b00000010,0b00000010,0b00000010,0b00000010,0b00000010,0b00000111 },
//   { 0b00000010,0b00000101,0b00000001,0b00000010,0b00000010,0b00000100,0b00000100,0b00000111 },
//   { 0b00000111,0b00000001,0b00000001,0b00000110,0b00000001,0b00000001,0b00000101,0b00000010 },
//   { 0b00000100,0b00000101,0b00000101,0b00000111,0b00000001,0b00000001,0b00000001,0b00000001 },
//   { 0b00000111,0b00000100,0b00000100,0b00000110,0b00000001,0b00000001,0b00000101,0b00000010 },
//   { 0b00000011,0b00000100,0b00000100,0b00000110,0b00000101,0b00000101,0b00000101,0b00000010 },
//   { 0b00000111,0b00000001,0b00000001,0b00000010,0b00000010,0b00000100,0b00000100,0b00000100 },
//   { 0b00000010,0b00000101,0b00000101,0b00000010,0b00000101,0b00000101,0b00000101,0b00000010 },
//   { 0b00000010,0b00000101,0b00000101,0b00000011,0b00000001,0b00000001,0b00000001,0b00000110 }
// };