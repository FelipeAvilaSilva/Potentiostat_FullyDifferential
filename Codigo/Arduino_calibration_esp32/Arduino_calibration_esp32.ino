
const int PinRead = 2;
const int PinPWM = 15;
float StartPotential; // These constants are used to store numerical values of potentials that user enters into serial monitor
float EndPotential;
int Startpot;
int Endpot;
long Intervals; //Time delay to obtain expected scan rate
int Scanrate; // scan rate
float Standtime; // waiting time before start experiment

  int freq = 5000;
  int channel = 0;
  int resolution = 8; // tentar mudar a resolução para 16bits depois

void Calibration() {

  Serial.println("First, potentiostat writes 255/0 on digital pin 9 for 5 seconds each time");
  Serial.println("In monitor serial you'll see two values, PWM corresponing to potential and analog reading corresponding to current");

  //float analog_reading = adc.analogReadXXbit(Pinread, bits_of_precision,num_samples);
  ledcWrite(channel, 255); // apply current potential to pin 9 // apply current potential to pin 9  
  delay(5000);
  Serial.println("For potential calibration: Watch potential in your multimeter (Vmin), this is the lower limit potential");
  Serial.print("PWM : ");
  Serial.println(0); 
  Serial.println("For current calibration: Watch potential in your multimeter, with this you must obtained the lower limit current (Imin)");
  Serial.println("AnalogReadingmin :"); 
  float analog_reading = analogRead(PinRead);
  Serial.println(analog_reading);
  delay(20000);
  Serial.println("");


  ledcWrite(channel, 0); // apply current potential to pin 9 // apply current potential to pin 9  
  delay(5000);
  Serial.println("For potential calibration: Watch potential in your multimeter(Vmax), this is the higher limit potential");
  Serial.print("PWM : ");
  Serial.println(255);  
  Serial.println("For current calibration: Watch potential in your multimeter, with this you must obtained the higher limit current (Imax)");
  Serial.println("AnalogReadingmax :");
  analog_reading = analogRead(PinRead);
  Serial.print(analog_reading);
  delay(20000);
}

void setup() {
 Serial.begin(9600);
 pinMode(PinPWM, OUTPUT);
 pinMode(PinRead, INPUT);
 ledcSetup(channel, freq, resolution);
 ledcAttachPin(PinPWM, channel);

}

void loop() {
  for (;;) {
    Serial.println("Calibration ");
    Serial.println("Press 1 to start ");
    Serial.println("Don't forget make properly connections for calibration");
    while (!Serial.available()) {;}    
    switch (Serial.read()) {
      case'1':
      Calibration();
      break;
      default: continue;
    }
  }
}