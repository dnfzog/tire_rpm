#include <LiquidCrystal_I2C.h>
#include "SPI.h" 
#include "SD.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
float value = 0;
float rev = 0;
int rpm;
int oldtime = 0;
int newtime;
int wings;
float RPMnew;


//SDカード設定
File myFile;                //クラス指定
const int chipSelect =53;  //チップセレクトピン指定




void isr() //interrupt service routine
{
  rev++;
}

void setup()
{
  lcd.init();
  lcd.backlight();
  attachInterrupt(digitalPinToInterrupt(2), isr, RISING);
  Serial.begin(9600); // シリアル通信の初期化

  //SDカードの初期化
  Serial.println("Initializing SD card...");

  //SD.begin(chipSelect);
  
  if (!SD.begin(chipSelect)) {
    Serial.println("Initialization failed or does not exist");
    while (1);
  }
  Serial.println("Initialization done.");
  Serial.println();

  
  
  delay(1000);  
  
  //シリアルログの最初の行に表示項目を表示する
  Serial.println("Count Start");

  //ログデータの最初の行に記録項目を記入する
  myFile = SD.open("DATA_LOG.csv", FILE_WRITE); //SDカードをオープンする 
  myFile.println("Count Start");
  myFile.close(); 	//SDカードをクローズする 
}

void loop()
{
  delay(1000);
  detachInterrupt(0);
  newtime = millis() - oldtime;
  wings = 8;
  RPMnew = rev / wings;
  rpm = (RPMnew / newtime) * 60000;
  oldtime = millis();
  rev = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("___TACHOMETER___");

  lcd.setCursor(0, 1);
  lcd.print(rpm);
  lcd.print(" RPM");
  lcd.print(" ");
  attachInterrupt(digitalPinToInterrupt(2), isr, RISING);
  Serial.print("RPM: ");
  Serial.println(rpm); // シリアルモニタにRPMを表示

 
   
  //SDカードへの書き込み
  myFile = SD.open("DATA_LOG.csv", FILE_WRITE); 
  myFile.println(rpm);
  myFile.close();

  
  
  // 測定間隔を設定する
  delay(1000);   

}
