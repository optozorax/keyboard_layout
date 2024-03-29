# Данный репозиторий устарел

**Внимание!** Раскладка в данном репозиториии морально устарела, и моя текущая раскладка находится в репозитории [moonlander](https://github.com/optozorax/moonlander). Заходите туда, она реально крута, и про неё есть отличная статья.

# Моя раскладка для ErgoDox EZ на QMK

## Введение в контекст

Данная секция написана для людей не знающих ничего о програмируемых и эргономичных клавиатурах.

**Программируемая клавиатура** - клавиатура, обладающая микроконтроллером, на который можно залить произвольную программу.

**Прошивка** - программа, которая заливается на клавиатуру. В данном репозитории как раз содержится часть этой прошивки, а именно моя часть кода раскладки.

Благодаря тому, что на клавиатуру можно залить произвольную программу, прямо на клавиатуре можно задавать произвольную раскладку с любой степенью автоматизации и кастомизации (если вы, конечно, умеете программировать), будь то автозамены или автоматический пробел после препинаков (знаков препинания). Что хорошо, всё это реализуется при помощи самого стандартного протокола USB HID, так что такая программируемая клавиатура будет работать на любом компьютере (и даже телефоне) без установки драйверов.

Возможности настолько велики, что кто-то даже сделал [тетрис на клавиатуре.](https://github.com/danamlund/meckb_tetris)

**[QMK](https://github.com/qmk/qmk_firmware)** - пример библиотеки для создания прошивки клавиатуры.

**Эргономичная клавиатура** - клавиатура, которая своей формой и расположением клавиш стремится избавиться от таких фатальных недостатков обычной клавиатуры, как:
* [Смещение рядов](http://ibnteo.klava.org/keyboard/standard).
* Неиспользование большого пальца.
* Плоская форма (но это дорого и реализуется редко, например в Kinesis Advantage).

**[ErgoDox EZ](https://ergodox-ez.com/)** - пример программируемой эргономичной клавиатуры, исправляющей первые два недостатка.

## Изображение раскладки

![](https://funkyimg.com/i/2LSSg.png)

## Описание

Четыре картинки выше - различные слои. Обозначение клавиш для включения и выключения слоя: закрашенные и пустые кружочки с цифрами: 
* **Пустые кружочки** - слой включается когда эту клавишу зажимают (аналог шифта), 
* **Закрашенные кружочки** - слой включается при нажатии этой клавиши один раз (аналог капс лока).

Для каждого слоя показан цвет лампочки, которая горит на клавиатуре при его включении.

Описание слоёв:
* Первые **два слоя** - английская раскладка на основе [Programer Dvorak](https://www.kaufmann.no/roland/dvorak/).
* Следующие **два слоя** - русская раскладка на основе ЙЦУКЕН с элементами русской машинописи и Дворака для программистов.
* **Слой Other** (я его называю красным слоем) - слой для всякого, там находятся клавиши для передвижения, F-клавшии и полезные сочетания клавиш, в частности ряд с F10, F11, Ctrl+F10 - слой для дебага.
* **Слой для Браузера**, там есть соответственно кнопки для закрытия вкладки, переключения вкладки, возвращения старой итд.

Для того, чтобы моя раскладка полностью коректно работала, необходимо, чтобы:
* На компьютере стояли стандартные русская и английская раскладки.
* В операционной системе был один язык для всех окон (такое нельзя сделать для Windows 7, но можно в 10 и для linux это включено по умолчанию). 
* Переключение языка назначено на Alt+Shift.

Картинка делалась в [моей модификации](https://github.com/optozorax/my-keyboard-layout-editor) сайта KLE.

На этой клавиатуре на русской раскладке я печатаю под 700 символов в минуту, на английском под 500. Доказательство: [аккаунт на клавогонках.](http://klavogonki.ru/u/#/517589/)

## Фичи раскладки:

* Собственные русская и английская раскладка.
* Залипающий шифт - если нажать шифт, а затем отпустить, то он применится к следующей клавише, если прошло менее 1 секунды.
* Запятая с автопробелом.
* Точка с автопробелом и автозалипающим шифтом.
* При нажатии Ctrl, Alt, Win, включается английский слой, и соответственно все модификаторы нажимаются так, как будто сейчас английская раскладка, даже если на самом деле русская. (и тут русскоязычные пользователи дворака прослезились)
* Текущий слой, язык или действие залипающего шифта показывается при помощи светодиодов.

## Философия раскладки:

Всё это можно увидеть на изображении раскладки.

* **Каждому слою своя картинка.** Потому что на картинке где одновременно русская, английская раскладка и ещё сверху пара слоёв, ничего невозможно разобрать.
* **Любое нажатие модификатора - грех** (клавиша переключения слоя не модификатор, шифт не считается). А для любых частых комбинаций клавиш используются уже зарезервированные клавишы на разных слоях, где автоматически нажимается `Ctrl+C`, `Ctrl+Shift+Print Screen` итд. Благодаря этому я практически никогда не использую модификаторы Ctrl, Alt, Win, за счёт чего смог разместить на больших пальцах более частые клавиши: Backspace, Shift.
* **Стрелки на домашней позиции.** Они находятся на месте клавиш ОЛДЖ, но так как там находятся буквы, стрелки включаются красным слоем. Стрелки обязательно размещать именно на домашней позиции, чтобы при доступе к ним, рука никуда не смещалась сразу после набора текста, потому что *это удобно!* А ещё потому что согласно статистике, замеренной с помощью [klavarog/heatmap](https://github.com/klavarog/heatmap), у меня стрелки занимают 25% всех нажатий!!! Это немыслимое число, поэтому им самое место на домашней позиции.
* **Стрелки 4х1.** Это означает, что стрелки расположены не в форме перевернутой буквы Т, как на обычных клавиатурах, а последовательно, 4 клавиши подряд. Прямо как hjkl в vim, но смещено вправо. К этому несложно привыкнуть. Так же это позволяет разместить сверху клавиши мета-стрелок: Home, End, Pg Up, Pg Down; а снизу - стрелки с уже зажатым модификатором Ctrl, для перемещения по словам. Стрелки с нажатым контролом использую очень часто, а так же очень быстро переключаюсь с обычных стрелок на стрелки с контролом, что невозможно делать, не будь этих клавиш, ведь невозможно настолько быстро нажимать и отпускать контрол.
* **Отдельные клавиши для Ctrl+CVXAZ.** Является частью философии про грешность модификаторов, но это настолько важно, что я вынес это в отдельный топик. Ctrl+CVXA очень полезны при редактировании текста, скорость рефакторинга повышается, внимание меньше рассеивается. Ctrl+Z разместил симметрично Esc, потому что и смысл у них симметричный.
* **Препинаки без шифта.** На стандартных клавиатурах в 4 ряду находятся цифры, а если нажать Shift, там будут находиться препинаки. В моей раскладке наоборот, потому что препинаки встречаются чаще цифр, а нажимать шифт сложно при быстрой печати. Особенно это актуально при программировании, чуть слабее при наборе текстов. А вообще данная философия наследуется от Дворака для программистов.
* **Каждому пальцу свой цвет.** На картинке раскладки цвет каждой клавиши означает то, каким пальцем эта клавиша будет нажиматься. Все соотношения цветов и пальцев вполне очевидны, кроме бирюзового. Это цвет клавиш, которые не будут нажиматься из домашней позиции, то есть будут нажиматься любым пальцем, какой попдается под руку, обычно глядя на клавиатуру. Остальные же клавиши нажимаются строго из домашней позиции, не глядя на клавиатуру. Наличие таких клавиш по моему мнению означает, что клавиатура спроектирована недостаточно хорошо для слепого набора, что не все клавиши удобно нажимать из домашней позиции, и такие клавиши надо либо располагать отдельно от основной клавиатуры, как это делается с нумпадом в обычной клавиатуре.

## Низкоуровневые фичи и философия раскладки

Данный раздел написан для тех, кто шарит QMK. Иными словами не для широкой публики.

* **Буквенная раскладка задаётся двумя слоями.** Для того, чтобы не прибегать к софтварным методам модификации раскладки. TODO чёт лень.
* **Inverted Shift** модификатор для клавиш - это такой модификатор, при котором, если нажат шифт, для этой клавиши шифт отпускается, а затем снова возвращается. Он используется на шифтовом слое английской и русской раскладки, в частности чтобы набирать цифры при зажатом шифте.

## Костыли

Для того, чтобы после автошифта его индикатор горел ровно необходимое число секунд, нужно добавить следующий код в qmk:

В `tmk_core/common/keyboard.h`:
```diff
// ...
+void user_timer(void);
// ...
```

В `tmk_core/common/keyboard.c`:
```diff
// ...
void keyboard_task(void)
{
	// ...

+    user_timer();
}

+__attribute__((weak))
+void user_timer() {
+}
// ...
```

## Heatmap

В основном для своей клавиатуры я разрабатываю инструмент для замера статистики использования, но он может быть использован для любой клавиатуры под управлением QMK, а так же в будущем ожидается и для любых клавиатур. Туда входят: частотность клавиш, частнотность биграмм, процент использования пальцев и множество других вещей. Посмотреть можно в [klavarog/heatmap](https://github.com/klavarog/heatmap).
