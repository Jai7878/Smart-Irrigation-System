#include <LiquidCrystal_I2C.h>
 #include <ESP8266WiFi.h>  // Include the MCU library
 #define BLYNK_PRINT Serial
 #define BLYNK_TEMPLATE_ID "TMPL3BdKEzPis"
 #define BLYNK_TEMPLATE_NAME "Smart Irrigation"
 #define BLYNK_AUTH_TOKEN "zQiTdmbNOuAh8XgBA0vjwcuV5LCpJFLB"
 #include <BlynkSimpleEsp8266.h>
 #include <DHT.h>
 #include <Wire.h>
 LiquidCrystal_I2C lcd(0x27, 16, 2);  // Format -> (Address,Width,Height )
 DHT dht(D4, DHT11);
 //Define component pins
 #define soil A0
 #define PUSH_BUTTON D7
 #define RELAY_PIN D3
 #define VPIN_BUTTON   V12 
BlynkTimer timer;
 bool pumpState = 0;
 int previousState = HIGH;
 int H, T, M;
 int MOISTURE_THRESHOLD = 60;
 bool forceStop = false;
 const char* ssid = "ESP8266";
 const char* pass = "***air62656***";
 void setup() {
 pinMode(RELAY_PIN, OUTPUT);
 digitalWrite(RELAY_PIN, HIGH);
 Serial.begin(9600);
 lcd.init();
 lcd.backlight();
 Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, lcd, "blynk.cloud", 80);
 delay(750);  
dht.begin();
 pinMode(PUSH_BUTTON, INPUT_PULLUP);
 lcd.clear();
 updateLCD();
 decideToStartPump();
 timer.setInterval(100, DHT11sensor);
 timer.setInterval(100, readSoilMoistureSensor);
 timer.setInterval(50, checkPushButton);
 timer.setInterval(1000, decideToStartPump);
 }
 void updateLCD() {
 Wire.setClock(10000);
 lcd.setCursor(8, 1);
 lcd.print("Pump:");
 lcd.setCursor(13, 1);
 lcd.print("   ");
  lcd.setCursor(13, 1);
  if (pumpState) lcd.print("On");
  else lcd.print("Off");
  digitalWrite(RELAY_PIN, !pumpState);
  Blynk.virtualWrite(VPIN_BUTTON, pumpState);
  Wire.setClock(100000);
 }
 void loop() {
  timer.run();
 }
 void clearLCD() {
  lcd.clear();
 }
 BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(VPIN_BUTTON);
 }
 BLYNK_WRITE(VPIN_BUTTON) {
  int a = param.asInt();
  if(pumpState == 1 && a == 0){
    forceStop = true;
    timer.setTimeout(5000, resetForceStop);
  }
  if(a == 1 && M == 0){
    pumpState = 1;
    updateLCD();
    delay(1500);
  }
  pumpState = a;
  updateLCD();
 }
 void readSoilMoistureSensor() {
  int value = analogRead(soil);
  value = map(value, 0, 1024, 0, 100);
  value = 100 - value;
  M = value;
  Blynk.virtualWrite(V3, M);
  static int lastValue = -1;  // Store last value to detect changes
  if (value != lastValue) {   // Update LCD only if value changes
    lcd.setCursor(0, 1);
    lcd.print("S:");
    lcd.print("   ");
    lcd.setCursor(2, 1);
    lcd.print(value);
    lcd.print("%");
    lastValue = value;
  }
 }
 void resetForceStop(){
  if(forceStop){
    forceStop = false;
  }
 }
 void checkPushButton() {
  if(forceStop) return;
  int buttonState = digitalRead(PUSH_BUTTON);  // Read button state
  
  if (buttonState != previousState && buttonState == LOW) {  // Check for button
 press with debounce
    // Button pressed, toggle pump state
    if(pumpState == 1){
      forceStop = true;
    }
    if(M == 0){
      pumpState = 1;
      updateLCD();
      delay(1500);
    } else{
    pumpState = !pumpState;
    updateLCD();
    }
    delay(100);
  }
  previousState = buttonState;
 }
 void DHT11sensor() {
  float h = dht.readHumidity();
  int t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  H = h; T = t;
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
  static float lastHumidity = NAN;
  static int lastTemperature = NAN;
  if (h != lastHumidity || t != lastTemperature) {  // Update LCD only if values
 change
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(t);
    lcd.print((char)223);
    lcd.print("C");
    lcd.setCursor(8, 0);
    lcd.print("H:");
    lcd.print(h);
    lcd.print("%");
    lastHumidity = h;
    lastTemperature = t;
  }
 }
 void decideToStartPump(){
  if ((M > 1 && M < MOISTURE_THRESHOLD)){
    if(pumpState != 1 && forceStop == false){
      pumpState = 1;
      updateLCD();
    }
  }
  if(pumpState != 0 && (forceStop || (M == 0 || M >= MOISTURE_THRESHOLD))){
    pumpState = 0;
    updateLCD();
  }
  if(forceStop){
    timer.setTimeout(5000, resetForceStop);
  }
  delay(100);
 }
