/* Volume knob with pushbutton mute toggle
 *  based on blink teensy example and encoder example
 */

#include <Bounce.h>
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

//control serial output to terminal
bool serial_out = false;

Encoder myEnc(5,6);  //quad encoder on PD0, PD1
Bounce button7 = Bounce(7, 10);  //SW on PD2


// Teensy 2.0 has the LED on pin 11
// Teensy++ 2.0 has the LED on pin 6
// Teensy 3.x / Teensy LC have the LED on pin 13
const int ledPin = 11;

int pulse_duration = 300;  //length of LED pulse wait for on/off
int pulse_count = 3;  //num of led pulses

bool detect = false;
long skip_val = 2;

// the setup() method runs once, when the sketch starts

void setup() {
  // initialize the digital pin as an output.
  pinMode(ledPin, OUTPUT);  //led output
  boot_blink();  //flash on-board LED to show sign of life


  //init input pins D0,D1,D3 for pushbuttons with pullups, short to ground for input
  pinMode(7, INPUT_PULLUP);

  if(serial_out == true)
  {
    Serial.begin(9600);
    Serial.println("Basic Encoder Test:");
  } 
}

long oldPosition = -999;

void boot_blink() {
  while(pulse_count >= 1){
    digitalWrite(ledPin, HIGH);   // set the LED on
    delay(pulse_duration);                  // wait for a second
    digitalWrite(ledPin, LOW);    // set the LED off
    delay(pulse_duration);                  // wait for a second
    pulse_count--;
  }
  
}


// the loop() methor runs over and over again,
// as long as the board has power
void loop() {

  //update inputs
  button7.update();

  long newPosition = myEnc.read();
/*
  if(newPosition != oldPosition){
    oldPosition = newPosition;
    Serial.println(newPosition);
  }
*/



//volume up event
if(newPosition > (oldPosition+skip_val)){
  oldPosition = newPosition;
  Keyboard.press(KEY_MEDIA_VOLUME_INC);
  Keyboard.release(KEY_MEDIA_VOLUME_INC);
  if(serial_out == true){
    Serial.println(newPosition);
    Serial.println("Up");
  }
}

//volume down event
if(newPosition < (oldPosition-skip_val))
  {
  oldPosition = newPosition;
  Keyboard.press(KEY_MEDIA_VOLUME_DEC);
  Keyboard.release(KEY_MEDIA_VOLUME_DEC);
  if(serial_out == true){
    Serial.println(newPosition);
    Serial.println("Down");
    }
  }


//toggle audio mute with pushbutton
  if (button7.fallingEdge()) {
    Keyboard.press(KEY_MEDIA_MUTE);
    Keyboard.release(KEY_MEDIA_MUTE);
    if(serial_out == true){
      Serial.println("MUTE");
    }
  }




/*  boot blink on pushbutton loop
  if (button7.fallingEdge()) {
    pulse_duration = 50;
    detect = true;
  }


if(detect != false){
  boot_blink();    
  detect = false;
}
*/
  
}

