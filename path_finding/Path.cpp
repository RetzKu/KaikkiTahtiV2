#include "Path.h"

float D1 = 1;
float D2 = 1.4444f;

#define CellCout(x, y) std::cout << " Added X: " << x << " Y: " << y << std::endl;

Path::Path(uint8_t* map, int width, int height)
{
	this->mapData = map;
	this->mapHeight = height;
	this->mapWidth = width;
}

Path::~Path()
{

}

void Path::Calculate(Cell startCell, Cell endCell)
{
	this->endCell = endCell;
	bool found = false;
	StepInfo firstStep;
	firstStep.currentCell = startCell;
	GetBestStep(&firstStep);
}

bool Path::NotInClosed(const Cell next)
{
	for (StepInfo step : closedStep)
	{
		if (step.currentCell.x == next.x && step.currentCell.y == next.y)
		{
			return true;
		}
	}
	return false;
}

void Path::GetBestStep(StepInfo* currentStep)
{
	std::vector<StepInfo> possibleSteps;
	Cell currentCell = currentStep->currentCell;
	for (int x = -1; x < 2; x++)
	{
		for (int y = -1; y < 2; y++)
		{
			uint8_t* pix = &mapData[3 * ((y + currentCell.y)*mapWidth + (x + currentCell.x))]; // get pixel
			uint8_t r = pix[2];
			uint8_t g = pix[1];
			uint8_t b = pix[0];
			if (r == 255 && g == 255 && b == 255 || NotInClosed(Cell(currentCell.x + x, currentCell.y +y)))
			{
				CellCout(currentCell.x + x, currentCell.y + y);
				pix[1] = 0;
				StepInfo newStep = CalculateStep(currentStep, Cell(currentCell.x + x, currentCell.y + y));
				closedStep.push_back(newStep);
				possibleSteps.push_back(newStep);
			}
		}
	}
	StepInfo bstCandi = possibleSteps[0];
	for (StepInfo best : possibleSteps)
	{
		if (best.walked- best.score > bstCandi.walked - bstCandi.score)
		{
			bstCandi = best;
		}
	}
	if (bstCandi.currentCell.x == endCell.x && bstCandi.currentCell.y == endCell.y || TimeIsUp())
	{
		return;
	}
	GetBestStep(&bstCandi);
}

bool Path::TimeIsUp()
{
	auto consumedTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - startTime);
	if (consumedTime.count() > 10)
	{
		return true;
	}
	return false;
}


Path::StepInfo Path::CalculateStep(StepInfo* currentStep, Cell newPosition)
{
	StepInfo newStep;
	newStep.previousSteps = currentStep->previousSteps;
	newStep.currentCell = newPosition;

	float dx = std::abs(newPosition.x - endCell.x);
	float dy = std::abs(newPosition.y - endCell.y);
	float returnvalue = D1 * (dx + dy) + (D2 - 2 * D1) * std::min(dx, dy);
	int distanceScore = returnvalue * 4.7f;
	newStep.score = distanceScore;
	if (currentStep->currentCell.x - newPosition.x != 0 && currentStep->currentCell.y - newPosition.y != 0)
	{
		newStep.walked = 1.4f*10 + currentStep->walked;
	}
	else if (currentStep->currentCell.x - newPosition.x != 0 || currentStep->currentCell.y - newPosition.y != 0)
	{ 
		newStep.walked = 1*10 + currentStep->walked;
	}

	return newStep;
}