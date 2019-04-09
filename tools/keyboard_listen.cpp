#ifdef _WIN32
#define __WINDOWS
#else
	#ifdef linux
		#undef __WINDOWS
	#else
		#error "Unknown system!"
	#endif
#endif

#include <chrono>
#include <iostream>
#include <deque>
#include <cctype>
#include <cstdio>

#ifdef __WINDOWS
#include <csignal>
#else
#include <unistd.h>
#include <signal.h> 
#endif

#include "keyboard_listen.h"

#define BUF_SIZE 500

namespace {

typedef std::chrono::time_point<std::chrono::system_clock> Time;

struct Key
{
	Time start_press;
	int col, row, layer;

	bool isFinished;
	long offset, duration;
};

//-----------------------------------------------------------------------------
long calcDuration(Time a, Time b) {
	return std::abs(std::chrono::duration_cast<std::chrono::milliseconds>(a - b).count());
}

//-----------------------------------------------------------------------------
FILE* listener;
bool isTerminate = false;
void handle_sig(int) {
	onExit();
	#ifdef __WINDOWS
	_pclose(listener);
	#else 
	pclose(listener);
	#endif
	
	isTerminate = true;
	exit(0);
}

};

//-----------------------------------------------------------------------------
int parseStrValue(const std::string& str, const std::string& name) {
	using namespace std;

	auto pos = str.find(name + "=");
	if (pos == string::npos) return -1;
	pos += name.size();
	pos++;
	string number;
	while (pos < str.size() && isdigit(str[pos])) {
		number += str[pos];
		pos++;
	}
	return stoi(number);
}

//-----------------------------------------------------------------------------
std::deque<Key> keys;
void processKey(int col, int row, int layer, int pressed) {
	using namespace std;

	static auto last_press_time = chrono::system_clock::now(); // Время последнего нажатия, чтобы считать offset
	auto current_time = chrono::system_clock::now();

	if (!pressed) {
		// Если клавишу отжимают, то ищем такую клавишу в очереди уже нажатых, и если нашли, говорит, что она отпущена
		for (auto& i : keys) {
			if (!i.isFinished && i.col == col && i.row == row) {
				i.isFinished = true;
				i.duration = calcDuration(current_time, i.start_press);
				break;
			}
		}
	} else {
		// Если клавишу нажимают, то помещаем её в очередь нажимаемых
		keys.push_back({
			current_time, 
			col, row, layer, 
			false,
			calcDuration(last_press_time, current_time), 0
		});
		last_press_time = current_time;
	}

	// Затем убираем из очереди первые клавиши, которые уже получили событие отжатия
	while ((keys.size() > 0) && ((keys.front().isFinished) ||
	       (calcDuration(keys.front().start_press, current_time) > max_hold_time))) {
		auto key = keys.front();
		if (!key.isFinished)
			key.duration = calcDuration(keys.front().start_press, current_time);

		// Сообщаем, что клавиша была отжата из-за ошибки
		if (key.duration > max_hold_time) {
			cerr << "Unpress key "
			     << "col=" << key.col
			     << ", row=" << key.row
			     << ", layer=" << key.layer
			     << ", because out of time." << endl;
		}
		onTap(key.col, key.row, key.layer, key.duration, key.offset);
		keys.pop_front();
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

int main(int argc, char** argv) {
	using namespace std;

	onStart(argc, argv);

	signal(SIGINT, handle_sig);
	#ifdef _WINDOWS
	listener = _popen("hid_listen.exe", "r");
	if (listener == NULL)
		cout << "Error when running `hid_listen.exe`." << endl;
	#else 
	listener = popen("sudo ./hid_listen", "r");
	if (listener == NULL)
		cout << "Error when running `sudo ./hid_listen.exe`." << endl;
	#endif

	char buf[BUF_SIZE] = {};
	while (!isTerminate) {
		// Считываем строку из вывода клавиатуры
		if (fgets(buf, BUF_SIZE, listener) != NULL) {
			string str(buf);
			// Если это строка, в которой содержится необходимая нам информация
			if (str.size() > 3 && str.substr(0, 3) == "KL:") {
				// Получаем параметры текущего нажатия
				int col = parseStrValue(str, "col");
				int row = parseStrValue(str, "row");
				int pressed = parseStrValue(str, "pressed");
				int layer = parseStrValue(str, "layer");

				// Проверяем их на корректность
				#define check(a) if (a == -1) {\
					cerr << "Error when parsing `" << #a << "` in string `" << str << "`." << endl;\
					continue;\
				}

				check(col);
				check(row);
				check(pressed);
				check(layer);

				processKey(col, row, layer, pressed);
			}
		}
	}
}