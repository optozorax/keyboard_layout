# Моя раскладка для ErgoDox EZ

# Инструменты

## QMK Typing Statistics CLI

# Настройка 

# Костыли

Для того, чтобы после автошифта его индикатор горел ровно необходимое число секунд, нужно добавить следующий код в qmk:

В `tmk_core/common/keyboard.h`
```c++
// ...
void user_timer(void);
// ...
```

В `tmk_core/common/keyboard.c`
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