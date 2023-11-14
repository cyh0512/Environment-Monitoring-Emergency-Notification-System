#include <Arduino.h>  
#include <Keypad.h>    // 引用Keypad程式庫
#include <SoftwareSerial.h> 
#include <U8g2lib.h>  //OLED library宣告
#include <SPI.h>
#include <Wire.h>
#include <SD.h>
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0); 


#define KEY_ROWS 4 // 按鍵模組的列數
#define KEY_COLS 4 // 按鍵模組的行數
// 依照行、列排列的按鍵字元（二維陣列）
char keymap[KEY_ROWS][KEY_COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte colPins[KEY_COLS] = {37,35,33,31};     // 按鍵模組，行1~4接腳。
byte rowPins[KEY_ROWS] = {45,43,41,39}; // 按鍵模組，列1~4接腳。 
// 初始化Keypad物件
// 語法：Keypad(makeKeymap(按鍵字元的二維陣列), 模組列接腳, 模組行接腳, 模組列數, 模組行數)
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, KEY_ROWS, KEY_COLS);
//************************SDcard***********************//
File myFile;
File myFile1;
String phone_edit="";
String phonenumber[10]={};
int column = 0;
int k=1;
String order;
//************************Dialer***********************//
String Phone_Number = "";
String tempPhone_Number = "";
int i = 0;
int mark = 0;
SoftwareSerial BT(10,11); // 接收腳, 傳送腳
char val;  // 儲存接收資料的變數

const int FindP = A0 ;//尋機pin
const int FindO = 26;//尋機out
boolean Findingstate= false ;
const int BP=A1 ;//緊急按鈕

void OLEDprint(String disp){//用來顯示在OLED上的函式
  //u8g2.clearBuffer();         // clear the internal memory
  //u8g2.setCursor(xc,yc);
  u8g2.setFont(u8g2_font_logisoso18_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
  u8g2.print(disp);
  u8g2.sendBuffer();         // transfer internal memory to the display
}
void editphone(String phone,int column){
   phonenumber[column-1]=phone;
 }

void setup()  
{
  u8g2.begin();//OLED顯示
  u8g2.setCursor(0,29);
  OLEDprint("Hello");
  delay(1000);
  u8g2.clearBuffer();
  u8g2.setCursor(10,29);
  Serial.begin(38400);
  Serial.println("BT is ready!");
  BT.begin(38400);
  pinMode(BP,INPUT);
  pinMode(FindP,INPUT);
  pinMode(FindO,OUTPUT);
  //pinMode(EmerP,INPUT);
  digitalWrite(FindP,HIGH);
//************************SDcard***********************//
 Serial.print("Initializing SD card...");
 if (!SD.begin(22)) {
   Serial.println("initialization failed!");
   while (1);
 }
 Serial.println("initialization done.");
 myFile = SD.open("haha.txt");
 if (myFile) {
   Serial.println("haha.txt:");
   while (myFile.available()) { // read from the file until there's nothing else in it:
     String readed=myFile.readStringUntil('\n');
     Serial.println(readed);  
     phonenumber[k-1]=readed;
     k++;
   // close the file:
   }
   myFile.close();
   }
  else {
   Serial.println("error opening haha.txt");     // if the file didn't open, print an error:
 }
}
void loop()
{
    

    char key = myKeypad.getKey(); // 透過Keypad物件的getKey()方法讀取按鍵的字元
    if (key) // 若有按鍵被按下…
    {  
      
      if(mark==0 && key == 65) //按下A(確定撥出)後執行
      {
       BT.print(key);
      Serial.print("Calling ");
      Serial.print(Phone_Number);  // 顯示撥打的電話號碼
      Serial.println("...");
      for(int l=0;l<2;l++){//Calling顯示在OLED
      u8g2.clearBuffer();
      u8g2.setCursor(0,23);
      OLEDprint("Calling.");
      u8g2.clearBuffer();
      u8g2.setCursor(0,23);
      OLEDprint("Calling..");
      u8g2.clearBuffer();
      u8g2.setCursor(0,23);
      OLEDprint("Calling...");
      delay(1000);}
      tempPhone_Number=Phone_Number;
 
      u8g2.clearDisplay();
      u8g2.setCursor(0,29);
      mark= 1;//是否有按下ABCD鍵，1為是，0為否 
      }
      if(key == 66) //按下B(重設號碼)後執行
      {
       BT.print(key);
      Serial.print("Reset number ");
      u8g2.clearBuffer();
      u8g2.setCursor(0,29);
      OLEDprint("Reset...");
      delay(1000);
      u8g2.clearDisplay();
      u8g2.setCursor(0,29);
      mark= 1;//是否有按下ABCD鍵，1為是，0為否
      }  
      if(key == 67) //按下C(再次撥號)後執行
      {
      BT.print(key);
      Serial.print("Calling again ");
      Serial.print(tempPhone_Number);  // 顯示撥打的電話號碼
      Serial.println("...");
      u8g2.clearBuffer();
      u8g2.setCursor(0,23);
      OLEDprint("Call again");
      delay(2000);
      u8g2.clearBuffer();
      u8g2.setCursor(0,29);
      OLEDprint(tempPhone_Number);
     
      u8g2.clearDisplay();
      u8g2.setCursor(0,29);
      mark= 1;//是否有按下ABCD鍵，1為是，0為否
      }  
      if(mark==0) //還沒按下A(還在撥號)時執行
      {
        Serial.print("PRESS ");
        Serial.println(key);  // 顯示按下的字元
        BT.print(key);
        OLEDprint(String(key));
        Phone_Number += key; //Phone_Number = Phone_Number + key ; phone number是字串，初始化時 = " "。
        delay(2);
      }
      //*************************************選擇聯絡人*************************************//
     if(mark == 2) //按下D之後，接著選擇聯絡人編號1~9。
     {
        Phone_Number=phonenumber[key-49];
        Serial.print("Select phone");
        Serial.println(key-48);
        u8g2.clearDisplay();
        u8g2.setCursor(0,29);
        OLEDprint("Select ");
        OLEDprint(String(key-48));
        BT.print(Phone_Number);
        mark=0;                      
     }
     if(key == 68) //按下D(選擇聯絡人)後執行
     {
     Serial.println("Select contact number");
      u8g2.clearDisplay();
      u8g2.setCursor(0,29);
     OLEDprint("Choosing");
     mark=2;
     }
     
     //*************************編輯聯絡人*****************************//
     if(mark == 4) //輸入新電話至聯絡人。
     {   
       if(key == 65)//修改完聯絡人後，按下A執行
       {
        
         editphone(phone_edit,column);
         SD.remove("haha.txt");
         myFile1 = SD.open("haha.txt", FILE_WRITE);
         if(myFile1){
           Serial.print("printing...");
           u8g2.clearDisplay();
           u8g2.setCursor(0,29);
           OLEDprint("printing");
           for(int pc=0;pc<10;pc++){
            myFile1.println(phonenumber[pc]);
           }
         }
         myFile1.close();
         Serial.println("Finish editing.");
         u8g2.clearDisplay();
         u8g2.setCursor(0,29);
         OLEDprint("Finish");
         phone_edit="";
         mark=0;
         Phone_Number = "";
       }
       else
       {
         Serial.print("PRESS ");
         Serial.println(key);  // 顯示按下的字元
         phone_edit += key; // 輸入要更改的聯絡人號碼
         Serial.println(phone_edit);
         u8g2.clearDisplay();
         u8g2.setCursor(0,29);
         OLEDprint(phone_edit);
         delay(2);
       }
     }
     if(key == 35) //按下#(編輯聯絡人)時執行
     {
       mark=3;
     }
     if(mark == 3 && key !=35) //按下#之後，選擇聯絡人來編輯電話號碼。
     {
       Serial.print("PRESS ");
       Serial.println(key);  // 顯示按下的字元
       if(key == 65)//輸入完聯絡人後，按下A來繼續輸入電話。
       {        
        Serial.println("Enter phone number to edit");
        u8g2.clearDisplay();
        u8g2.setCursor(0,29);
        OLEDprint("Enter number");
        mark=4;
       }
       else
       {
        column=key-48; // 輸入要更改的聯絡人編號
        Serial.print("Edit phone ");
        Serial.println(column);
        u8g2.clearDisplay();
        u8g2.setCursor(0,29);
        OLEDprint("Editing");
        OLEDprint(String(column));
       }
           
     }
 //*****************************************************************//
     if(mark == 1) //If the number just dialed is received. Since the number has been output, clear it and reset mark.
    {
      Phone_Number = "";
      Serial.println();
      mark = 0;
    }
    }
    
    if (BT.available()) {
    val = BT.read();
    Serial.print(val);
  }


  //以下為緊急觸發情況
  if(analogRead(BP)>500 || analogRead(A2)>300){
    BT.print("*");
    u8g2.clearBuffer();
    u8g2.setCursor(0,23);
    OLEDprint("HELP!!!");
    Serial.println("Calling emergency !");
    while((analogRead(BP))>500 || analogRead(A2)>300){}
  }
  //以上為緊急觸發情況

  //以下為尋機狀況
  while(analogRead(FindP)<400){
    u8g2.clearBuffer();
    u8g2.setCursor(0,23);
    OLEDprint("Searching");
    Serial.println("Searching...");
    digitalWrite(FindO,LOW);
    delay(200);
    digitalWrite(FindO,HIGH);
    delay(800);
    }
  //以上為尋機狀況
}



