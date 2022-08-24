#pragma once
#include "Board.h"
#include "Util.h"
/// FUNCTIONS FOR DIFFERENT AUTOMATA
Cell SolidUpdateFunction(const Cell& cell)
{
	Cell dest_cell = cell;

	return dest_cell;
}

Cell ConwayUpdateFunctions(const Cell& cell)
{
	Cell dest_cell;
	dest_cell.type = cell.type;


	int count = 0;
	auto count_cell = [](Cell p, Cell* c) {
		if (c != nullptr /* && c->type == p.type*/ && c->active) return 1;
		else return 0;
	};

	count += count_cell(cell,cell.l);
	count += count_cell(cell,cell.r);
	count += count_cell(cell,cell.u);
	count += count_cell(cell,cell.d);
	count += count_cell(cell,cell.ur);
	count += count_cell(cell,cell.ul);
	count += count_cell(cell,cell.dr);
	count += count_cell(cell,cell.dl);
	
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

	return dest_cell;
}

