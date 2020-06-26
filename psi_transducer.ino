#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SerialMenu.hpp>
#include <EEPROM.h>

#define COPYRIGHT "Configuration Menu"
#define Width 128
#define Height 64
#define pruebatransductor 1
#define transductorPin 0
#define in6 6
#define in7 7
#define in8 8
#define outPression1 11
#define outPression2 12
#define outPression3 10
#define OLED_RESET 4

bool unit = false;
bool keepSettings = false;
int estado1, estado2 ;
float lectura, presion, diferencia, m, b, offset, scaleRes, scaleMin, scaleMax, inputRes;
float inputMin = 104;
float inputMax = 958;

//EEPROM Address Location
int addr1 = 1, addr2 = 2, addr3 = 3, addr4 = 4, addr5 = 5, addr6 = 6, addr7 = 7;
float value = 0;
float lowP1 = EEPROM.read(addr1);
float highP1 = EEPROM.read(addr2);
float lowP2 = EEPROM.read(addr3);
float highP2 = EEPROM.read(addr4);
float lowP3 = EEPROM.read(addr5);
float highP3 = EEPROM.read(addr6);
int printOut = EEPROM.read(addr7);

constexpr int loopDelayMs = 250;

const SerialMenu& menu = SerialMenu::get();

Adafruit_SSD1306 oled(Width, Height, &Wire, OLED_RESET);

const uint8_t LOGO_VI [] PROGMEM =
{
  // 'vi automation', 128x64px
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xe0, 0x03, 0xff, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xe0, 0x07, 0xff, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xf0, 0x07, 0xff, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xf0, 0x0f, 0xfe, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x0f, 0xfe, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x1f, 0xfc, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfc, 0x1f, 0xf8, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xfc, 0x3f, 0xf8, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xfe, 0x3f, 0xf0, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x7f, 0xe0, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0x7f, 0xe0, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xc0, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x03, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0xe0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xfe, 0x38, 0xef, 0xc7, 0xe3, 0xfc, 0xf1, 0xfc, 0xfe, 0xe3, 0xf0, 0xff, 0x00, 0x00,
  0x00, 0x00, 0xff, 0x38, 0xef, 0xcf, 0xf3, 0xff, 0xf9, 0xfe, 0xfe, 0xe7, 0xf8, 0xff, 0x80, 0x00,
  0x00, 0x00, 0x07, 0x38, 0xe7, 0x1e, 0x7b, 0xdf, 0x78, 0x0e, 0x78, 0xe7, 0x3c, 0xf7, 0x80, 0x00,
  0x00, 0x00, 0x37, 0x38, 0xe7, 0x1c, 0x3b, 0xce, 0x38, 0xee, 0x70, 0xef, 0x1c, 0xe7, 0x80, 0x00,
  0x00, 0x00, 0xff, 0x38, 0xe7, 0x1c, 0x3b, 0x8e, 0x39, 0xfe, 0x70, 0xee, 0x1c, 0xe3, 0x80, 0x00,
  0x00, 0x01, 0xe7, 0x39, 0xe7, 0x1c, 0x3b, 0x8e, 0x3b, 0x8e, 0x70, 0xef, 0x1c, 0xe3, 0x80, 0x00,
  0x00, 0x01, 0xe7, 0x3f, 0xe7, 0xdf, 0xf3, 0x8e, 0x3b, 0xce, 0x7c, 0xe7, 0xfc, 0xe3, 0x80, 0x00,
  0x00, 0x00, 0xff, 0x3f, 0xe7, 0xcf, 0xf3, 0x8e, 0x39, 0xfe, 0x7e, 0xe3, 0xf8, 0xe3, 0x80, 0x00,
  0x00, 0x00, 0x77, 0x1c, 0xe3, 0xc3, 0xc1, 0x8e, 0x38, 0xee, 0x1c, 0xe1, 0xe0, 0xe3, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

///////////////////////////////////////////////////////////////////////////////
// Main menu
///////////////////////////////////////////////////////////////////////////////

// Forward declarations for the sub-menu referenced before it is defined.
extern const SerialMenuEntry subMenu[];
extern const uint8_t subMenuSize;

// Define the main menu
const SerialMenuEntry mainMenu[] = {
  { "1 - Monitor Output", false, '1', []() {
      EEPROM.write(addr7, 0);
      Serial.println(lowP1);
      Serial.println(highP1);
      Serial.println(lowP2);
      Serial.println(highP2);
      Serial.println(lowP3);
      Serial.println(highP3);
    }
  },
  { "2 - Print Pressure", false, '2', []() {
      EEPROM.write(addr7, 1);
    }
  },
  { "> - Output(s) Configuration",  false, '>', []() {
      EEPROM.write(addr7, 0);
      menu.load(subMenu, subMenuSize);
      menu.show();
    }
  },
  { "M - Reload Main Menu", false, 'm', []() {
      EEPROM.write(addr7, 0);
      menu.show();
    }
  }
};
constexpr uint8_t mainMenuSize = GET_MENU_SIZE(mainMenu);


///////////////////////////////////////////////////////////////////////////////
// Sub-menu
///////////////////////////////////////////////////////////////////////////////

void(* resetFunc) (void) = 0; //declare reset function @ address 0

// For this menu most entries are in PROGMEM Flash, and the last two in SRAM
// which is the normal default
const char subMenuStr1[] PROGMEM = "A - Low Pressure Output #1";
const char subMenuStr2[] PROGMEM = "B - High Pressure Output #1";
const char subMenuStr3[] PROGMEM = "C - Low Pressure Output #2";
const char subMenuStr4[] PROGMEM = "D - Low Pressure Output #2";
const char subMenuStr5[] PROGMEM = "E - High Pressure Output #3";
const char subMenuStr6[] PROGMEM = "F - Low Pressure Output #3";
const char subMenuStr7[] = "R - Save all Configuration(s)";
//const char subMenuStr9[] = "I - Input a value";

// Define the sub-menu
// The last two menu entries declare their string directly
const SerialMenuEntry subMenu[] = {
  { subMenuStr1, true, 'a', []() {
      EEPROM.write(addr1, menu.getNumber<float>("Input value: "));
    }
  },
  { subMenuStr2, true, 'b', []() {
      EEPROM.write(addr2, menu.getNumber<float>("Input value: "));
    }
  },
  { subMenuStr3, true, 'c', []() {
      EEPROM.write(addr3, menu.getNumber<float>("Input value: "));
    }
  },
  { subMenuStr4, true, 'd', []() {
      EEPROM.write(addr4, menu.getNumber<float>("Input value: "));
    }
  },
  { subMenuStr5, true, 'e', []() {
      EEPROM.write(addr5, menu.getNumber<float>("Input value: "));
    }
  },
  { subMenuStr6, true, 'f', []() {
      EEPROM.write(addr6, menu.getNumber<float>("Input value: "));
    }
  },
  //  {subMenuStr7, true, 'r', [](){ Serial.println(++value); }},
  { subMenuStr7, false, 'r', []() {
      resetFunc();
    }
  },
  { "M - Menu",  false, 'm',
    []() {
      menu.show();
    }
  },
  { "< - Main",  false, '<',
    []() {
      menu.load(mainMenu, mainMenuSize);
      menu.show();
    }
  }
};
constexpr uint8_t subMenuSize = GET_MENU_SIZE(subMenu);

void setup() {

  Serial.begin(9600);
  // Set the main menu as the current active menu
  menu.load(mainMenu, mainMenuSize);
  // Display the current menu
  menu.show();
  Serial.println(COPYRIGHT);
  while (!Serial) {};

  Wire.begin();
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  pinMode(in6, INPUT);
  pinMode(in7, INPUT);
  pinMode(in8, INPUT);
  pinMode(outPression1, OUTPUT);
  pinMode(outPression2, OUTPUT);
  pinMode(outPression3, OUTPUT);
  oled.clearDisplay();
  oled.drawBitmap(0, 0, LOGO_VI, 128, 64, WHITE);
  oled.display();
  delay(4000);
  oled.clearDisplay();
}

void loop()
{
  readTransducer();
  menu.run(loopDelayMs);
  delay(loopDelayMs);

  if (printOut == 1) {
    Serial.println(presion);
  }

}

void readTransducer() {

  lectura = analogRead(transductorPin);
  unit = digitalRead(in7);

  if ((unit) == true) {
    scaleMax = 10; //psi o bar
    oled.clearDisplay();
    oled.setTextColor(WHITE);
    oled.setCursor(0, 4);
    oled.setTextSize(3);
    oled.print("BAR");
  }
  else
  {
    scaleMax = 145; //psi o bar
    oled.clearDisplay();
    oled.setTextColor(WHITE);
    oled.setCursor(0, 4);
    oled.setTextSize(3);
    oled.print("PSI");
  }

  // y = mx +b (Scale with parameteres)
  scaleRes = (scaleMax - scaleMin);
  inputRes = (inputMax - inputMin);
  m = (scaleRes / inputRes);
  b = scaleMin - (inputMin * m);
  presion = (m * lectura) + b;

  oled.setCursor(0, 38);
  oled.println(presion);

  checkLimitsPressure(presion, highP1, lowP1, outPression1, 62, 7, 59, "1" );
  checkLimitsPressure(presion, highP2, lowP2, outPression2, 82, 7, 79, "2" );
  checkLimitsPressure(presion, highP3, lowP3, outPression3, 102, 7, 99, "3" );

  oled.display();
  delay(250);
}

void checkLimitsPressure(float pressure, float highPressure, float lowPressure, bool out, int x, int y, int rectangle, char toPrint) {
  if (pressure < highPressure) {
    if (pressure > lowPressure) {
      digitalWrite(out, HIGH);
      oled.setCursor(x, y);
      oled.drawRect(rectangle, 3, 17, 21, WHITE);
      oled.setTextSize(2);
      oled.print(toPrint);
    } else
    {
      digitalWrite(out, LOW);
    }
  }
  else
  {
    digitalWrite(out, LOW);
  }
}
