# Кликер - демо проект для esp32

Управляем простыми html играми через кнопки тумблеры (on/off). Включение или выключение тумблера интерпретируется как зажатие стрелок клавиатуры. Это означает, что отсутствует состояние "кнопка не нажата".

## ESP32 DEVKIT

### Железная часть

* esp32 devkit.
* К esp32 подключено два тумблера.
* Питание от power bank или от USB.

### Скетч

* При включении esp32 подключается к wifi сети (доступ прописывается в исходниках) и выводит свой ip адрес в консоль.
* Поднимается web сервер который предоставляет SSE (Server-sent events) API.
* Внешняя html страница подключается к SSE API для отслеживания состояния тумблеров.

## Зависимости

* [Arduino IDE](https://www.arduino.cc/en/software/)
* [ESPAsyncWebServer](https://esp32async.github.io/ESPAsyncWebServer/)

## Ссылки

* Исходник Pong игры - [Basic Pong HTML and JavaScript Game] https://gist.github.com/straker/81b59eecf70da93af396f963596dfdc5 
