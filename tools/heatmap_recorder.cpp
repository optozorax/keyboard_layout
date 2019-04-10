#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include <chrono>

#include "keyboard_listen.h"

const std::string save_file = "heatmap_file.txt"; /// Файл, куда сохраняется статистика
const int tap_count_to_save = 3000; /// Сколько должно пройти нажатий, чтобы текущая статистика сбросилась в файл на диск
const int tap_count_to_update_date = 100; /// Сколько должно пройти нажатий, чтобы обновилась дата, вдруг день уже не сегодняшний, или вдруг человек не выключает компьютер

class Heatmap
{
public:
	virtual void onTap(Tap tap) = 0;

	virtual void save(std::ostream& out) const = 0;
	virtual void load(std::istream& in) = 0;	

	typedef std::vector<int32_t> Line;
	typedef std::vector<Line> Layer;
};

/** Просто статистика числа нажатий по каждой клавише. */
class OneTapHeatmap : public Heatmap
{
public:
	OneTapHeatmap(int layersN, int rowsN, int colsN) : layers_heatmap(layersN, Layer(rowsN, Line(colsN, 0))) {}

	void onTap(Tap tap) {
		if (tap.layer >= layersCount() || tap.row >= rowsCount() || tap.col >= colsCount()) {
			resize(std::max(tap.layer+1, layersCount()), 
			       std::max(tap.row+1, rowsCount()),
			       std::max(tap.col+1, colsCount()));
		}
		
		layers_heatmap[tap.layer][tap.row][tap.col]++;
	}

	void save(std::ostream& out) const {
		out << layersCount() << " " << rowsCount() << " " << colsCount() << std::endl;
		for (int i = 0; i < layersCount(); ++i) {
			for (int j = 0; j < rowsCount(); ++j) {
				for (int k = 0; k < colsCount(); ++k) {
					out << layers_heatmap[i][j][k];
					if (k+1 != colsCount())
						out << " ";
				}
				out << std::endl;
			}
			out << std::endl;
		}
	}
	void load(std::istream& in) {
		int layersN, rowsN, colsN;
		in >> layersN >> rowsN >> colsN;
		layers_heatmap.clear();
		layers_heatmap.resize(layersN, Layer(rowsN, Line(colsN, 0)));
		for (int i = 0; i < layersN; ++i) {
			for (int j = 0; j < rowsN; ++j) {
				for (int k = 0; k < colsN; ++k) {
					in >> layers_heatmap[i][j][k];
				}
			}
		}
	}

	void resize(int layersN, int rowsN, int colsN) {
		std::vector<Layer> result(layersN, Layer(rowsN, Line(colsN, 0)));
		for (int i = 0; i < layers_heatmap.size(); ++i) {
			for (int j = 0; j < layers_heatmap[i].size(); ++j) {
				for (int k = 0; k < layers_heatmap[i][j].size(); ++k) {
					result[i][j][k] = layers_heatmap[i][j][k];
				}
			}
		}

		swap(result, layers_heatmap);
	}

	int layersCount(void) const {
		return layers_heatmap.size();
	}
	int rowsCount(void) const {
		if (layersCount() > 0)
			return layers_heatmap[0].size();
		else
			return 0;
	}
	int colsCount(void) const {
		if (rowsCount() > 0)
			return layers_heatmap[0][0].size();
		else
			return 0;
	}
	int getTapCount(Tap tap) const {
		if (tap.layer > layersCount() || tap.row > rowsCount() || tap.col > colsCount()) {
			std::cerr << "Error when trying access to tap: " << tap << std::endl;
			throw std::exception();
		} else {
			return layers_heatmap[tap.layer][tap.row][tap.col];
		}
	}
private:
	int counter;
	std::vector<Layer> layers_heatmap;
};

/** Статистика двух нажатий, когда одно идет за другим. Как часто та или иная комбинация встречается. */
class TwoTapHeatmap : public Heatmap
{
public:
	TwoTapHeatmap(int layersN, int rowsN, int colsN) : two_taps_heatmap(layersN, TwoTapLayer(rowsN, TwoTapLine(colsN, OneTapHeatmap(1, rowsN, colsN)))) {}

	void onTap(Tap tap) {
		if (tap.layer >= layersCount() || tap.row >= rowsCount() || tap.col >= colsCount()) {
			resize(std::max(tap.layer+1, layersCount()), 
			       std::max(tap.row+1, rowsCount()),
			       std::max(tap.col+1, colsCount()));
		}
		
		if (lastTap.layer != -1) {
			auto tap_copy = tap;
			tap_copy.layer = 0;
			two_taps_heatmap[lastTap.layer][lastTap.row][lastTap.col].onTap(tap_copy);
		}
		lastTap = tap;
	}

	void save(std::ostream& out) const {
		out << layersCount() << " " << rowsCount() << " " << colsCount() << std::endl;
		for (int i = 0; i < layersCount(); ++i) {
			for (int j = 0; j < rowsCount(); ++j) {
				for (int k = 0; k < colsCount(); ++k) {
					two_taps_heatmap[i][j][k].save(out);
					out << std::endl;
				}
				out << std::endl;
			}
			out << std::endl;
		}
	}
	void load(std::istream& in) {
		int layersN, rowsN, colsN;
		in >> layersN >> rowsN >> colsN;
		two_taps_heatmap.clear();
		two_taps_heatmap.resize(layersN, TwoTapLayer(rowsN, TwoTapLine(colsN, OneTapHeatmap(1, rowsN, colsN))));
		for (int i = 0; i < layersN; ++i) {
			for (int j = 0; j < rowsN; ++j) {
				for (int k = 0; k < colsN; ++k) {
					two_taps_heatmap[i][j][k].load(in);
				}
			}
		}
	}

	void resize(int layersN, int rowsN, int colsN) {
		for (auto& i : two_taps_heatmap) {
			for (auto& j : i) {
				for (auto& k : j) {
					k.resize(1, rowsN, colsN);
				}
			}
		}

		std::vector<TwoTapLayer> result(layersN, TwoTapLayer(rowsN, TwoTapLine(colsN, OneTapHeatmap(1, rowsN, colsN))));
		for (int i = 0; i < two_taps_heatmap.size(); ++i) {
			for (int j = 0; j < two_taps_heatmap[i].size(); ++j) {
				for (int k = 0; k < two_taps_heatmap[i][j].size(); ++k) {
					result[i][j][k] = two_taps_heatmap[i][j][k];
				}
			}
		}

		swap(result, two_taps_heatmap);
	}

	int layersCount(void) const {
		return two_taps_heatmap.size();
	}
	int rowsCount(void) const {
		if (layersCount() > 0)
			return two_taps_heatmap[0].size();
		else
			return 0;
	}
	int colsCount(void) const {
		if (rowsCount() > 0)
			return two_taps_heatmap[0][0].size();
		else
			return 0;
	}
	int getTapCount(Tap first, Tap second) const {
		if (first.layer > layersCount() || first.row > rowsCount() || first.col > colsCount()) {
			std::cerr << "Error when trying access to taps: " << first << " " << second << std::endl;
			throw std::exception();
		} else {
			second.layer = 0;
			return two_taps_heatmap[first.layer][first.row][first.col].getTapCount(second);
		}
	} // У второго слой считается таким же как и у первого

	typedef std::vector<OneTapHeatmap> TwoTapLine;
	typedef std::vector<TwoTapLine> TwoTapLayer;
private:
	std::vector<TwoTapLayer> two_taps_heatmap;
	Tap lastTap;
};

/** Количество нажатий за определенный день(просто всех клавиш). */
class DayHeatmap : public Heatmap
{
public:
	DayHeatmap() : counter(0) {
		updateToday();
	}
	void onTap(Tap tap) {
		counter++;

		statistics[today]++;

		if (counter > tap_count_to_update_date) {
			counter = 0;
			updateToday();
		}
	}

	void save(std::ostream& out) const {
		out << statistics.size() << std::endl;
		for (auto& i : statistics)
			out << i.first << " " << i.second << std::endl;
	}
	void load(std::istream& in) {
		int size;
		in >> size;
		for (int i = 0; i < size; ++i) {
			int first, second;
			in >> first >> second;
			statistics[first] = second;
		}
	}
private:
	void updateToday() {
		using namespace std::chrono;
		auto now = system_clock::now();
		today = duration_cast<hours>(now.time_since_epoch()).count();
		today /= 24;
	}

	int counter;
	int today;
	std::map<int, int32_t> statistics;
};

//=============================================================================
//=============================================================================
//=============================================================================

OneTapHeatmap onetap(1, 1, 1);
TwoTapHeatmap twotap(1, 1, 1);
DayHeatmap    daytap;

//-----------------------------------------------------------------------------
void load_all(const std::string& file) {
	using namespace std;

	ifstream fin(file);
	if (fin) {
		onetap.load(fin);
		twotap.load(fin);
		daytap.load(fin);
		fin.close();
	}
}

//-----------------------------------------------------------------------------
void save_all(const std::string& file) {
	using namespace std;

	ofstream fout(file);
	onetap.save(fout);
	twotap.save(fout);
	daytap.save(fout);
	fout.close();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
bool onStart(int argc, char** argv) {
	load_all(save_file);
	return true;
}

//-----------------------------------------------------------------------------
void onTap(Tap tap, long time_duration, long time_offset) {
	using namespace std;
	static int counter = 0;
	counter++;
	onetap.onTap(tap);
	twotap.onTap(tap);
	daytap.onTap(tap);
	if (counter > tap_count_to_save) {
		counter = 0;
		save_all(save_file);
	}
}

//-----------------------------------------------------------------------------
void onExit(void) {
	using namespace std;

	cerr << endl;
	cerr << "Saving information..." << endl;
	save_all(save_file);
	cerr << "Terminating..." << endl;
}