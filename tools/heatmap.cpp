#include <algorithm>
#include <chrono>
#include <fstream>
#include "heatmap.h"

//-----------------------------------------------------------------------------
OneTapHeatmap::OneTapHeatmap(int layersN, int rowsN, int colsN) : layers_heatmap(layersN, Layer(rowsN, Line(colsN, 0))) {}

//-----------------------------------------------------------------------------
void OneTapHeatmap::onTap(Tap tap) {
	if (tap.layer >= layersCount() || tap.row >= rowsCount() || tap.col >= colsCount()) {
		resize(std::max(tap.layer+1, layersCount()), 
		       std::max(tap.row+1, rowsCount()),
		       std::max(tap.col+1, colsCount()));
	}
	
	layers_heatmap[tap.layer][tap.row][tap.col]++;
}

//-----------------------------------------------------------------------------
void OneTapHeatmap::save(std::ostream& out) const {
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

//-----------------------------------------------------------------------------
void OneTapHeatmap::load(std::istream& in) {
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

//-----------------------------------------------------------------------------
void OneTapHeatmap::resize(int layersN, int rowsN, int colsN) {
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

//-----------------------------------------------------------------------------
int OneTapHeatmap::layersCount(void) const {
	return layers_heatmap.size();
}

//-----------------------------------------------------------------------------
int OneTapHeatmap::rowsCount(void) const {
	if (layersCount() > 0)
		return layers_heatmap[0].size();
	else
		return 0;
}

//-----------------------------------------------------------------------------
int OneTapHeatmap::colsCount(void) const {
	if (rowsCount() > 0)
		return layers_heatmap[0][0].size();
	else
		return 0;
}

//-----------------------------------------------------------------------------
int OneTapHeatmap::getTapCount(Tap tap) const {
	if (tap.layer > layersCount() || tap.row > rowsCount() || tap.col > colsCount()) {
		std::cerr << "Error when trying access to tap: " << tap << std::endl;
		throw std::exception();
	} else {
		return layers_heatmap[tap.layer][tap.row][tap.col];
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
TwoTapHeatmap::TwoTapHeatmap(int layersN, int rowsN, int colsN) : two_taps_heatmap(layersN, TwoTapLayer(rowsN, TwoTapLine(colsN, OneTapHeatmap(1, rowsN, colsN)))) {}

//-----------------------------------------------------------------------------
void TwoTapHeatmap::onTap(Tap tap) {
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

//-----------------------------------------------------------------------------
void TwoTapHeatmap::save(std::ostream& out) const {
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

//-----------------------------------------------------------------------------
void TwoTapHeatmap::load(std::istream& in) {
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

//-----------------------------------------------------------------------------
void TwoTapHeatmap::resize(int layersN, int rowsN, int colsN) {
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

//-----------------------------------------------------------------------------
int TwoTapHeatmap::layersCount(void) const {
	return two_taps_heatmap.size();
}

//-----------------------------------------------------------------------------
int TwoTapHeatmap::rowsCount(void) const {
	if (layersCount() > 0)
		return two_taps_heatmap[0].size();
	else
		return 0;
}

//-----------------------------------------------------------------------------
int TwoTapHeatmap::colsCount(void) const {
	if (rowsCount() > 0)
		return two_taps_heatmap[0][0].size();
	else
		return 0;
}

//-----------------------------------------------------------------------------
int TwoTapHeatmap::getTapCount(Tap first, Tap second) const {
	if (first.layer > layersCount() || first.row > rowsCount() || first.col > colsCount()) {
		std::cerr << "Error when trying access to taps: " << first << " " << second << std::endl;
		throw std::exception();
	} else {
		second.layer = 0;
		return two_taps_heatmap[first.layer][first.row][first.col].getTapCount(second);
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
DayHeatmap::DayHeatmap() : counter(0) {
	updateToday();
}

//-----------------------------------------------------------------------------
void DayHeatmap::onTap(Tap tap) {
	counter++;

	statistics[today]++;

	if (counter > tap_count_to_update_date) {
		counter = 0;
		updateToday();
	}
}

//-----------------------------------------------------------------------------
void DayHeatmap::save(std::ostream& out) const {
	out << statistics.size() << std::endl;
	for (auto& i : statistics)
		out << i.first << " " << i.second << std::endl;
}

//-----------------------------------------------------------------------------
void DayHeatmap::load(std::istream& in) {
	int size;
	in >> size;
	for (int i = 0; i < size; ++i) {
		int first, second;
		in >> first >> second;
		statistics[first] = second;
	}
}

//-----------------------------------------------------------------------------
void DayHeatmap::updateToday() {
	using namespace std::chrono;
	auto now = system_clock::now();
	today = duration_cast<hours>(now.time_since_epoch()).count();
	today /= 24;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
AllStatistics::AllStatistics(const std::string& file) : onetap(1, 1, 1), twotap(1, 1, 1), daytap(), file(file) {}

//-----------------------------------------------------------------------------
void AllStatistics::load(void) {
	std::ifstream fin(file);
	if (fin) {
		onetap.load(fin);
		twotap.load(fin);
		daytap.load(fin);
		fin.close();
	}
}

//-----------------------------------------------------------------------------
void AllStatistics::save(void) const {
	std::ofstream fout(file);
	onetap.save(fout);
	twotap.save(fout);
	daytap.save(fout);
	fout.close();
}