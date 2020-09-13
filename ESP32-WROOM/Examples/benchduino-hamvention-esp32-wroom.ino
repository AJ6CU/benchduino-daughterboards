
// ==============================================================================
// ==============================================================================
// Title:  BenchDuino Example 1.  Author:  George KJ6VU
// Morse code section based on the simple Morse Beacon by: Mark VandeWettering K6HX http://brainwagon.org
// Modified by Mark AJ6CU on Sept 13 for ESP32
//
// Specific board:
// https://www.amazon.com/gp/product/B0718T232Z/ref=ask_ql_qh_dp_hza
//
//
// To use Arduino IDE must:
// 1. Set Board to ESP32 Dev Module
// 2. Upload speed 115200 (sometimes the 900k speeds works...)
// 3. Set partition size to Huge APP (3mb no  otw/1mb Spiffs) - this is because the BT library is so large...
//    even though we don't use it...
//

           char sw_version[10] = "1.0";
           int dbg = 0;
// ==============================================================================
// ==============================================================================
// Physical pin assignments 
// NOTE These are in terms of ESP32 GPIO pin#. Look in pin mapping, and then 
// See which GPIO pin# is associated with which Benchduino pin
// ==============================================================================
  const int PinA  = 2;         const int PinB  = 33;         const int PinSW = 4;    // Rotary encoder
  const int button1 = 5;
  const int button2 = 16;
  const int button3 = 17;
  const int button4 = 18;
  const int led1 = 32;
  const int led2 = 0;
  const int led3 = 25;  //So want to use GPIO11 here. But it is reserved for SPI flash
  const int led4 = 12;
  const int statuspin   = 13;
  int analogPin = 36;
//  const int beeperpin = 22;
// ==============================================================================
// Define Menu System text and control variables
// ==============================================================================
// LCD Menu array is 5 different windows with 2 lines.  Addressing starts at 0.
  int maxmenu = 5;
  String menu[6][2] {    
    {"Welcome         ","BenchDuino      "},  // Menu item #0 - Welcome splash screen
    {"Freq            ","LED LCD POT CW  "}}; // Menu item #1 - Demo items
    int current_menu = 0;  

struct t_mtab { char c, pat; } ;

struct t_mtab morsetab[] = {
  {'.', 106},{',', 115}, {'?', 76}, {'/', 41},
  {'A', 6},  {'B', 17}, {'C', 21},  {'D', 9},  {'E', 2},  {'F', 20},  {'G', 11},  {'H', 16},  {'I', 4},
  {'J', 30}, {'K', 13}, {'L', 18},  {'M', 7},  {'N', 5},  {'O', 15},  {'P', 22},  {'Q', 27},  {'R', 10},
  {'S', 8},  {'T', 3},  {'U', 12},  {'V', 24}, {'W', 14}, {'X', 25},  {'Y', 29},  {'Z', 19},  
  {'1', 62}, {'2', 60}, {'3', 56},  {'4', 48}, {'5', 32}, {'6', 33},  {'7', 35},  {'8', 39},  {'9', 47},  {'0', 63}
} ;

#define N_MORSE  (sizeof(morsetab)/sizeof(morsetab[0]))
#define SPEED  (20)
#define DOTLEN  (1200/SPEED)
#define DASHLEN  (3*(1200/SPEED))
// ==============================================================================
// Define radio and control head operating parameters
// ==============================================================================
  byte config_radio_address = 0xA2;
  byte config_controller_address = 0xE0;
  byte button_debounce_time = 500;
  unsigned long freq = 7100;
  unsigned long lastFreq = freq;
  unsigned long freqStep = 1;
  int button1_val;  int button2_val;  int button3_val;  int button4_val;
// ==============================================================================
// Define global variables
// ==============================================================================
  int i = 0;
  int j = 0;
  int x = 0;
  int y = 0;
  String cmd;
  String arg[10];
  int parser_stringlen = 0;
  int parser_strstart = 0;
  int parser_strstop = 0;
  int parser_numargs = 0;
  int parser_argstart[10];
  int parser_argend[10];
  int parser_argnum = 0;
  String str = ""; // create a string
  byte inbyte;
  const byte numChars = 101;
  char receivedChars[numChars];
  boolean newData = false;
  int data =0;
  String outdata;
  String my_address_string;
  String next_arg;
  String payload;
  String intoaddress;
  char boardtype[3];
  int packetlen = 0;
  int stringlen; // String length
  char my_address[2] = {'0', '0'};
  int toaddress_match = 0;
  int delimiter1_match = 0;
  int delimiter2_match = 0;
  int fromaddress_match = 0;
  int goodpacket = 0;
  int directpacket = 0;
  String command;
  int diditflag = 0;
  int val = 0;
  int my_address_value = 0;
  int lastCount = 0;
  volatile int virtualPosition = 0;
  uint8_t draw_state = 2;
  float freqfloat = 0;
// ==============================================================================
// Import libraries
// ==============================================================================
  #include <U8g2lib.h>           //replaces U8glib
  #define U8X8_HAVE_HW_I2C
  #include <Wire.h>
  
// Had problems with the "NewLiquidCrystal_I2C" library and had to drop back
// to the "standard" one
  #include <LiquidCrystal_I2C.h> 
  
 
// ==============================================================================
// Device address assignments
// ==============================================================================
  int lcd_addr    = 0x27;  // LCD address.  Typicaly 0x27 or 0x3F
// ==============================================================================
// Initialize displays 
// ==============================================================================
 // LiquidCrystal_I2C  lcd(lcd_addr,2,1,0,4,5,6,7);
 LiquidCrystal_I2C lcd(0x27, 16, 2); 

 typedef u8g2_uint_t u8g_uint_t;   //avoids lots of code changes
 U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g(U8G2_R0,SCL,SDA, U8X8_PIN_NONE);

  
// ==============================================================================
// INTERRUPT
// ==============================================================================
 void isr ()  {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > 5) {      // If interrupts come slower than 5ms, we assume it's good data
    if (digitalRead(PinB) == LOW) {freq = freq - freqStep ;}  else  {freq = freq + freqStep;}
  }
  lastInterruptTime = interruptTime;    // Keep track of when we were here last (no more than every 5ms)
}


// ==============================================================================
// ==============================================================================
// ===== SETUP =====
// ==============================================================================
// ==============================================================================
void setup() {
  Serial.begin(115200);
//  Serial1.begin(9600);  // Never tested the CAT out. So just commented it out.
// ==============================================================================
// Set pin modes
// ==============================================================================
    Serial.print("In Setup");
    pinMode(PinA, INPUT);  pinMode(PinB, INPUT);  pinMode(PinSW, INPUT_PULLUP);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(led4, OUTPUT);
    pinMode(statuspin, OUTPUT);
    pinMode(button1, INPUT);
    pinMode(button2, INPUT); 
    pinMode(button3, INPUT); 
    pinMode(button4, INPUT); 
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    digitalWrite(led3,LOW);
    digitalWrite(led4,LOW); 
   
// ==============================================================================
    attachInterrupt(digitalPinToInterrupt(PinA), isr, RISING);
 // Serial1.print("STARTUP  Version ");
 // Serial1.println(sw_version);
  outdata = "/";
  my_address_string = "00";
  next_arg = "";
  payload = "";
//=== LCD SETUP =================================================================
  // lcd.setBacklightPin(3,POSITIVE);  lcd.setBacklight(HIGH);  
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);  lcd.clear();
  Serial.print("initialized lcd");

  u8g.begin();
  do_update_lcd(current_menu);  delay(1000);       // SPLASH SCREEN
  current_menu = 1;  do_update_lcd(current_menu);  // Jump to first screen

  digitalWrite(led1,HIGH);
  digitalWrite(led2,HIGH);
  digitalWrite(led3,HIGH);
  digitalWrite(led4,HIGH);
  digitalWrite(statuspin,HIGH);
}

// ==============================================================================
// ==============================================================================
// ===== MAIN LOOP =====
// ==============================================================================
// ==============================================================================
void loop() 
{

 // recvWithStartEndMarkers();
 // processpayload();

  button1_val = digitalRead(button1);  if (button1_val==LOW and current_menu==1) { do_button11(); }
  button2_val = digitalRead(button2);  if (button2_val==LOW and current_menu==1) { do_button12(); }
  button3_val = digitalRead(button3);  if (button3_val==LOW and current_menu==1) { do_button13(); }
  button4_val = digitalRead(button4);  if (button4_val==LOW and current_menu==1) { do_button14(); }
//
// Had to add this to turn back on led's after execution of the command. Not
// needed for other MCU's. Not sure why...
//
  digitalWrite(led1,HIGH);
  digitalWrite(led2,HIGH);
  digitalWrite(led3,HIGH);
  digitalWrite(led4,HIGH);

  if ((!digitalRead(PinSW))) { freq = 7100;  while (!digitalRead(PinSW))  delay(200); }
  if (freq != lastFreq) { Serial.println(freq);  /* Serial1.println(freq); */    lcd.setCursor (5,0); lcd.print(String (freq));   }   lastFreq = freq; // Keep track of this new value 
  


 
/// OLED picture loop  
  u8g.firstPage();  
   
  do 
  {
    draw();
  } while( u8g.nextPage() );
  
  // increase the state
  draw_state++;
  if ( draw_state >= 9*8 )
    draw_state = 0; 
    
}

// ==============================================================================
// ==============================================================================
// ===== SUBROUTINES =====
// ==============================================================================
// ==============================================================================


//-------------------------------------------------------------------------------
void send_msg(String outtext) {
  Serial.println(outtext);
  //Serial1.println(outtext);
return;
}

//----- LCD SUBROUTINES ---------------------------------------------------------
void do_update_lcd(int menu_item) {
  lcd.setCursor (0,0);  lcd.print(menu[menu_item][0]);
  lcd.setCursor (0,1);  lcd.print(menu[menu_item][1]);
  lcd.setCursor (5,0);  lcd.print(String (freq));
  }

//------ MENU 1 BUTTON 1 --------------------------------------------------------
  void do_button11()  { 
    send_msg("M1B1");
    Serial.println("button 1 pressed"); 
    debounce(1);
    lcd.setCursor (0,0);  lcd.print("BenchDuino      "); 
    lcd.setCursor (0,1);  lcd.print("LED Test        "); 
    for (i=0;i<=4;i++) {
      y = 100;
      int z = 0;
      digitalWrite(led1,HIGH);  delay(y);  digitalWrite(led1,LOW);  delay(z);
      digitalWrite(led2,HIGH);  delay(y);  digitalWrite(led2,LOW);  delay(z);
      digitalWrite(led3,HIGH);  delay(y);  digitalWrite(led3,LOW);  delay(z);
      digitalWrite(led4,HIGH);  delay(y);  digitalWrite(led4,LOW);  delay(z);
    }
    lcd.clear();  do_update_lcd(1); 
  }
//------ MENU 1 BUTTON 2 ---------------------------------------------------------------
  void do_button12()  { 
    send_msg("M1B2");  
    debounce(2);
    lcd.clear();
    lcd.setCursor (0,0);  lcd.print("BenchDuino      "); 
    lcd.setCursor (0,1);  lcd.print("Dev Board       "); 
    delay(2000);
    lcd.setCursor (0,0);  lcd.print("Design your own "); 
    lcd.setCursor (0,1);  lcd.print("project easily !"); 
    delay(2000);
    lcd.clear();  do_update_lcd(1);
    }
//------ MENU 1 BUTTON 3 ---------------------------------------------------------------
  void do_button13()  {  
    send_msg("M1B3"); 
    debounce(3);
    lcd.clear();
    lcd.setCursor (0,0);  lcd.print("Pot value"); 
    lcd.setCursor (12,1);  lcd.print("EXIT"); 
    while (digitalRead(button4)==HIGH) {
      
      
      int val = 0;  // variable to store the value read
      val = analogRead(analogPin);  // read the input pin
      lcd.setCursor (10,0);  lcd.print("    "); 
      // Following fixes an apparent bug in original code where
      // value of the pot was not echoed to the serial port
      lcd.setCursor (10,0);  lcd.print(String (val)); 
      Serial.println("x=" + String(val));
      delay(500);
      }
      debounce(4);
      lcd.clear();  do_update_lcd(1);
    }
//------ MENU 1 BUTTON 4 ---------------------------------------------------------------
  void do_button14()  { 
    send_msg("M1B4"); 
    debounce(4);
    lcd.clear();
    lcd.setCursor (0,0);  lcd.print("Sending CW      ");   
    lcd.setCursor (0,1);  lcd.print("CQ DE KJ6VU     ");
    sendcw("CQ CQ CQ DE KJ6VU");
    lcd.clear();  do_update_lcd(1);
    }


// Did test Call, all commented out
//---------------------------------------------------------------
//--- RECEIVE Serial1 DATA ---------------------------------------
/* void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '/';
    char endMarker1 = 13;
    char endMarker2 = 10;
    char rc;
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();
        if (recvInProgress == true) {
            if (rc != endMarker1 and rc != endMarker2)  {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }
        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//--- COMMAND PROCESSING  ----------------------------------------
void processpayload() {
  if (newData == true) {
  String str(receivedChars);
  str.toUpperCase();
  stringlen = str.length();
  command = str;
  parser_stringlen = command.length();
  x = command.indexOf(",");
  cmd = command.substring(0,x);
  // Determine how many arguments there are and store it in numargs
  parser_numargs = 0;
  for (int i=0;i<=parser_stringlen;i++) {
    if (command.substring(i,i+1).equals(",")) {
      parser_numargs++;
      parser_argstart[parser_numargs] = i+1;
      parser_argend[parser_numargs-1] = i;
    }
  }

  for (int i=1;i<=parser_numargs-1;i++) {
    x=i;
    arg[i] = command.substring(parser_argstart[i],parser_argend[i]);
  }

    x++;
    arg[x] = command.substring(command.lastIndexOf(",")+1);

//=== COMMAND TABLE =========================================================
  if (dbg==1)  Serial1.println("Command >" + command + "<");
  if (cmd.equals("PING")) { diditflag=1; send_msg("PING!");}
  if (cmd.equals("LCD"))   { do_set_lcd(); }
  if (cmd.equals("MENU"))  { do_set_menu(); }
  
  if (diditflag == 1) {
  //--- ASSEMBLE PAYLOAD --------------------------------------------
  payload = "-";
  //--- ASSEMBLE PACKET ---------------------------------------------
  outdata = payload;                  // Start of packet
  //--- SENT PACKET -------------------------------------------------
  send_msg(outdata);
  digitalWrite(statuspin,HIGH);
  delay (250);
  digitalWrite(statuspin,LOW);
  diditflag = 0;
  }
  diditflag = 0;
  command = "";
  goodpacket = 0;
  directpacket = 0;
  newData = false;
    }
}
*/

//-------------------------------------------------------------------
//-------------------------------------------------------------------
void do_set_led(String mode) {
  // command format:  LCD,1,2,TEXT  LCD,Line number 1..2, character position 1..16, text
  if (mode == "1") {  digitalWrite(statuspin,HIGH);  }  
  if (mode == "0") { /*digitalWrite(statuspin,LOW); */}
  if (mode == "B") { 
     digitalWrite(statuspin,LOW);   delay(200); 
    digitalWrite(statuspin,HIGH);  delay(200); 
    digitalWrite(statuspin,LOW);   delay(50); 
    digitalWrite(statuspin,HIGH);  delay(200); 
    digitalWrite(statuspin,LOW); 
  }
}
//-------------------------------------------------------------------
void do_set_lcd() {
  // command format:  LCD,1,2,TEXT  LCD,Line number 1..2, character position 1..16, text
  if (dbg==1)  /*Serial1.println("arg 1 = " + arg[1] + ", arg 2 = " + arg[2])*/;
  lcd.setCursor (arg[2].toInt()-1,arg[1].toInt()-1);
  lcd.print(arg[3]);
}
//-------------------------------------------------------------------
void debounce(int x)  {
    if (x==1) {do {delay(button_debounce_time);} while (digitalRead(button1)==LOW); } // Debounce the button
    if (x==2) {do {delay(button_debounce_time);} while (digitalRead(button2)==LOW); } // Debounce the button
    if (x==3) {do {delay(button_debounce_time);} while (digitalRead(button3)==LOW); } // Debounce the button
    if (x==4) {do {delay(button_debounce_time);} while (digitalRead(button4)==LOW); } // Debounce the button
}
//-------------------------------------------------------------------
void do_set_menu() {
  // menu[A][B]  A=Slot, B=Row
  if (arg[1]=="1" and arg[2]=="1") {menu[1][2]=arg[3];}
  if (arg[1]=="2" and arg[2]=="2") {menu[1][2]=arg[3];}
  do_update_lcd(current_menu);
}


//=========================================================================
//===== OLED GRAPHICS ROUTINES ============================================
//=========================================================================

//
// Note that the u8g2lib is less fuctional than the original u8glib. So
// Had to comment out several features (i.e. RGB, forground color, etc.)
//
 void u8g_prepare(void) 
{
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
//  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}

void intro(uint8_t a)
{
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr(12, 19, "Welcome");
  u8g.drawStr(12, 38, "To");
  u8g.drawStr(12, 57, "BenchDuino :)");
}

void u8g_box_frame(uint8_t a) 
{
  u8g.drawStr( 12, 0, "BenchDuino");
  u8g.drawBox(5,10,30,20);
  u8g.drawBox(10+a,15,40,17);
  u8g.drawStr( 12, 35, "BenchDuino");
  u8g.drawFrame(5,10+38,20,10);
  u8g.drawFrame(10+a,15+38,30,7);
}

void u8g_disc_circle(uint8_t a) 
{
  u8g.drawStr( 12, 0, "BenchDuino");
  u8g.drawDisc(11,18,9);
  u8g.drawDisc(24+a,16,7);
  u8g.drawStr(12, 35, "BenchDuino");
  u8g.drawCircle(11,18+35,9);
  u8g.drawCircle(24+a,16+35,7);
}

void u8g_r_frame(uint8_t a) 
{
  u8g.drawStr( 12, 0, "BenchDuino");
  u8g.drawRFrame(5, 15,40,30, a+1);
  u8g.drawRBox(50, 15,25,40, a+1);
}

void u8g_string(uint8_t a) 
{
  /*u8g.drawStr270(64,31-a, "BenchDuino");
  u8g.drawStr(64+a,31, "BenchDuino");
  u8g.drawStr90(64,31+a, "BenchDuino");
  u8g.drawStr180(64-a,31, "BenchDuino");*/
  u8g.setFontDirection(3); // 270 degrees
  u8g.drawStr(64,31-a, "BenchDuino");
  u8g.drawStr(64+a,31, "BenchDuino");
  u8g.setFontDirection(1);  //90 degrees
  u8g.drawStr(64,31+a, "BenchDuino");
  u8g.setFontDirection(2);  //180 degrees
  u8g.drawStr(64-a,31, "BenchDuino");
}

void u8g_line(uint8_t a) 
{
  u8g.drawStr( 12, 0, "BenchDuino");
  u8g.drawLine(7+a, 15, 40, 55);
  u8g.drawLine(7+a*2, 15, 60, 55);
  u8g.drawLine(7+a*3, 15, 80, 55);
  u8g.drawLine(7+a*4, 15, 100, 55);
}

void u8g_extra_page(uint8_t a)
{
//  if ( u8g.getMode() == U8G_MODE_HICOLOR || u8g.getMode() == U8G_MODE_R3G3B2) {
    // draw background (area is 128x128) 
 /*   u8g_uint_t r, g, b;
    b = a << 5;
    for( g = 0; g < 64; g++ )
    {
      for( r = 0; r < 64; r++ )
      {
  u8g.setRGB(r<<2, g<<2, b );
  u8g.drawPixel(g, r);
      }
    }
    u8g.setRGB(255,255,255);
    u8g.drawStr( 66, 0, "Color Page");
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT )
  {
    u8g.drawStr( 66, 0, "Gray Level");
    u8g.setColorIndex(1);
    u8g.drawBox(2, 4, 64, 32);    
    u8g.drawBox(70, 20, 4, 12);
    u8g.setColorIndex(2);
    u8g.drawBox(2+1*a, 4+1*a, 64-2*a, 32-2*a);
    u8g.drawBox(74, 20, 4, 12);
    u8g.setColorIndex(3);
    u8g.drawBox(2+2*a, 4+2*a, 64-4*a, 32-4*a);
    u8g.drawBox(78, 20, 4, 12);
  }
  else
  {
  */
    u8g.drawStr( 2, 9, "BenchDuino");
    // u8g.setScale2x2();
    u8g.drawStr( 1,14+a, "CQ CQ CQ");
    // u8g.undoScale();
//  }
}


void draw(void) 
{
  u8g_prepare();
  switch(draw_state >> 3) {
    case 0: intro(draw_state&7); break;
    case 1: u8g_box_frame(draw_state&7); break;
    case 2: u8g_disc_circle(draw_state&7); break;
    case 3: u8g_r_frame(draw_state&7); break;
    case 4: u8g_string(draw_state&7); break;
    case 5: u8g_line(draw_state&7); break;
    case 6: u8g_extra_page(draw_state&7); break;
  }
}






//=========================================================================
//  CW SENDING ROUTINES
//  Based on the simple Morse Beacon by: Mark VandeWettering K6HX http://brainwagon.org
//=========================================================================


void dash()
{
  digitalWrite(led1, HIGH);  digitalWrite(led2, HIGH);  digitalWrite(led3, HIGH); digitalWrite(led4, HIGH);  /* digitalWrite(beeperpin, HIGH); */   
  delay(DASHLEN);
  digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW); digitalWrite(led4, LOW) ;  /* digitalWrite(beeperpin, LOW); */ 
  delay(DOTLEN) ;
}

void dit()
{
  digitalWrite(led1, HIGH);  digitalWrite(led2, HIGH);  digitalWrite(led3, HIGH); digitalWrite(led4, HIGH);  /* digitalWrite(beeperpin, HIGH); */   
  delay(DOTLEN);
  digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW); digitalWrite(led4, LOW) ;  /* digitalWrite(beeperpin, LOW); */
  delay(DOTLEN);
}

void send(char c)
{
  int i ;
  if (c == ' ') {
    Serial.print(c) ;
    delay(7*DOTLEN) ;
    return ;
  }
  for (i=0; i<N_MORSE; i++) {
    if (morsetab[i].c == c) {
      unsigned char p = morsetab[i].pat ;
      Serial.print(morsetab[i].c) ;

      while (p != 1) {
          if (p & 1)
            dash() ;
          else
            dit() ;
          p = p / 2 ;
      }
      delay(2*DOTLEN) ;
      return ;
    }
  }
  /* if we drop off the end, then we send a space */
  Serial.print("?") ;
}

void sendcw(char *str)
{
  while (*str)
    send(*str++) ;
  Serial.println("");
}


// ==============================================================================
// ==============================================================================
// ===== END OF PROGRAM =====
// ==============================================================================
// ==============================================================================
