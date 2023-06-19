// constants defined for library
#include <eRCaGuy_analogReadXXbit.h>

eRCaGuy_analogReadXXbit adc;
const float MAX_READING_10_bit = 1023.0;
const float MAX_READING_11_bit = 2046.0;
const float MAX_READING_12_bit = 4092.0;
const float MAX_READING_13_bit = 8184.0;
const float MAX_READING_14_bit = 16368.0;
const float MAX_READING_15_bit = 32736.0;
const float MAX_READING_16_bit = 65472.0;
const float MAX_READING_17_bit = 130944.0;
const float MAX_READING_18_bit = 261888.0;
const float MAX_READING_19_bit = 523776.0;
const float MAX_READING_20_bit = 1047552.0;
const float MAX_READING_21_bit = 2095104.0;

const int Pinread = A0;
const int PinPWM = 9;
float StartPotential; // These constants are used to store numerical values of potentials that user enters into serial monitor
float EndPotential;
int Startpot;
int Endpot;
long Intervals; //Time delay to obtain expected scan rate
int Scanrate; // scan rate
float Standtime; // waiting time before start experiment

void Calibration() {

  Serial.println("First, potentiostat writes 255/0 on digital pin 9 for 5 seconds each time");
  Serial.println("In monitor serial you'll see two values, PWM corresponing to potential and analog reading corresponding to current");
  int bits_of_precision = 12;
  int num_samples = 1;
  //float analog_reading = adc.analogReadXXbit(Pinread, bits_of_precision,num_samples);
  analogWrite(PinPWM, 0); // apply current potential to pin 9  
  delay(5000);
  Serial.println("For potential calibration: Watch potential in your multimeter (Vmin), this is the lower limit potential");
  Serial.print("PWM : ");
  Serial.println(0); 
  Serial.println("For current calibration: Watch potential in your multimeter, with this you must obtained the lower limit current (Imin)");
  Serial.println("AnalogReadingmin :"); 
  float analog_reading = adc.analogReadXXbit(Pinread, bits_of_precision,num_samples);
  Serial.println(analog_reading);
  delay(20000);
  Serial.println("");


  analogWrite(PinPWM, 255); // apply current potential to pin 9  
  delay(5000);
  Serial.println("For potential calibration: Watch potential in your multimeter(Vmax), this is the higher limit potential");
  Serial.print("PWM : ");
  Serial.println(255);
  delay(5000);
  Serial.println("For current calibration: Watch potential in your multimeter, with this you must obtained the higher limit current (Imax)");
  Serial.println("AnalogReadingmax :");
  analog_reading = adc.analogReadXXbit(Pinread, bits_of_precision,num_samples);
  Serial.print(analog_reading);
  delay(20000);
}

void setup() {
 TCCR1B = TCCR1B & B11111000 | B00000001;
 Serial.begin(9600);
 analogReference(1);
 pinMode(PinPWM, OUTPUT);
 pinMode(Pinread, INPUT);
}

void loop() {
  for (;;) {
    Serial.println("Calibration ");
    Serial.println("Press 1 to start ");
    Serial.println("Don't forget make properly connections for calibration");
    delay(5000);
    while (!Serial.available()) {;}    
    switch (Serial.read()) {
      case'1':
      Calibration();
      break;
      default: continue;
    }
  }
}