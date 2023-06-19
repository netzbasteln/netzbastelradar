/* Based on HB100-Filtering.ino by TMRh20 2016
https://github.com/TMRh20/AnalogFrequency/blob/master/examples/HB100-Filtering/HB100-Filtering.ino
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

//******** USER CONFIG *********************

//Define the "zero" point of the AC waveform as a DC value between 0 and 1023. This needs to be correct when the sensitivity is low ( <100 )
#define MIDPOINT 511

// Define the sensitivity of analog detection. With a very clean & consistent waveform, the sensitivity can be rased significantly.
// This value is important in determining if detected peak-to-peak values constitute a 'wave'
#define SENSITIVITY 2

// The analog pin to use
#define ADCPin A1

// Incoming data is summed, so fetching the results every second
// will indicate speed over the previous second
// How often in mS to display the results ( 0 = print all results if possible)
#define printDelay 500

//*****************************************/

#include "AnalogFrequency.h"

uint32_t displayTimer = 0;
uint32_t amplitude = 0, samples = 0;

void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
  Serial.begin(115200);
  setupADC(ADCPin);
  Serial.print("HI");
  lcd.init();                      // initialize the lcd
  lcd.backlight();
  lcd.setCursor(0, 1);
  lcd.print("#NETZBASTELRADAR");
}

void loop() {
  if ( fAvailable() && millis() - displayTimer > printDelay ) {
    displayTimer = millis();
    uint32_t frequency = getFreq(&amplitude, &samples);

    //Filter out low amplitude signals with a low count
    if (amplitude < 540 && samples < 2) {
      return;
    }
    lcd.setCursor(0, 0);
    lcd.print("   km/h");
    float speedKM = frequency / 19.49;
    Serial.print(speedKM/2);
    lcd.setCursor(0, 0);
    lcd.print((int)speedKM/2);
    Serial.print("   Hz ");
    Serial.print(frequency);
    Serial.print("   Amplitude ");
    Serial.print(amplitude);
    Serial.print("   Samples ");
    Serial.println(samples);
  }
}
