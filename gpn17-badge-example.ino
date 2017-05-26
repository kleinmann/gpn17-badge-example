#include <GPNBadge.hpp>
#include <FS.h>
#include <BadgeUI.h>
#include <UIThemes.h>

#include "rboot.h"
#include "rboot-api.h"

Badge badge;
WindowSystem* ui = new WindowSystem(&tft);
Menu * mainMenu = new Menu();

void setup() {
  badge.init();

  SPIFFS.begin();

  badge.setBacklight(true);
  ui->root->setBmp("/deflt/load.bmp", 40, 12);
  ui->root->setSub("");
  ui->draw();

  // TODO: Is this needed?
  badge.setGPIO(IR_EN, LOW);
  badge.setAnalogMUX(MUX_JOY);

  rboot_config rboot_config = rboot_get_config();
  File f = SPIFFS.open("/rom" + String(rboot_config.current_rom), "w");
  f.println("Test-ROM\n");

  mainMenu->addMenuItem(new MenuItem("My Badge", []() {
    FullScreenBMPDisplay* badge_screen = new FullScreenBMPDisplay();
    badge_screen->setBmp("/badge.bmp");
    ui->open(badge_screen);
  }));
  mainMenu->addMenuItem(new MenuItem("Second Item", []() {
  }));

  ui->open(mainMenu);
}

void loop() {
  ui->dispatchInput(badge.getJoystickState());
  ui->draw();
  pixels.show();
}
