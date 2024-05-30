#include <Arduino.h>             // Základní knihovna Arduino
#define IR_USE_AVR_TIMER 1       // Definuje použití AVR timeru pro knihovnu IRremote
#include <IRremote.hpp>          // Knihovna pro práci s IR přijímačem

// Definice pinu pro IR přijímač
#define IR_RECEIVE_PIN 12        // Definuje pin pro připojení IR přijímače

// Definice IR kódů pro barvy
#define RED 3208707840           // Definuje IR kód pro červenou barvu (CH+)
#define BLUE 3091726080          // Definuje IR kód pro modrou barvu (>>| (next))
#define GREEN 3158572800         // Definuje IR kód pro zelenou barvu (|>|| (play/pause))

// Definice pinů pro RGB LED
#define blue 9                   // Definuje pin pro modrou LED
#define red 6                    // Definuje pin pro červenou LED
#define green 5                  // Definuje pin pro zelenou LED

// Definice pinů pro LED bar a IR kódů pro čísla 0-9
int bar[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};     // Pole pinů pro LED bar
unsigned long codes[] = {                         // Pole IR kódů pro čísla 0-9
    3910598400, // 0
    4077715200, // 1
    3877175040, // 2
    2707357440, // 3
    4144561920, // 4
    3810328320, // 5
    2774204160, // 6
    3175284480, // 7
    2907897600, // 8
    3041591040, // 9
};

// Počet prvků v poli codes
int codesSize = sizeof(codes) / sizeof(codes[0]); // Zjištění počtu prvků v poli codes

// Proměnná pro uchovávání řetězce čísla
String stringNum = "";         // Inicializace prázdného řetězce pro uchovávání čísla

// Funkce pro nastavení RGB hodnot
void setRGB(int redVal, int greenVal, int blueVal) {
  analogWrite(red, redVal);    // Nastavení hodnoty PWM pro červenou LED
  analogWrite(green, greenVal);// Nastavení hodnoty PWM pro zelenou LED
  analogWrite(blue, blueVal);  // Nastavení hodnoty PWM pro modrou LED
}

void setup() {
  pinMode(red, OUTPUT);        // Nastavení pinu pro červenou LED jako výstupní
  pinMode(green, OUTPUT);      // Nastavení pinu pro zelenou LED jako výstupní
  pinMode(blue, OUTPUT);       // Nastavení pinu pro modrou LED jako výstupní
  
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Inicializace IR přijímače s povolením zpětné vazby LED
  
  Serial.begin(9600);          // Inicializace sériové komunikace s baud rate 9600
}

void loop() {
  if (IrReceiver.decode()) {   // Kontrola, zda byl přijat signál z IR dálkového ovládání
    unsigned long receivedValue = IrReceiver.decodedIRData.decodedRawData; // Získání přijaté hodnoty
    Serial.println(receivedValue); // Výpis přijaté hodnoty na sériový monitor
    
    for (int i = 0; i < 10; i++) { // Porovnání přijaté hodnoty s kódy čísel 0-9
      if (receivedValue == codes[i])
        stringNum += String(i).c_str(); // Přidání čísla do řetězce
    }
    
    if (receivedValue == RED) { // Pokud je přijat kód pro červenou barvu
      int intNum = stringNum.toInt(); // Konverze řetězce na celé číslo
      stringNum = ""; // Vyprázdnění řetězce
      if (intNum >= 0 && intNum <= 255) { // Kontrola, zda je číslo v intervalu 0-255
        Serial.print("Zadaná hodnota: ");
        Serial.println(intNum); // Výpis hodnoty na sériový monitor
        analogWrite(red, intNum); // Nastavení hodnoty PWM pro červenou LED
      } else {
        Serial.println("Zadané číslo není v intervalu 0-255."); // Výpis chyby, pokud číslo není v intervalu
      }
    }

    if (receivedValue == GREEN) { // Pokud je přijat kód pro zelenou barvu
      int intNum = stringNum.toInt(); // Konverze řetězce na celé číslo
      stringNum = ""; // Vyprázdnění řetězce
      if (intNum >= 0 && intNum <= 255) { // Kontrola, zda je číslo v intervalu 0-255
        Serial.print("Zadaná hodnota: ");
        Serial.println(intNum); // Výpis hodnoty na sériový monitor
        analogWrite(green, intNum); // Nastavení hodnoty PWM pro zelenou LED
      } else {
        Serial.println("Zadané číslo není v intervalu 0-255."); // Výpis chyby, pokud číslo není v intervalu
      }
    }

    if (receivedValue == BLUE) { // Pokud je přijat kód pro modrou barvu
      int intNum = stringNum.toInt(); // Konverze řetězce na celé číslo
      stringNum = ""; // Vyprázdnění řetězce
      if (intNum >= 0 && intNum <= 255) { // Kontrola, zda je číslo v intervalu 0-255
        Serial.print("Zadaná hodnota: ");
        Serial.println(intNum); // Výpis hodnoty na sériový monitor
        analogWrite(blue, intNum); // Nastavení hodnoty PWM pro modrou LED
      } else {
        Serial.println("Zadané číslo není v intervalu 0-255."); // Výpis chyby, pokud číslo není v intervalu
      }
    }

    IrReceiver.resume(); // Povolí přijímání dalšího signálu
  }
}
