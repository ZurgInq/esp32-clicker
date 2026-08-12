// Тумблер - таймаут дребезга
const unsigned long toggleSwitcherDebounce = 50;

// Тумблер
struct ToggleSwitcher {
  ToggleSwitcher(int buttonPin);

  // Инициализация выбранного пина в режиме INPUT_PULLUP
  void setup();

  // Считывает и обновляет состояние пина
  bool updateToggleSwitcherState();

  // Возвращает true, если кнопка нажата (пин в LOW)
  bool isToggleSwitcherStateOn();

  int getButtonState();

  private:
    int buttonPin;
    int buttonState;
    int lastButtonState;
    unsigned long lastDebounceTime;

    // digitalRead pin
    int read();
};

// Кликер - считаем количество переключений тумблера
struct Clicker {
  Clicker(int buttonPin);
  void setup();

  ToggleSwitcher switcher;
  int clickCount = 0;
  bool lastSwitcherState = false;
};

// true - если состояние было обновленно
bool updateClicker(Clicker& clicker);
