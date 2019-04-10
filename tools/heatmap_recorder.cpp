#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <algorithm>

#include "keyboard_listen.h"

typedef std::vector<int32_t> Line;
typedef std::vector<Line> Matrix;
typedef std::vector<Matrix> Layers;

const std::string save_file = "heatmap_file.txt";

int lineN = 1, matrixN = 1, layersN = 1; 
Layers layers;

//-----------------------------------------------------------------------------
void write(std::ostream& out, const Layers& layers, const int& lineN, const int& matrixN, const int& layersN) {
	out << lineN << " " << matrixN << " " << layersN << std::endl;
	for (int i = 0; i < layersN; ++i) {
		for (int j = 0; j < matrixN; ++j) {
			for (int k = 0; k < lineN; ++k) {
				out << layers[i][j][k];
				if (k+1 != lineN)
					out << " ";
			}
			out << std::endl;
		}
		out << std::endl;
	}
}

//-----------------------------------------------------------------------------
void read(std::istream& in, Layers& layers, int& lineN, int& matrixN, int& layersN) {
	in >> lineN >> matrixN >> layersN;
	layers.resize(layersN, Matrix(matrixN, Line(lineN, 0)));
	for (int i = 0; i < layersN; ++i) {
		for (int j = 0; j < matrixN; ++j) {
			for (int k = 0; k < lineN; ++k) {
				in >> layers[i][j][k];
			}
		}
	}
}

//-----------------------------------------------------------------------------
void resize(int col, int row, int layer) {
	using namespace std;

	lineN = max(col+1, lineN);
	matrixN = max(row+1, matrixN);
	layersN = max(layer+1, layersN);

	Layers result(layersN, Matrix(matrixN, Line(lineN, 0)));
	for (int i = 0; i < layers.size(); ++i) {
		for (int j = 0; j < layers[i].size(); ++j) {
			for (int k = 0; k < layers[i][j].size(); ++k) {
				result[i][j][k] = layers[i][j][k];
			}
		}
	}

	swap(result, layers);
}

//-----------------------------------------------------------------------------
void load(const std::string& file) {
	using namespace std;

	ifstream fin(file);
	if (fin) {
		read(fin, layers, lineN, matrixN, layersN);
		fin.close();
	} else {
		layers.resize(layersN, Matrix(matrixN, Line(lineN, 0)));
	}
}

//-----------------------------------------------------------------------------
void save(const std::string& file) {
	using namespace std;

	ofstream fout(file);
	write(fout, layers, lineN, matrixN, layersN);
	fout.close();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
bool onStart(int argc, char** argv) {
	load(save_file);
	return true;
}

//-----------------------------------------------------------------------------
void onTap(int col, int row, int layer, long time_duration, long time_offset) {
	using namespace std;

	static int32_t count = 0;
	if (col >= lineN || row >= matrixN || layer >= layersN)
		resize(col, row, layer);
	layers[layer][row][col]++;
	count++;
	if (count >= 5000) {
		save(save_file);
		count = 0;
	}
}

//-----------------------------------------------------------------------------
void onExit(void) {
	using namespace std;

	save(save_file);
	cerr << endl;
	cerr << "Saving information..." << endl;
	cerr << "Terminating..." << endl;
}