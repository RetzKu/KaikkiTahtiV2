#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <deque>

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
		std::vector<Cell> previousSteps;
	};

	Path(uint8_t* map, int width, int height);
	~Path();

	void Calculate(Cell startCell, Cell endCell);
	bool TimeIsUp();

	Cell* PathSteps;
	int PathStepsCount = 0;
	Cell endCell;
private:
	uint8_t* mapData;
	int mapWidth;
	int mapHeight;
	StepInfo* openSteps[3];
	int stepCount = 0;

	std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();

private:
	void GetBestStep(StepInfo* currentStep);
	StepInfo CalculateStep(StepInfo* currentStep, Cell newPosition);
	void AddStep(StepInfo step);
};

