#include <LiquidCrystal.h>

#define checker A0
#define dcIn A1
#define batFul A2
#define buz 9
#define batCon 8
boolean sound = true;
boolean batF = true;
int timer = 0; int hdTimer = 225;


LiquidCrystal lcd(2,3,4,5,6,7);


void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  pinMode(dcIn, INPUT);
  pinMode(batFul, INPUT);
  pinMode(checker, INPUT);
  pinMode(buz, OUTPUT);
  pinMode(batCon, OUTPUT);
  alarm(1);
  lcd.print("INVERTER BUILT BY YUKEN AUTOMATION");
  delay(2000);
  for(int i = 0; i < 17; i++){
    lcd.scrollDisplayLeft();
    delay(800);
  }
}

void batteryCheck(){
  while(dcMeter() < 5.00){
    lcd.clear();
    lcd.print("BATTERY LOW,PLEASE SHUT DOWN ");
    digitalWrite(buz, HIGH);
    for(int i = 0; i < 15; i++){
      lcd.scrollDisplayLeft();
      delay(800);
    }
  }
}

void alarm(int x){
  for(int i = 0; i < x; i++){
    digitalWrite(buz, HIGH);
    delay(1000);
    digitalWrite(buz, LOW);
    delay(1000);
  }
}


float dcMeter(){
  float val = 0.0;
  for(int i = 0; i < 20; i++){
    val = val + analogRead(dcIn);
    delay(100);
  }
  val = val / 20;
  val = (5 * val) / 1023;
  float result = 4.3 * val; //(r1+r2)/r2 = 4.3, r1=10k and r2=3k
  return result;
}
void inverter(){
  digitalWrite(batCon, LOW);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("***INVERTING***");
  lcd.setCursor(0,1); //col,row
  lcd.print("* OUTPUT: 300V *");
  delay(3000);
  float meter = dcMeter();
  int rs = (meter/13) * 100;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("NOT CHARGING!");//chaarge icon
  lcd.setCursor(0,1); //col,row
  lcd.print("BATTERY :  ");
  if(sound == true){
    alarm(5);
    sound = false;
  }
  lcd.setCursor(10,1);
  lcd.print(rs); 
  lcd.setCursor(13,1);
  lcd.print("%");
  delay(3000);
  batteryCheck();
  
  timer++;
  if(timer >= hdTimer){
     alarm(2);
     timer = 0;
  }
 
}

void byPass(){
  timer = 0;
  digitalWrite(batCon, HIGH);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("*AC MAIN SUPPLY*");
  lcd.setCursor(0,1);
  lcd.print("* OUTPUT: 220V *");
  if(sound == false){
    alarm(5);
    sound = true;
  } 
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  
  if(analogRead(batFul) > 500){
    lcd.clear();
    
    lcd.print("**BATTERY FULL**");
    lcd.setCursor(0,1);
    lcd.print("charging stopped");
    delay(1000);
    if(batF == true){
      alarm(2);
      batF = false;
    }
  }
  else{
    lcd.clear();
    batF = true;
    lcd.print("BATTERY CHARGING");//chaarge icon
    lcd.setCursor(0,1);
    for(int i = 0; i < 16; i++){
      lcd.setCursor(i,1);
      lcd.print(".");
      delay(500);
    }
  }
  delay(1000);
}

void intro(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("YUKEN AUTOMATION");
  lcd.setCursor(0,1);
  lcd.print("*PURE SINE WAVE*");
  delay(3000);
}

void loop() {
  intro();
  lcd.setCursor(0,0);
  if(analogRead(checker) >= 500){
    digitalWrite(batCon, LOW);
    byPass();  
  }
  else{
    digitalWrite(batCon, HIGH);
    digitalWrite(buz, LOW);
    inverter();
    
  }
  lcd.clear();
}

