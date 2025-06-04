# English

Module `ruen` lets you send keycodes regardless of the active system language or keyboard layout. All examples and explanations use Russian and English layouts, but you can apply the same approach to any other language pair, for example French and English or Spanish and English.

> [!WARNING]  
> This module is currently in early alpha. Use at your own risk.

## New behaviors

- `&ruen_switch <0/1>` — switch both the system layout and the internal keyboard layout according to your configuration:  
  - `0`: layout → Russian  
  - `1`: layout → English  

- `&ruen_key <en_key> <ru_key>` — send `en_key` if the active language is English, or `ru_key` if it’s Russian:  
  - `en_key`: keycode to send in English layout  
  - `ru_key`: keycode to send in Russian layout  

- `&ruen_one_key <0/1> <key>` — if the current language matches your configuration, send `key`; otherwise, perform: switch → send `key` → switch back:  
  - `0`: send in Russian  
  - `1`: send in English  
  - `key`: the keycode to send  

## Examples

> [!NOTE]  
> Examples assume macOS with English and Russian layouts.

- `&ruen_switch 0` — internal keyboard layout switches to Russian; sends the special hotkey to switch the system to Russian.  
- `&ruen_switch 1` — internal keyboard layout switches to English; sends the special hotkey to switch the system to English.  
- `&ruen_key LS(N5) LS(N4)` — in English layout sends `LShift+5` (prints `%`), in Russian layout sends `LShift+4` (also prints `%`). Always yields `%`.  
- `&ruen_key Q W` — in English layout sends `Q`, in Russian layout sends `W` (Ц). Lets you combine Colemak and the “Dictor” (Russian alternative layout) layouts in one layer without changing system layouts.  
- `&ruen_one_key 1 LS(N3)` — in English layout sends `LShift+3` (`#`); in Russian: switch to English → `LShift+3` → switch back. Always yields `#`.  
- `&ruen_one_key 0 LS(N3)` — in Russian layout sends `LShift+3` (`№`); in English: switch to Russian → `LShift+3` → switch back. Always yields `№`.  

[More examples in my keymap](https://github.com/jewtvet/zmk-cradio/blob/main/config/cradio.keymap)

## Usage on MacOS

1. Install **MLSwitcher2** from Mac App Store and configure separate hotkeys for Russian and English layouts.  
2. In your `config/west.yml`, add these entries under `remotes` and `projects`:
   ```yaml
   manifest:
     remotes:
       - name: zmkfirmware
         url-base: https://github.com/zmkfirmware
       - name: jewtvet
         url-base: https://github.com/jewtvet
     projects:
       - name: zmk
         remote: zmkfirmware
         revision: main
         import: app/west.yml
       - name: zmk-ruen
         remote: jewtvet
         revision: main
     self:
       path: config
   ```
3. In your `config/<your-keyboard>.conf` add these configs:
   ```conf
   CONFIG_SETTINGS=y
   CONFIG_NVS=y
   CONFIG_SETTINGS_NVS=y
   ```
4. In your `config/<your-keyboard>.keymap`, add `#include <behaviors/ruen.dtsi>` to other includes.
5. In the same file, under `macros`, add these entries, update bindings to your own layout-switch hotkeys:
   ```yaml
   ruen_to_en: ruen_to_en {
       compatible = "zmk,behavior-macro";
       #binding-cells = <0>;
       bindings = <&kp LG(N8)>; // Your hotkey to switch to English
   }; 

   ruen_to_ru: ruen_to_ru {
       compatible = "zmk,behavior-macro";
       #binding-cells = <0>;
       bindings = <&kp LG(N9)>; // Your hotkey to switch to Russian
   };
   ```
   Tip: You can include multiple `&kp` entries in the `bindings` section if you need to send several different hotkeys - for example, one for your primary macOS machine and another for a Windows machine accessed via RDP:
   ```yaml
   bindings = <&kp LG(N5)>, <&kp LG(N6)>;
   ```
   In this case, LG(N5) will be sent first, and after a 5 ms delay, LG(N6) will be sent.
> [!WARNING]  
> Do NOT include anything other than &kp in bindings; any other devices will be ignored.
6. Add your new keys into the same keymap file.
7. Add `&ruen_macos 1` to a distant key position in a rarely used layer.
8. Build and flash the firmware to your keyboard.
9. Once press `&ruen_macos 1` to switch `ruen` to macOS compatibility mode, the state will be saved in persistent storage.

## Usage on Windows & Linux

> [!WARNING]  
> This module was only tested on macOS; Windows & Linux support is unverified.

1. Configure separate hotkeys for Russian and English layouts.  
2. In your `config/west.yml`, add these entries under `remotes` and `projects`:
   ```yaml
   manifest:
     remotes:
       - name: zmkfirmware
         url-base: https://github.com/zmkfirmware
       - name: jewtvet
         url-base: https://github.com/jewtvet
     projects:
       - name: zmk
         remote: zmkfirmware
         revision: main
         import: app/west.yml
       - name: zmk-ruen
         remote: jewtvet
         revision: main
     self:
       path: config
   ```
3. In your `config/<your-keyboard>.conf` add these configs:
   ```conf
   CONFIG_SETTINGS=y
   CONFIG_NVS=y
   CONFIG_SETTINGS_NVS=y
   ```
4. In your `config/<your-keyboard>.keymap`, add `#include <behaviors/ruen.dtsi>` to other includes.
5. In the same file, under `macros`, add these entries, update bindings to your own layout-switch hotkeys:
   ```yaml
   ruen_to_en: ruen_to_en {
       compatible = "zmk,behavior-macro";
       #binding-cells = <0>;
       bindings = <&kp LG(N8)>; // Your hotkey to switch to English
   }; 

   ruen_to_ru: ruen_to_ru {
       compatible = "zmk,behavior-macro";
       #binding-cells = <0>;
       bindings = <&kp LG(N9)>; // Your hotkey to switch to Russian
   };
   ```
   Tip: You can include multiple `&kp` entries in the `bindings` section if you need to send several different hotkeys - for example, one for your primary macOS machine and another for a Windows machine accessed via RDP:
   ```yaml
   bindings = <&kp LG(N5)>, <&kp LG(N6)>;
   ```
   In this case, LG(N5) will be sent first, and after a 5 ms delay, LG(N6) will be sent.
> [!WARNING]  
> Do NOT include anything other than &kp in bindings; any other devices will be ignored.
6. Add your new keys into the same keymap file. 
7. Build and flash the firmware to your keyboard.

# Русский

Модуль `ruen` позволяет отправлять кейкоды независимо от выбранного языка и раскладки.

> [!WARNING]
> В настоящий момент модуль находится на ранней альфа‑версии, поэтому используйте его на свой страх и риск.

## Новые behaviors

- `&ruen_switch <0/1>` — переключает язык в системе и в клавиатуре согласно настройке:
  - `0`: раскладка — русский
  - `1`: раскладка — английский

- `&ruen_key <en_key> <ru_key>` — отправляет `en_key`, если текущий язык — английский, и `ru_key`, если текущий язык — русский:
  - `en_key`: клавиша при английской раскладке
  - `ru_key`: клавиша при русской раскладке

- `&ruen_one_key <0/1> <key>` — если текущий язык совпадает с выбранным при конфигурации, отправляет `key`; иначе выполняет: смена языка → отправка `key` → возвращение языка:
  - `0`: при русской раскладке
  - `1`: при английской раскладке
  - `key`: отправляемая клавиша

## Примеры использования

> [!NOTE]
> Примеры приведены для macOS с раскладками English и Russian.

- `&ruen_switch 0` — внутренняя раскладка клавиатуры меняется на русский, отправляет особый хоткей для переключения на русский в системе.
- `&ruen_switch 1` — внутренняя раскладка клавиатуры меняется на английский, отправляет особый хоткей для переключения на английский в системе.
- `&ruen_key LS(N5) LS(N4)` — при английской раскладке отправляет `LShift+5` (% в английской), при русской — `LShift+4` (% в русской). Всегда отправляет `%`.
- `&ruen_key Q W` — при английской раскладке отправляет `Q`, при русской — `W` (Ц). Позволяет объединить Colemak и "Диктор" раскладки в одном слое без изменения системных раскладок.
- `&ruen_one_key 1 LS(N3)` — при английской раскладке отправляет `LShift+3` (#); при русской — переключает на английский → отправляет `LShift+3` → возвращает русский. Всегда отправляет `#`.
- `&ruen_one_key 0 LS(N3)` — при русской раскладке отправляет `LShift+3` (№); при английской — переключает на русский → отправляет `LShift+3` → возвращает английский. Всегда отправляет `№`.

[Больше примеров в моей кеймапе](https://github.com/jewtvet/zmk-cradio/blob/main/config/cradio.keymap)

## Использование с macOS

1. Установите **MLSwitcher2** из Mac App Store и настройте отдельные хоткеи для русской и английской раскладок.
2. Добавьте следующие записи в `remotes` и `projects` в файле `config/west.yml`:
   ```yaml
   manifest:
     remotes:
       - name: zmkfirmware
         url-base: https://github.com/zmkfirmware
       - name: jewtvet
         url-base: https://github.com/jewtvet
     projects:
       - name: zmk
         remote: zmkfirmware
         revision: main
         import: app/west.yml
       - name: zmk-ruen
         remote: jewtvet
         revision: main
     self:
       path: config
   ```
3. В файле `config/<your-keyboard>.conf` добавьте следующие конфигурации:
   ```conf
   CONFIG_SETTINGS=y
   CONFIG_NVS=y
   CONFIG_SETTINGS_NVS=y
   ```
4. В файле `config/<your-keyboard>.keymap`, добавьте `#include <behaviors/ruen.dtsi>` к остальным include.
5. В том же файле, в разделе `macros` добавьте `ruen_to_en` и `ruen_to_ru`:
   ```yaml
   ruen_to_en: ruen_to_en {
       compatible = "zmk,behavior-macro";
       #binding-cells = <0>;
       bindings = <&kp LG(N8)>; // Ваш хоткей для переключения на английский
   }; 

   ruen_to_ru: ruen_to_ru {
       compatible = "zmk,behavior-macro";
       #binding-cells = <0>;
       bindings = <&kp LG(N9)>; // Ваш хоткей для переключения на русский
   };
   ```
   Совет: Вы можете включать несколько `&kp` в список `bindings`, если нужно отправить несколько хоткеев — например, один для основной машины под macOS и другой для Windows-машины, к которой вы подключаетесь через RDP:
   ```yaml
   bindings = <&kp LG(N5)>, <&kp LG(N6)>;
   ```
   В этом случае сначала будет отправлен `LG(N5)`, и через 5 мс — `LG(N6)`.
> [!WARNING]
> Не включайте ничего, кроме `&kp` в `bindings`; любые другие девайсы будут игнорироваться.
6. Добавьте новые клавиши в раскладку в том же файле.
7. Добавьте `&ruen_macos 1` на удаленное место в редко используемом слое. 
8. Соберите и загрузите прошивку на клавиатуру.
9. Один раз нажмите `&ruen_macos 1`, чтобы перевести `ruen` в режим совместимости с macOS, это значение будет сохранено в постоянной памяти.

## Использование с Windows & Linux

> [!WARNING]
> Модуль проверялся только на macOS; работоспособность на Windows & Linux не гарантируется.

1. Настройте отдельные хоткеи для русской и английской раскладок.
2. Добавьте следующие записи в `remotes` и `projects` в файле `config/west.yml`:
   ```yaml
   manifest:
     remotes:
       - name: zmkfirmware
         url-base: https://github.com/zmkfirmware
       - name: jewtvet
         url-base: https://github.com/jewtvet
     projects:
       - name: zmk
         remote: zmkfirmware
         revision: main
         import: app/west.yml
       - name: zmk-ruen
         remote: jewtvet
         revision: main
     self:
       path: config
   ```
3. В файле `config/<your-keyboard>.conf` добавьте следующие конфигурации:
   ```conf
   CONFIG_SETTINGS=y
   CONFIG_NVS=y
   CONFIG_SETTINGS_NVS=y
   ```
4. В файле `config/<your-keyboard>.keymap`, добавьте `#include <behaviors/ruen.dtsi>` к остальным include.
5. В том же файле, в разделе `macros` добавьте `ruen_to_en` и `ruen_to_ru`:
   ```yaml
   ruen_to_en: ruen_to_en {
       compatible = "zmk,behavior-macro";
       #binding-cells = <0>;
       bindings = <&kp LG(N8)>; // Ваш хоткей для переключения на английский
   }; 

   ruen_to_ru: ruen_to_ru {
       compatible = "zmk,behavior-macro";
       #binding-cells = <0>;
       bindings = <&kp LG(N9)>; // Ваш хоткей для переключения на русский
   };
   ```
   Совет: Вы можете включать несколько `&kp` в список `bindings`, если нужно отправить несколько хоткеев — например, один для основной машины под macOS и другой для Windows-машины, к которой вы подключаетесь через RDP:
   ```yaml
   bindings = <&kp LG(N5)>, <&kp LG(N6)>;
   ```
   В этом случае сначала будет отправлен `LG(N5)`, а через 5 мс — `LG(N6)`.
> [!WARNING]
> Не включайте ничего, кроме `&kp` в `bindings`; любые другие девайсы будут игнорироваться.
6. Добавьте новые клавиши в раскладку в том же файле.
7. Соберите и загрузите прошивку на клавиатуру.
