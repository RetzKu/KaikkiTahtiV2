#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>

struct Cell
{
	Cell(int x, int y) { this->x = x; this->y = y; }
	Cell() {}
	int x = 0;
	int y = 0;
};


class Path
{
public:
	struct StepInfo
	{
		int walked = 0;
		int score = 999999999;
		Cell currentCell;
		std::vector<StepInfo> previousSteps;
	};

	Path(uint8_t* map, int width, int height);
	~Path();

	void Calculate(Cell startCell, Cell endCell);
	bool NotInClosed(const Cell next);
	bool TimeIsUp();

	Cell* PathSteps;
	int PathStepsCount = 0;
	Cell endCell;
private:
	uint8_t* mapData;
	int mapWidth;
	int mapHeight;
	std::vector<StepInfo> openSteps;
	std::vector<StepInfo> closedStep;

	std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();

private:
	void GetBestStep(StepInfo* currentStep);
	StepInfo CalculateStep(StepInfo* currentStep, Cell newPosition);
};

