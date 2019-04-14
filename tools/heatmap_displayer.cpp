#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#include "heatmap.h"

namespace po = boost::program_options;

AllStatistics all("heatmap_file.txt");
Fingers fingers;
Names names;

typedef std::vector<std::string> TableLine;
typedef std::vector<TableLine> Table;

//-----------------------------------------------------------------------------
void tablePrint(const Table& table) {
	std::vector<int> sizes(table[0].size(), 0);
	for (auto& i : table) {
		if (i.size() != sizes.size())
			throw std::exception();
		for (int j = 0; j < i.size(); ++j)
			sizes[j] = std::max<size_t>(sizes[j], i[j].size());
	}

	for (int i = 0; i < table.size(); ++i) {
		for (int j = 0; j < table[i].size(); ++j)
			std::cout << std::setw(sizes[j]) << table[i][j];
		std::cout << std::endl;
	}
}

//-----------------------------------------------------------------------------
std::string getPercent(double a, double all) {
	std::stringstream sout;
	sout << std::setprecision(2) << std::fixed << 100.0*a/all;
	return sout.str();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

int sumLayerTaps(int layer);
int sumAllTaps(void);
int sumFingerTapsLayer(int finger, int layer);
int sumFingerTaps(int finger);
int sumRowTapsLayer(int row, int layer);
int sumRowTaps(int row);

//-----------------------------------------------------------------------------
int sumLayerTaps(int layer) {
	if (layer == -1)
		return sumAllTaps();

	int32_t sum = 0;
	for (int j = 0; j < all.onetap.rowsCount(); ++j) {
		for (int k = 0; k < all.onetap.colsCount(); ++k) {
			sum += all.onetap.getTapCount({k, j, layer});
		}
	}
	return sum;	
}

//-----------------------------------------------------------------------------
int sumAllTaps(void) {
	int32_t sum = 0;
	for (int i = 0; i < all.onetap.layersCount(); ++i)
		sum += sumLayerTaps(i);
	return sum;
}

//-----------------------------------------------------------------------------
int sumFingerTapsLayer(int finger, int layer) {
	if (layer == -1)
		return sumFingerTaps(finger);

	int32_t sum = 0;
	for (int j = 0; j < all.onetap.rowsCount(); ++j) {
		for (int k = 0; k < all.onetap.colsCount(); ++k) {
			Tap tap{k, j, layer};
			if (finger == 10) {
				if (fingers.getFinger(tap) == 0 && fingers.getRow(tap) == 0)
					sum += all.onetap.getTapCount(tap);
			} else {
				if (fingers.getFinger(tap) == finger && fingers.getRow(tap) != 0)
					sum += all.onetap.getTapCount(tap);
			}
		}
	}
	return sum;
}

//-----------------------------------------------------------------------------
int sumFingerTaps(int finger) {
	int32_t sum = 0;
	for (int i = 0; i < all.onetap.layersCount(); ++i)
		sum += sumFingerTapsLayer(finger, i);
	return sum;
}

//-----------------------------------------------------------------------------
int sumRowTapsLayer(int row, int layer) {
	if (layer == -1) 
		return sumRowTaps(row);

	int32_t sum = 0;
	for (int j = 0; j < all.onetap.rowsCount(); ++j) {
		for (int k = 0; k < all.onetap.colsCount(); ++k) {
			Tap tap{k, j, layer};
			if (row == 0) {
				if (fingers.getRow(tap) == 0 && fingers.getFinger(tap) == 0)
					sum += all.onetap.getTapCount(tap);
			} else {
				if (fingers.getRow(tap) == row)
					sum += all.onetap.getTapCount(tap);
			}
		}
	}
	return sum;
}

//-----------------------------------------------------------------------------
int sumRowTaps(int row) {
	int32_t sum = 0;
	for (int i = 0; i < all.onetap.layersCount(); ++i)
		sum += sumRowTapsLayer(row, i);
	return sum;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void sum_all(const po::variables_map& vm) {
	if (!vm.count("separate-by-layers")) {
		std::cout << "Count of all taps: " << sumAllTaps() << std::endl;
	} else {
		auto allSum = sumAllTaps();
		Table table;
		for (int i = 0; i < all.onetap.layersCount(); ++i) {
			auto layerSum = sumLayerTaps(i);

			TableLine line;
			line.push_back("Layer: ");
			line.push_back(std::to_string(i));
			line.push_back(", taps: ");
			line.push_back(std::to_string(layerSum));
			line.push_back(", percent: ");
			line.push_back(getPercent(layerSum, allSum));
			line.push_back("%");
			
			table.emplace_back(line);
		}
		tablePrint(table);
	}
}

//-----------------------------------------------------------------------------
void sum_fingers(const po::variables_map& vm) {
	auto print_for_layer = [&] (int layer) {
		auto allSum = sumLayerTaps(layer);
		Table table;
		for (int i = 0; i <= 10; ++i) {
			auto fingerSum = sumFingerTapsLayer(i, layer);

			TableLine line;
			line.push_back("Hand: ");
			line.push_back(fingers.getHandName(i));
			line.push_back(", finger: ");
			line.push_back(fingers.getFingerName(i));
			line.push_back(", taps: ");
			line.push_back(std::to_string(fingerSum));
			line.push_back(", percent: ");
			line.push_back(getPercent(fingerSum, allSum));
			line.push_back("%");
			
			table.emplace_back(line);
		}
		tablePrint(table);
	};

	if (!vm.count("separate-by-layers")) {
		print_for_layer(-1);
	} else {
		for (int i = 0; i < all.onetap.layersCount(); ++i) {
			std::cout << "For layer: " << i << std::endl;
			print_for_layer(i);
			if (i+1 != all.onetap.layersCount())
				std::cout << std::endl; 
		}
	}
}

//-----------------------------------------------------------------------------
void sum_rows(const po::variables_map& vm) {
	auto print_for_layer = [&] (int layer) {
		auto allSum = sumLayerTaps(layer);
		Table table;
		for (int i = 5; i >= 0; --i) {
			auto rowSum = sumRowTapsLayer(i, layer);

			TableLine line;
			line.push_back("Row: ");
			line.push_back(fingers.getRowName(i));
			line.push_back(", taps: ");
			line.push_back(std::to_string(rowSum));
			line.push_back(", percent: ");
			line.push_back(getPercent(rowSum, allSum));
			line.push_back("%");
			
			table.emplace_back(line);
		}
		tablePrint(table);
	};

	if (!vm.count("separate-by-layers")) {
		print_for_layer(-1);
	} else {
		for (int i = 0; i < all.onetap.layersCount(); ++i) {
			std::cout << "For layer: " << i << std::endl;
			print_for_layer(i);
			if (i+1 != all.onetap.layersCount())
				std::cout << std::endl; 
		}
	}
}

//-----------------------------------------------------------------------------
void sum(const po::variables_map& vm) {
	std::string regime, fingerfile;
	bool useFingers = false;
	if (vm.count("regime"))
		regime = vm["regime"].as<std::string>();
	if(vm.count("fingerfile")) {
		fingerfile = vm["fingerfile"].as<std::string>();

		// Открыть файл и проверить его
		try {
			std::ifstream fin(fingerfile);
			if (fin) {
				fingers.load(fin);
				fin.close();
			} else {
				std::cout << "Fingers file didn't exists!" << std::endl;
				return;
			}
		} catch (std::exception& ex) {
			std::cout << "Fingers file is wrong! Exception: " << ex.what() << std::endl;
			return;
		}
		useFingers = true;
	} else if (regime == "fingers" || regime == "rows") {
		std::cout << "You not enter file with fingers specification." << std::endl;
	}
	
	if (regime == "all") {
		sum_all(vm);
	} else if (regime == "fingers") {
		sum_fingers(vm);	
	} else if (regime == "rows") {
		sum_rows(vm);
	} else {
		std::cout << "Unknown regime `" << regime << "` when using sum type." << std::endl;
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

int main(int argc, char* argv[]) {
	po::options_description desc("General options");
	desc.add_options()
		("help,h", "Show help")
		("statfile,s", po::value<std::string>(), "File with statistics. For default it is `heatmap_file.txt`.")
		("type,t", po::value<std::string>(), "Select statistics type: `sum`."
			//", `onetap`, `twotap`, `daily`."
			"\n  \t`sum` - sum keys count in different ways."
			//"\n  \t`onetap` - statistics of one tap.\n  \t`twotap` - count of two serial taps.\n  \t`daily` - statistics by each day."
			);

	po::options_description sum_desc("`sum` options");
	sum_desc.add_options()
		("regime,r", po::value<std::string>(), "Select sum regime: `all`, `fingers`, `rows`.\n  \t`all` - write count of all taps.\n  \t`fingers` - write count of taps by fingers.\n  \t`rows` - write count of taps by rows.")
		("fingerfile,f", po::value<std::string>(), "File with fingers and rows specification")
		("separate-by-layers,S", "Separate any statistics by layers.");

	/*po::options_description onetap_desc("`onetap` options");
	onetap_desc.add_options()
		("regime,r", po::value<std::string>(), "Select sum regime: `all`, `fingers`, `rows`.\n  \t`all` - write count of all taps.\n  \t`fingers` - write count of taps by fingers.\n  \t`rows` - write count of taps by rows.")
		("fingerfile,f", po::value<std::string>(), "File with fingers and rows specification");	*/

	po::variables_map vm;
	try {
		po::parsed_options parsed = po::command_line_parser(argc, argv).options(desc).allow_unregistered().run();
		po::store(parsed, vm);
		po::notify(vm);

		if (vm.count("statfile"))
			all.file = vm["statfile"].as<std::string>();

		all.load();

		if (vm.count("type")) {
			auto type = vm["type"].as<std::string>();
			if (type == "sum") {
				desc.add(sum_desc);
				po::store(po::parse_command_line(argc, argv, desc), vm);
				sum(vm);
			} else {
				std::cout << "Unknown type `" << type << "`." << std::endl;
			}
		} else if (vm.count("help")) {
			desc.add(sum_desc);
			std::cout << desc << std::endl;
		} else {
			std::cout << "You don't specify regime! Try `--help` for more information." << std::endl; 
		}
	} catch(std::exception& ex) {
		std::cout << ex.what() << std::endl;
	}

	//system("pause");
	return 0;
}