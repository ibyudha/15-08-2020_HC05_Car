// Created by Gus Yudha

#include <AFMotor.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

AF_DCMotor rodaKiri(1);
AF_DCMotor rodaKanan(4);

const int belMobil = 3;
const int lampuBelakang = 5;
const int lampuHias = 6;
const int lampuDek = 10;
const int lampuKanan = 11;
const int lampuKiri = 12;
char tangkapanBluetooth='S';
int kecepatanMobil = 250; // default kecepatan awal


void belokKiri() {
    rodaKiri.run(FORWARD);
    rodaKanan.run(FORWARD);
}
void belokKanan() {
    rodaKiri.run(BACKWARD);
    rodaKanan.run(BACKWARD);
}
void jalanMaju() {
    rodaKiri.run(BACKWARD);
    rodaKanan.run(FORWARD);
}
void jalanMundur() {
    rodaKiri.run(FORWARD);
    rodaKanan.run(BACKWARD);
}
void berhenti() {
    rodaKiri.run(RELEASE);
    rodaKanan.run(RELEASE);
}
void nyalahLampu() {
  digitalWrite ( lampuKanan, HIGH );
  digitalWrite ( lampuKiri, HIGH );
  digitalWrite ( lampuDek, HIGH );
  digitalWrite ( lampuHias, HIGH );
  digitalWrite ( lampuBelakang, HIGH );
}
void matikanLampu() {
  digitalWrite ( lampuKanan, LOW );
  digitalWrite ( lampuKiri, LOW );
  digitalWrite ( lampuDek, LOW );
  digitalWrite ( lampuBelakang, LOW );
  digitalWrite ( lampuHias, LOW );
}
void tambahKecepatan() {
    if ( kecepatanMobil > 250) {
        kecepatanMobil = 250;
        rodaKiri.setSpeed(kecepatanMobil);
        rodaKanan.setSpeed(kecepatanMobil);
    } else {
        kecepatanMobil = kecepatanMobil + 5;
        rodaKiri.setSpeed(kecepatanMobil);
        rodaKanan.setSpeed(kecepatanMobil);
    }
}
void kurangiKecepatan() {
    if ( kecepatanMobil < 50) {
        kecepatanMobil = 50;
        rodaKiri.setSpeed(kecepatanMobil);
        rodaKanan.setSpeed(kecepatanMobil);
    } else {
        kecepatanMobil = kecepatanMobil - 5;
        rodaKiri.setSpeed(kecepatanMobil);
        rodaKanan.setSpeed(kecepatanMobil);
    }
}
void ngebelAktif() {
  digitalWrite ( belMobil, HIGH );
}
void ngebelNonAktif() {
  digitalWrite ( belMobil, LOW );
}
void labelPesan(){
    lcd.setCursor(0,0);
    lcd.print("HALO YANG DISANA");
    delay ( 1000 );
    lcd.setCursor(0,1);
    lcd.print("SAYA ADALAH HC05");
    delay ( 1000 );
    lcd.setCursor(0,0);
    lcd.print("DICIPTAKAN OLEH:");
    delay ( 1000 );
    lcd.setCursor(0,1);
    lcd.print(" YUDHA  WIRAWAN ");
    delay ( 2000 );
    lcd.setCursor(0,0);
    lcd.print(" KONTAK PERSON: ");
    delay ( 1000 );
    lcd.setCursor(0,1);
    lcd.print("= 082144573238 =");
    delay ( 1000 );
    lcd.setCursor(0,0);
    lcd.print("  KUNJUNGILAH!  ");
    delay ( 1000 );
    lcd.setCursor(0,1);
    lcd.print("OFFICIAL WEBSITE");
    delay ( 1000 );
    lcd.setCursor(0,0);
    lcd.print("  yudhawirawan  ");
    delay ( 1000 );
    lcd.setCursor(0,1);
    lcd.print("     .rf.gd     ");
    delay ( 2000 );
    lcd.clear();
}

void setup()
{

    Serial.begin(9600);
    rodaKiri.setSpeed(kecepatanMobil);
    rodaKanan.setSpeed(kecepatanMobil);

    lcd.init();
    lcd.backlight();

    labelPesan();
    pinMode( lampuKanan, OUTPUT );
    pinMode( lampuKiri, OUTPUT );
    pinMode( lampuDek, OUTPUT );
    pinMode( lampuHias, OUTPUT );
    pinMode( lampuBelakang, OUTPUT );
    pinMode( belMobil, OUTPUT );
    berhenti();
}


void loop() {
  
    tangkapanBluetooth=Serial.read();
    if(tangkapanBluetooth=='A') {
        lcd.noBacklight();
        lcd.setCursor(0,0);
        lcd.print("=== PERINTAH ===");
        lcd.setCursor(0,1);
        lcd.print("   PUTAR KIRI   ");
        belokKiri();
    }
    if(tangkapanBluetooth=='D') {
        lcd.noBacklight();
        lcd.setCursor(0,0);
        lcd.print("=== PERINTAH ===");
        lcd.setCursor(0,1);
        lcd.print("  PUTAR KANAN  ");
        belokKanan(); 
    }
    if(tangkapanBluetooth=='W') {
        lcd.noBacklight();
        lcd.setCursor(0,0);
        lcd.print("=== PERINTAH ===");
        lcd.setCursor(0,1);
        lcd.print("   JALAN MAJU   ");
        jalanMaju(); 
    }
    if(tangkapanBluetooth=='S') {
        lcd.noBacklight();
        lcd.setCursor(0,0);
        lcd.print("=== PERINTAH ===");
        lcd.setCursor(0,1);
        lcd.print("  JALAN MUNDUR  ");
        jalanMundur(); 
    }
    if(tangkapanBluetooth=='B') {
        berhenti(); 
    }
    if (tangkapanBluetooth=='P') {
        nyalahLampu();
        delay(500);
        matikanLampu();
        delay(500);
        nyalahLampu;
        tambahKecepatan();
    }
    if (tangkapanBluetooth=='M') {
        nyalahLampu();
        delay(500);
        matikanLampu();
        delay(500);
        nyalahLampu;
        tambahKecepatan();
        kurangiKecepatan();
    }
    if (tangkapanBluetooth=='F'){
        nyalahLampu();
        lcd.backlight();
        lcd.setCursor(0,0);
        lcd.print(" (===)    (===) ");
        lcd.setCursor(0,1);
        lcd.print("      ____      ");
    }
    if (tangkapanBluetooth=='G'){
        matikanLampu();
        lcd.noBacklight();
        lcd.setCursor(0,0);
        lcd.print("===   HC05   ===");
        lcd.setCursor(0,1);
        lcd.print("     TERJAGA    ");
    }
    if (tangkapanBluetooth=='H'){
        ngebelAktif();
    }
    if (tangkapanBluetooth=='J'){
        ngebelNonAktif();
    }
}
