#define WINDOWS

#include <iostream>
#include <chrono>
#include <string>
#include <cctype>
#include <queue>
#include <iomanip>

#include <stdio.h>
#include <string.h>

#ifndef WINDOWS
#include <unistd.h>
#include <signal.h> 
#else
#include <csignal>
#endif

using namespace std;

#define MATRIX_ROWS 14
#define MATRIX_COLS 6

#define LAYOUT_ergodox(                                         \
                                                                \
    k00,k01,k02,k03,k04,k05,k06,                                \
    k10,k11,k12,k13,k14,k15,k16,                                \
    k20,k21,k22,k23,k24,k25,                                    \
    k30,k31,k32,k33,k34,k35,k36,                                \
    k40,k41,k42,k43,k44,                                        \
                            k55,k56,                            \
                                k54,                            \
                        k53,k52,k51,                            \
                                                                \
        k07,k08,k09,k0A,k0B,k0C,k0D,                            \
        k17,k18,k19,k1A,k1B,k1C,k1D,                            \
            k28,k29,k2A,k2B,k2C,k2D,                            \
        k37,k38,k39,k3A,k3B,k3C,k3D,                            \
                k49,k4A,k4B,k4C,k4D,                            \
    k57,k58,                                                    \
    k59,                                                        \
    k5C,k5B,k5A )                                               \
                                                                \
   /* matrix positions */                                       \
   {                                                            \
    { #k00, #k10, #k20, #k30, #k40, "KC_NO" },   \
    { #k01, #k11, #k21, #k31, #k41, #k51 },   \
    { #k02, #k12, #k22, #k32, #k42, #k52 },   \
    { #k03, #k13, #k23, #k33, #k43, #k53 },   \
    { #k04, #k14, #k24, #k34, #k44, #k54 },   \
    { #k05, #k15, #k25, #k35, "KC_NO", #k55 },   \
    { #k06, #k16, "KC_NO", #k36, "KC_NO", #k56 },   \
                                                                 \
    { #k07, #k17, "KC_NO", #k37 "KC_NO", #k57 },   \
    { #k08, #k18, #k28, #k38 "KC_NO", #k58 },   \
    { #k09, #k19, #k29, #k39, #k49, #k59 },   \
    { #k0A, #k1A, #k2A, #k3A, #k4A, #k5A },   \
    { #k0B, #k1B, #k2B, #k3B, #k4B, #k5B },   \
    { #k0C, #k1C, #k2C, #k3C, #k4C, #k5C },   \
    { #k0D, #k1D, #k2D, #k3D, #k4D, "KC_NO" }    \
   }

const char* keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  //---------------------------------------------------------------------------
  LAYOUT_ergodox(
    Esc,     &,  [,  ],  +,  =, Ctrl+X,
    Tab,     ;,  {,  },  p,     y,   Ctrl+C,
    MO(4),      a,     o,     e,     u,     i,
    MO(5),      "'",  q,     j,     k,     x,   Ctrl+V,
    TG(4),      TG(5),    TG(6),    Ctrl+/,  /,

        Ctrl+A,     Delete,   Ctrl,
        Shift,    Backspace,  Enter,


    Shift+PrintScreen,    *,  !,  ")",  "(",  ?,  Ctrl+Z,
    Ctrl+D,       f,     g,     c,     r,     l,     "\\",
                d,     h,     t,     n,     s,     "-",
    Ctrl+S,     b,     m,     w,     v,     z,     "#",
                          ",_",  Klavogonki Next,  Shift+Alt,  Ctrl+Alt,  Ctrl+Alt+Shift,

        Ctrl+Shift,  Alt,   Win,
        Lang,  ".",   Space),

  //---------------------------------------------------------------------------
  LAYOUT_ergodox(
    _______,    7,     5,     3,     1,     9,     "`",
    _______,    :,  <,    >,    _______,  _______,  $,
    _______,    _______,  _______,  _______,  _______,  _______,
    _______,    "\"",   _______,  _______,  _______,  _______,  ~,
    _______,    _______,  _______,  _______,  _______,

        _______,  _______,  _______,
        _______,  _______,  _______,


    _______,    0,     2,     4,     6,     8,     %,
    _______,    _______,  _______,  _______,  _______,  _______,  |,
                _______,  _______,  _______,  _______,  _______,  @,
    _______,    _______,  _______,  _______,  _______,  _______,  ^,
                          ",",  _______,  _______,  _______,  _______,

        _______,  _______,  _______,
        _______,  _,  _______),

  //---------------------------------------------------------------------------
  LAYOUT_ergodox(
    _______,    +,  "-",  ;,   "\"",    :,  CT_X,
    _______,    й,     ц,     у,     е,     ф,     Ctrl+C,
    _______,    к,     м,     в,     а,     п,
    _______,    я,     ч,     с,     и,     ы,     Ctrl+V,
    _______,    _______,  _______,  _______,  _,

        _______,   _______,   _______,
        Shift,     _______,   _______,


    _______,    *,  !,  ")",  "(",  ?,  Ctrl+Z,
    _______,    щ,     г,     т,     н,     з,     ъ,
                р,     о,     л,     д,     ж,  э,
    _______,    ш,     ь,     б,     ю,     х,  ё,
                          ", ",  Klavogonki Next,  _______,  _______,  _______,

        _______,   _______,   _______,
        _______,   ". AutoShift",   _______),

  //---------------------------------------------------------------------------
  LAYOUT_ergodox(
    _______,    IS_7,     IS_5,     IS_3,     IS_1,     IS_9,     _______,
    _______,    _______,  _______,  _______,  _______,  _______,  KC_HASH,
    _______,    _______,  _______,  _______,  _______,  _______,
    _______,    _______,  _______,  _______,  _______,  _______,  IS_EQL,
    _______,    _______,  _______,  _______,  _______,

        _______,  _______,  _______,
        _______,  _______,  _______,


    _______,    IS_0,     IS_2,     IS_4,     IS_6,     IS_8,     KC_PERC,
    _______,    _______,  _______,  _______,  _______,  _______,  _______,
                _______,  _______,  _______,  _______,  _______,  _______,
    _______,    _______,  _______,  _______,  _______,  _______,  _______,
                          KC_QUES,  _______,  _______,  _______,  _______,

        _______,  _______,  _______,
        _______,  IS_SLSH,  _______),

  //---------------------------------------------------------------------------
  LAYOUT_ergodox(
    TG(4),      KC_F7,    KC_F5,    KC_F3,    KC_F1,    KC_F9,    CT_X,
    AU_MUTE,    SH_F11,   CT_F10,   KC_F10,   KC_F11,   WN_D,     CT_C,
    _______,    AU_VOLU,  AU_NEXT,  CS_TAB,   CT_TAB,   AL_TAB,
    _______,    AU_VOLD,  AU_PREV,  CT_1,     CT_2,     AL_TTAB,  CT_V,
    DN_PLY2,    DN_STOP,  DN_STR2,  WN_E,     AU_PLAY,

        _______,  _______,  _______,
        _______,  _______,  _______,

    KC_NLCK,    KC_F10,   KC_F2,    KC_F4,    KC_F6,    KC_F8,    KC_F11,
    KC_INSERT,  CT_DEL,   KC_HOME,  KC_END,   KC_PGUP,  KC_PGDN,  KC_F12,
                Ctrl+Backspace,  KC_LEFT,  KC_UP,    KC_DOWN,  KC_RGHT,  KC_PSCR,
    KC_PAUSE,   CT_ENT,   CT_LEFT,  CT_UP,    CT_DOWN,  CT_RGHT,  AL_PSCR,
                          _______,   _______, DN_STR1,  DN_STOP,  DN_PLY1,

        _______,  _______,  _______,
        _______,  _______,  _______),

  //---------------------------------------------------------------------------
  LAYOUT_ergodox(
    TG(5),      CT_1,     CT_2,     CT_3,     CT_4,     CT_5,     CT_X,
    KC_F5,      KC_F6,    CS_T,     CT_T,     CT_W,     CT_J,     CT_C,
    KC_BTN2,    KC_BTN1,  KC_BTN3,  CS_TAB,   CT_TAB,   CT_D,
    _______,    CT_S,     KC_ENT,   KC_UP,    KC_DOWN,  CT_F,     CT_V,
    _______,    _______,  _______,  _______,  _______,  

        _______,  _______,  _______,
        _______,  _______,  _______,

    _______,    _______,  _______,  _______,  _______,  _______,  _______,
    _______,    _______,  _______,  _______,  _______,  _______,  _______,
                _______,  MS_LEFT,  MS_UP,    MS_DOWN,  MS_RGHT,  _______,
    _______,    _______,  MSWH_LF,  MSWH_UP,  MSWH_DW,  MSWH_RG,  _______,
                _______,  _______,  _______,  _______,  _______,

        _______,  _______,  _______,
        _______,  _______,  _______),

  //---------------------------------------------------------------------------
  LAYOUT_ergodox(
    KC_ESC,     KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,
    KC_TAB,     KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_7,
    KC_LSFT,    KC_A,     KC_S,     KC_D,     KC_F,     KC_G,
    KC_LCTL,    KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_8,
    XXXXXXX,    XXXXXXX,  TG(6),    KC_PERC,  RU_CMSP,

        XXXXXXX,   XXXXXXX,   XXXXXXX,
        KC_SPC,    KC_LALT,   KC_ENT,


    XXXXXXX,    KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     XXXXXXX,
    XXXXXXX,    KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_RBRC,
                KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,
    XXXXXXX,    KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_UNDS,
                          KC_LEFT,  KC_UP,    KC_DOWN,  KC_RGHT,  XXXXXXX,

        XXXXXXX,   XXXXXXX,   XXXXXXX,
        XXXXXXX,   XXXXXXX,   XXXXXXX),
};

#define BUF_SIZE 500

bool isTerminate = false;
FILE* proc1;

void handle_sig(int a) {
	cout << endl;
	cout << "I receive a signal: " << a << endl;
	cout << "Terminating..." << endl;
	isTerminate = true;
	_pclose(proc1);
	exit(0);
}

int getStrValue(const string& str, const string& name) {
	auto pos = str.find(name + "=");
	pos += name.size();
	pos++;
	string number;
	while (pos < str.size() && isdigit(str[pos])) {
		number += str[pos];
		pos++;
	}
	return stoi(number);
}

string getName(int layer, int row, int col) {
	string result;
	start:
	result = keymaps[layer][row][col];
	if (result.back() == result.front() && result.front() == '"') {
		result.pop_back();
		result.erase(result.begin());
	}
	if (result == "_______") {
		if (layer != 0) {
			layer--;
			goto start;
		}
	}
	if (result == "\\\\")
		result = "\\";
	if (result == "\\\"")
		result = "\"";
	return result;
}

int main() {
	signal(SIGINT, handle_sig);
	#ifndef WINDOWS
	signal(SIGKILL, handle_sig);
	signal(SIGSTOP, handle_sig);
	signal(SIGTSTP, handle_sig);
	#endif

	proc1 = _popen("hid_listen.exe", "r");
	char buf[BUF_SIZE] = {};
	auto start_time = chrono::system_clock::now();
	auto pause_time = chrono::system_clock::now();
	int pause_taps = 0;
	struct Key
	{
		std::chrono::time_point<std::chrono::system_clock> start_press;
		int col, row, layer;

		bool isFinished;
		long offset, duration;
	};
	deque<Key> keys;
	while (!isTerminate) {
		int n = fgets(buf, BUF_SIZE, proc1) != NULL;
		if (n) {
			string str(buf);
			str.pop_back();
			if (str.size() > 3 && str.substr(0, 3) == "KL:") {
				auto current_time = chrono::system_clock::now();

				int col = getStrValue(str, "col");
				int row = getStrValue(str, "row");
				int pressed = getStrValue(str, "pressed");
				int layer = getStrValue(str, "layer");

				bool isHave = false;
				for (auto& i : keys) {
					if (!i.isFinished && i.col == col && i.row == row && !pressed) {
						isHave = true;
						i.isFinished = true;
						i.duration = std::chrono::duration_cast<std::chrono::milliseconds>(current_time-i.start_press).count();
					}
				}

				if (!isHave && pressed) {
					keys.push_back({current_time, col, row, layer, false, long(std::chrono::duration_cast<std::chrono::milliseconds>(current_time-start_time).count()), 0});
					start_time = current_time;
				}

				while (keys.size() != 0 && keys.front().isFinished) {
					auto key = keys.front();
					pause_taps++;
					if (key.offset > 2000) {
						long time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time-pause_time).count()-key.offset;
						double seconds = time/1000.0;
						double minutes = seconds/60.0;
						cout << "tpm: " << pause_taps/minutes << endl;
						cout << "-----------------------------------------" << endl;
						pause_time = current_time;
						pause_taps = 0;
					}
					cout << "col=" << setw(2) << key.col << ", row=" << setw(2) << key.row << ", layer=" << layer << ", offset=" << setw(5) << key.offset << "ms, duration=" << setw(5) << key.duration << "ms, key=\"" << getName(key.layer, key.row, key.col) << "\"" << endl;
					keys.pop_front();
				}
			}
		}
	}
}