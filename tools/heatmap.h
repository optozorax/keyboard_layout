#include <iostream>
#include <vector>
#include <map>

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
	OneTapHeatmap(int layersN, int rowsN, int colsN);

	void onTap(Tap tap);

	void save(std::ostream& out) const;
	void load(std::istream& in);

	void resize(int layersN, int rowsN, int colsN);

	int layersCount(void) const;
	int rowsCount(void) const;
	int colsCount(void) const;
	int getTapCount(Tap tap) const;
private:
	int counter;
	std::vector<Layer> layers_heatmap;
};

/** Статистика двух нажатий, когда одно идет за другим. Как часто та или иная комбинация встречается. */
class TwoTapHeatmap : public Heatmap
{
public:
	TwoTapHeatmap(int layersN, int rowsN, int colsN);

	void onTap(Tap tap);

	void save(std::ostream& out) const;
	void load(std::istream& in);

	void resize(int layersN, int rowsN, int colsN);

	int layersCount(void) const;
	int rowsCount(void) const;
	int colsCount(void) const;
	int getTapCount(Tap first, Tap second) const; // У второго слой считается таким же как и у первого

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
	DayHeatmap();
	void onTap(Tap tap);

	void save(std::ostream& out) const;
	void load(std::istream& in);
private:
	void updateToday();

	int counter;
	int today;
	std::map<int, int32_t> statistics;
};

class AllStatistics
{
public:
	AllStatistics(const std::string& file);

	void load(void);
	void save(void) const;

	OneTapHeatmap onetap;
	TwoTapHeatmap twotap;
	DayHeatmap    daytap;	
	std::string   file;
};
