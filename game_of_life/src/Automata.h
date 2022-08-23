#pragma once
#include "Board.h"
#include "Util.h"
/// FUNCTIONS FOR DIFFERENT AUTOMATA
void SolidUpdateFunction(const Cell& cell, Cell& dest_cell)
{
	dest_cell.active = !cell.active;
}

void ConwayUpdateFunctions(const Cell& cell, Cell& dest_cell)
{
	int count = 0;
	auto count_cell = [](Cell* c) {
		if (c != nullptr && c->active) return 1;
		else return 0;
	};

	count += count_cell(cell.l);
	count += count_cell(cell.r);
	count += count_cell(cell.u);
	count += count_cell(cell.d);
	count += count_cell(cell.ur);
	count += count_cell(cell.ul);
	count += count_cell(cell.dr);
	count += count_cell(cell.dl);
	
	if (cell.active)
	{
		if (count == 2 || count == 3)
		{
			dest_cell.active = true;
		}
		else
		{
			dest_cell.active = false;
		}
	}
	else
	{
		if (count == 3)
		{
			dest_cell.active = true;
		}
		else
		{
			dest_cell.active = false;
		}
	}

	//std::cout << count << " ";
}

