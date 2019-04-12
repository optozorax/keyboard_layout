#include <iostream>
#include <iomanip>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#include "heatmap.h"

namespace po = boost::program_options;

AllStatistics all("heatmap_file.txt");
Fingers fingers;
Names names;

int sumAllTaps(void) {
	int32_t sum = 0;
	for (int i = 0; i < all.onetap.layersCount(); ++i) {
		for (int j = 0; j < all.onetap.rowsCount(); ++j) {
			for (int k = 0; k < all.onetap.colsCount(); ++k) {
				sum += all.onetap.getTapCount({k, j, i});
			}
		}
	}
	return sum;
}

int sumFingerTaps(int finger) {
	int32_t sum = 0;
	for (int i = 0; i < all.onetap.layersCount(); ++i) {
		for (int j = 0; j < all.onetap.rowsCount(); ++j) {
			for (int k = 0; k < all.onetap.colsCount(); ++k) {
				Tap tap{k, j, i};
				if (fingers.getFinger(tap) == finger)
					sum += all.onetap.getTapCount(tap);
			}
		}
	}
	return sum;
}

int sumRowTaps(int row) {
	int32_t sum = 0;
	for (int i = 0; i < all.onetap.layersCount(); ++i) {
		for (int j = 0; j < all.onetap.rowsCount(); ++j) {
			for (int k = 0; k < all.onetap.colsCount(); ++k) {
				Tap tap{k, j, i};
				if (fingers.getRow(tap) == row)
					sum += all.onetap.getTapCount(tap);
			}
		}
	}
	return sum;
}

void sum(const po::variables_map& vm) {
	std::string regime, fingerfile;
	bool useFingers = false;
	if (vm.count("regime"))
		regime = vm["regime"].as<std::string>();
	if(vm.count("fingerfile")) {
		fingerfile = vm["fingerfile"].as<std::string>();

		// Открыть файл и проверить его
		useFingers = true;
	}
	
	if (regime == "all") {
		std::cout << "Count of all taps: " << sumAllTaps() << std::endl;
	} else if (regime == "fingers") {
		if (useFingers) {
			auto allSum = sumAllTaps();
			for (int i = 0; i <= 10; ++i) {
				auto fingerSum = sumFingerTaps(i); 
				std::cout << "Finger: " << std::setw(2) << i << ", taps: " << std::setw(8) << fingerSum << ", percent: " << std::setprecision(2) << std::fixed << double(fingerSum)/allSum << std::endl;	
			}
		} else {
			std::cout << "You not enter file with fingers specification." << std::endl;
		}
	} else if (regime == "rows") {
		if (useFingers) {
			auto allSum = sumAllTaps();
			for (int i = 0; i <= 5; ++i) {
				auto rowSum = sumRowTaps(i); 
				std::cout << "Row: " << i << ", taps: " << std::setw(8) << rowSum << ", percent: " << std::setprecision(2) << std::fixed << double(rowSum)/allSum << std::endl;	
			}
		} else {
			std::cout << "You not enter file with fingers specification." << std::endl;
		}
	} else {
		std::cout << "Unknown regime `" << regime << "` when using sum type." << std::endl;
	}
}

int main(int argc, char* argv[]) {
	po::options_description desc("General options");
	desc.add_options()
		("help,h", "Show help")
		("statfile,s", po::value<std::string>(), "File with statistics. For default it is `heatmap_file.txt`.")
		("type,t", po::value<std::string>(), "Select statistics type: `sum`, `onetap`, `twotap`, `daily`.\n  \t`sum` - sum keys count in different ways.\n  \t`onetap` - statistics of one tap.\n  \t`twotap` - count of two serial taps.\n  \t`daily` - statistics by each day.");

	po::options_description sum_desc("`sum` options");
	sum_desc.add_options()
		("regime,r", po::value<std::string>(), "Select sum regime: `all`, `fingers`, `rows`.\n  \t`all` - write count of all taps.\n  \t`fingers` - write count of taps by fingers.\n  \t`rows` - write count of taps by rows.")
		("fingerfile,f", po::value<std::string>(), "File with fingers and rows specification");

	po::options_description onetap_desc("`onetap` options");
	onetap_desc.add_options()
		("regime,r", po::value<std::string>(), "Select sum regime: `all`, `fingers`, `rows`.\n  \t`all` - write count of all taps.\n  \t`fingers` - write count of taps by fingers.\n  \t`rows` - write count of taps by rows.")
		("fingerfile,f", po::value<std::string>(), "File with fingers and rows specification");	

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

	system("pause");
	return 0;
}