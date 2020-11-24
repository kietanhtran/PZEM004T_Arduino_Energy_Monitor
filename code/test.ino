#include "LiquidCrystal.h"
#include "PZEM004T.h"
#include "TU_TAO.h"
int count = 220;
int tt = 0;
const int up = 0, dw = 1, mode = 2, role = 9, loa = 13;
const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 7, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
float v, i, p, e;
int v_float, v_float_dv, v_dv, v_ch, v_tr;
int i_float, i_float_dv, i_float_ch, i_dv, i_ch;
int p_float, p_dv, p_ch, p_tr, p_ng;
int e_float, e_float_dv, e_dv, e_ch, e_tr;
PZEM004T pzem(11, 10); // (RX,TX) connect to TX,RX of PZEM
IPAddress ip(192, 168, 1, 1);
void setup() {
  Serial.begin(9600);
  pzem.setAddress(ip);
  pinMode(up, INPUT);
  pinMode(dw, INPUT);
  pinMode(mode, INPUT);
  pinMode(role, OUTPUT);
  pinMode(loa, OUTPUT);
  digitalWrite(loa, LOW);
  digitalWrite(role, LOW);
  digitalWrite(mode, HIGH);
  digitalWrite(dw, HIGH);
  digitalWrite(up, HIGH);
  lcd.begin(20, 4);
  lcd.clear();
  lcd.createChar(1, LT);
  lcd.createChar(2, UB);
  lcd.createChar(3, RT);
  lcd.createChar(4, LL);
  lcd.createChar(5, LB);
  lcd.createChar(6, LR);
  lcd.createChar(7, MB);
  lcd.createChar(8, BLOCK);
}

void loop() {


  if (digitalRead(up) == LOW)
  {
    delay(20);
    if (digitalRead(up) == LOW)
    {
      count++;
    }
  } while (digitalRead(up) == LOW);

  if (digitalRead(dw) == LOW)
  {
    delay(20);
    if (digitalRead(dw) == LOW)
    {
      count--;
    }
  } while (digitalRead(dw) == LOW);

  if (digitalRead(mode) == LOW)
  {
    delay(20);
    if (digitalRead(mode) == LOW)
    {
      tt++;
    }
  } while (digitalRead(mode) == LOW);
  if (tt == 8) tt = 0;
  switch (tt)
  {
    case 0:
      do_dien_ap();
      break;
    case 1:
      lcd.clear();
      tt++;
      break;
    case 2:
      do_dong_dien();
      break;
    case 3:
      lcd.clear();
      tt++;
      break;
    case 4:
      do_cong_suat();
      break;
    case 5:
      lcd.clear();
      tt++;
      break;
    case 6:
      do_dien_nang();
      break;
    case 7:
      lcd.clear();
      tt++;
      break;
  }
}
void do_dien_ap() {
  v = pzem.voltage(ip);
  if (v < 0.0) v = 0.0;
  if (v > 200 && v < 215) {
    digitalWrite(role, HIGH);
    digitalWrite(loa, HIGH);
  }
  else if (v > count) {
    digitalWrite(role, HIGH);
    digitalWrite(loa, HIGH);
  }
  else {
    digitalWrite(role, LOW);
    digitalWrite(loa, LOW);
  }
  Serial.println(v);
  v_float = v * 10; //2
  v_float_dv = v_float % 10;
  v_dv = v_float / 10 % 10;
  v_ch = v_float / 100 % 10;
  v_tr = v_float / 1000 % 10;
  lcd.setCursor(1, 0);
  lcd.print("DIEN AP:");
  printDigits(v_tr, 1, 1)  ;
  printDigits(v_ch, 5, 1)  ;
  printDigits(v_dv, 9, 1)  ;
  lcd.setCursor(13, 2);
  lcd.write(5);
  printDigits(v_float_dv, 15, 1)  ;
  lcd.setCursor(15, 3);
  lcd.print("VON");
  lcd.setCursor(1, 3);
  lcd.print("SET: ");
  lcd.setCursor(6, 3);
  lcd.print(count);
}
void do_dong_dien() {
  i = pzem.current(ip);
  if (i < 0.0) i = 0.0;
  i_float = i * 100; //2202   0.05
  i_float_dv = i_float % 10;
  i_float_ch = i_float / 10 % 10;
  i_dv = i_float / 100 % 10;
  i_ch = i_float / 1000 % 10;
  lcd.setCursor(1, 0);
  lcd.print("DONG DIEN:");
  printDigits(i_ch, 1, 1)  ;
  printDigits(i_dv, 5, 1)  ;
  lcd.setCursor(9, 2);
  lcd.write(5);
  printDigits(i_float_ch, 11, 1)  ;
  printDigits(i_float_dv, 15, 1)  ;
  lcd.setCursor(15, 3);
  lcd.print("AMPE");
}
void do_cong_suat() {
  p = pzem.power(ip);
  if (p < 0.0) p = 0.0;
  p_float = p ; //2202
  p_dv = p_float % 10;
  p_ch = p_float / 10 % 10;
  p_tr = p_float / 100 % 10;
  p_ng = p_float / 1000 % 10;
  lcd.setCursor(1, 0);
  lcd.print("CONG SUAT:");
  printDigits(p_ng, 1, 1)  ;
  printDigits(p_tr, 5, 1)  ;
  printDigits(p_ch, 9, 1)  ;
  printDigits(p_dv, 13, 1)  ;
  lcd.setCursor(15, 3);
  lcd.print("WATT");
}
void do_dien_nang() {
  e = pzem.energy(ip);
  if (e < 0.0) e = 0.0;
  e_float = e * 10; //2202
  e_float_dv = e_float % 10;
  e_dv = e_float / 10 % 10;
  e_ch = e_float / 100 % 10;
  e_tr = e_float / 1000 % 10;
  lcd.setCursor(1, 0);
  lcd.print("TIEU THU:");
  printDigits(e_tr, 1, 1)  ;
  printDigits(e_ch, 5, 1)  ;
  printDigits(e_dv, 9, 1)  ;
  lcd.setCursor(13, 2);
  lcd.write(5);
  printDigits(e_float_dv, 15, 1)  ;
  lcd.setCursor(15, 3);
  lcd.print("Wh");
}

void custom0(int x, int y) {

  lcd.setCursor(x, y);
  lcd.write(1);
  lcd.write(2);
  lcd.write(3);
  lcd.setCursor(x, y + 1);
  lcd.write(4);
  lcd.write(5);
  lcd.write(6);
}

void custom1(int x, int y) {
  lcd.setCursor(x, y);
  lcd.write(2);
  lcd.write(3);
  lcd.print(" ");
  lcd.setCursor(x, y + 1);
  lcd.write(5);
  lcd.write(8);
  lcd.write(5);
}

void custom2(int x, int y) {
  lcd.setCursor(x, y);
  lcd.write(7);
  lcd.write(7);
  lcd.write(3);
  lcd.setCursor(x, y + 1);
  lcd.write(4);
  lcd.write(5);
  lcd.write(5);
}

void custom3(int x, int y) {
  lcd.setCursor(x, y);
  lcd.write(7);
  lcd.write(7);
  lcd.write(3);
  lcd.setCursor(x, y + 1);
  lcd.write(5);
  lcd.write(5);
  lcd.write(6);
}

void custom4(int x, int y) {
  lcd.setCursor(x, y);
  lcd.write(4);
  lcd.write(5);
  lcd.write(8);
  lcd.setCursor(x, y + 1);
  lcd.print(" ");
  lcd.print(" ");
  lcd.write(8);
}

void custom5(int x, int y) {
  lcd.setCursor(x, y);
  lcd.write(4);
  lcd.write(7);
  lcd.write(7);
  lcd.setCursor(x, y + 1);
  lcd.write(5);
  lcd.write(5);
  lcd.write(6);
}

void custom6(int x, int y) {
  lcd.setCursor(x, y);
  lcd.write(1);
  lcd.write(7);
  lcd.write(7);
  lcd.setCursor(x, y + 1);
  lcd.write(4);
  lcd.write(5);
  lcd.write(6);
}

void custom7(int x, int y) {
  lcd.setCursor(x, y);
  lcd.write(2);
  lcd.write(2);
  lcd.write(3);
  lcd.setCursor(x, y + 1);
  lcd.print(" ");
  lcd.print(" ");
  lcd.write(8);
}

void custom8(int x, int y) {
  lcd.setCursor(x, y);
  lcd.write(1);
  lcd.write(7);
  lcd.write(3);
  lcd.setCursor(x, y + 1);
  lcd.write(4);
  lcd.write(5);
  lcd.write(6);
}

void custom9(int x, int y) {
  lcd.setCursor(x, y);
  lcd.write(1);
  lcd.write(7);
  lcd.write(3);
  lcd.setCursor(x, y + 1);
  lcd.write(5);
  lcd.write(5);
  lcd.write(6);

}
void printDigits(int digits, int x, int y) {
  switch (digits) {
    case 0:
      custom0(x, y);
      break;
    case 1:
      custom1(x, y);
      break;
    case 2:
      custom2(x, y);
      break;
    case 3:
      custom3(x, y);
      break;
    case 4:
      custom4(x, y);
      break;
    case 5:
      custom5(x, y);
      break;
    case 6:
      custom6(x, y);
      break;
    case 7:
      custom7(x, y);
      break;
    case 8:
      custom8(x, y);
      break;
    case 9:
      custom9(x, y);
      break;

  }
}


