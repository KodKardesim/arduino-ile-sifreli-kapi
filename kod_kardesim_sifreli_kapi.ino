/*

    KOD KARDEŞİM ŞİFRELİ KAPI PROJESİ.
    https://www.kodkardesim.site/arduino-ile-sifreli-kapi-yapalim.html

*/

#include <Keypad.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h>

/******************** Değişkenlerimizi tanımladık ********************/

const byte satir = 4;
const byte sutun = 3;
char Keys[satir][sutun] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte satirPin[satir] = {13, 12, 11, 10};
byte sutunPin[sutun] = {9, 8, 7};
int motor = 5;
int led = 4;
 
Servo MyServo;
Keypad MyKeypad = Keypad(makeKeymap(Keys), satirPin, sutunPin, satir, sutun);
Adafruit_NeoPixel MyStrip = Adafruit_NeoPixel(4, led, NEO_GRB + NEO_KHZ800);

String sifre = "2647";
String gelenSifre;
int denemeSayisi = 0;

void setup() {
  Serial.begin(9600);
  MyServo.attach(motor);
  MyServo.write(0);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  MyStrip.begin();
  MyStrip.show();

}

void loop() {
  char gelenTus = MyKeypad.getKey();

  if (gelenTus) { // Tuşa basılmışsa
    Serial.println(gelenTus);
    gelenSifre = gelenSifre + gelenTus;
    MyStrip.setPixelColor(denemeSayisi, MyStrip.Color(0, 0, 255)); // Ledimizi ayarladık.
    MyStrip.show(); // Ledimizi yaktık.
    denemeSayisi++;
    if (denemeSayisi == 4) { // Eğer 4 hane yazılmışsa
      if (sifre == gelenSifre) { // Eğer şifre doğruysa
        Serial.println("Şifre doğru");
        ledYak(true);
        MyServo.write(180); // Kapıyı aç.
        delay(1500); // 1.5 saniye bekle.
        MyServo.write(1); // Kapıyı kapat.
      } else { // Eğer şifre hatalıysa
        Serial.println("Şifre Hatalı");
        ledYak(false);
      }
      gelenSifre = "";  // Gelen şifreyi sıfırla.
      denemeSayisi = 0; // Deneme sayısını sıfırla.
    }
  }
}

void ledYak(bool durum) {
  if (durum == true) { // Şifre doğru ise çalışacak fonksiyon.
    /* Ledi 5 defa 50ms arayla yeşil yak söndür. */
    for (int a = 0; a < 5; a++) {
      for (int i = 0; i < MyStrip.numPixels(); i++) {
        MyStrip.setPixelColor(i, MyStrip.Color(0, 255, 0));
        MyStrip.show();
      }
      delay(50);
      for (int i = 0; i < MyStrip.numPixels(); i++) {
        MyStrip.setPixelColor(i, MyStrip.Color(0, 0, 0));
        MyStrip.show();
      }
      delay(50);
    }
  } else { // Şifre yanlış ise çalışacak fonksiyon.
    /* Ledi 5 defa 50ms arayla kırmızı yak söndür. */
    for (int a = 0; a < 5; a++) {
      for (int i = 0; i < MyStrip.numPixels(); i++) {
        MyStrip.setPixelColor(i, MyStrip.Color(255, 0, 0));
        MyStrip.show();
      }
      delay(50);
      for (int i = 0; i < MyStrip.numPixels(); i++) {
        MyStrip.setPixelColor(i, MyStrip.Color(0, 0, 0));
        MyStrip.show();
      }
      delay(50);
    }
  }
}
