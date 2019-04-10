# Моя раскладка для ErgoDox EZ

![](https://funkyimg.com/i/2LSSg.png)

Фичи раскладки:

* Раскладка для английского: модифицированный дворак для программистов; раскладка для русского: модифицированный йцукен с элементами дворака для программистов. Раскладки задаются в клавиатуре.
* Работает на любом компьютере, где имеются стандартные раскладки QWERTY, ЙЦУКЕН, где одна раскладка для всех окон, переключение языка на Alt+Shift.
* Inverted Shift модификатор для клавиш - это такой модификатор, при котором, если нажат шифт, для этой клавиши шифт отпускается, а затем снова возвращается. Он используется на шифтовом слое английской и русской раскладки, в частности чтобы набирать цифры при зажатом шифте.
* Залипающий шифт - если нажать шифт, а затем отпустить, то он применится к следующей клавише, если прошло менее 1 секунды.
* Запятая с автопробелом.
* Точка с автопробелом и автозалипающим шифтом.
* При нажатии Ctrl, Alt, Win, включается английский слой, и соответственно все модификаторы нажимаются так, как будто сейчас английская раскладка, даже если на самом деле русская.
* Текущий слой, язык или действие залипающего шифта показывается при помощи светодиодов.

[Аккаунт на клавогонках.](http://klavogonki.ru/u/#/517589/)

# Инструменты

## Введение

На самом деле клавиатура способна не только передавать нажатые клавиши, но ещё и писать в консоль при помощи `print.h`. Для того, чтобы посмотреть что она пишет, необходима программа `hid_listener`, бинарники и исходники которой можно скачать на сайте: [https://www.pjrc.com/teensy/hid_listen.html](https://www.pjrc.com/teensy/hid_listen.html). 

В последующих инструментах общение с клавиатурой будет реализовано через эту программу, так что необходимо поместить её в папку с программой-инструментом.

## Настройки своей раскладки

Для работы инструментов необходимо, чтобы клавиатура писала текущую нажатую клавишу при помощи `uprint`. Необходимо добавить следующий код в `keymap.c` в функцию `process_record_user`:
```c++
#include <print.h>

// ...

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  #ifdef CONSOLE_ENABLE
  uprintf("KL: col=%d, row=%d, pressed=%d, layer=%d\n", record->event.key.col, record->event.key.row, record->event.pressed, biton32(layer_state));
  #endif

// ...
```

И включить консоль в `rules.mk`:
```c++
CONSOLE_ENABLE = yes
```

## Mini Typing Statistics

Инструмент для отображения нажатой клавиши в реальном времени. Показывает положение в матрице, а так же слой, время нажатия (duration) и временное смещение относительно начала предыдущего нажатия (offset).

Компиляция:
```
cd tools
make mini_typing_statistics
./mini_typing_statistics
```

Пример работы:
```
col=2, row=9, layer=0, offset=0ms, duration=86ms
col=2, row=3, layer=0, offset=100ms, duration=100ms
col=1, row=12, layer=0, offset=149ms, duration=50ms
col=1, row=12, layer=0, offset=150ms, duration=49ms
col=2, row=2, layer=0, offset=99ms, duration=89ms
col=5, row=10, layer=0, offset=100ms, duration=148ms
col=3, row=10, layer=0, offset=199ms, duration=32ms
col=2, row=2, layer=0, offset=50ms, duration=50ms
col=1, row=11, layer=0, offset=99ms, duration=99ms
col=1, row=12, layer=0, offset=199ms, duration=100ms
col=2, row=8, layer=0, offset=49ms, duration=100ms
col=5, row=10, layer=0, offset=200ms, duration=150ms
col=2, row=0, layer=0, offset=450ms, duration=2449ms # Это клавиша включения слоя, и она зажата на протяжении всех следующих клавиш, поэтому и duration такое большое 
col=2, row=9, layer=4, offset=299ms, duration=50ms
col=2, row=9, layer=4, offset=150ms, duration=49ms
col=2, row=9, layer=4, offset=99ms, duration=50ms
col=2, row=9, layer=4, offset=150ms, duration=50ms
col=2, row=9, layer=4, offset=100ms, duration=49ms
col=2, row=9, layer=4, offset=99ms, duration=50ms
col=2, row=9, layer=4, offset=149ms, duration=50ms
col=2, row=9, layer=4, offset=81ms, duration=19ms
col=2, row=9, layer=4, offset=119ms, duration=49ms
col=2, row=9, layer=4, offset=99ms, duration=50ms
col=2, row=9, layer=4, offset=100ms, duration=99ms
col=2, row=10, layer=4, offset=550ms, duration=99ms
```

# Костыли

Для того, чтобы после автошифта его индикатор горел ровно необходимое число секунд, нужно добавить следующий код в qmk:

В `tmk_core/common/keyboard.h`:
```c++
// ...
void user_timer(void);
// ...
```

В `tmk_core/common/keyboard.c`:
```c++
// ...
void keyboard_task(void)
{
	// ...

    user_timer();
}

__attribute__((weak))
void user_timer() {
}
// ...
```