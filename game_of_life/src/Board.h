#pragma once
#include <vector>
#include <unordered_map>
#include <map>

#include <glm/glm.hpp>

#include "Shader.h"

enum class CELL_TYPE {
	SOLID,
	CONWAY,
	SAND
};

enum class DIRECTION {
	RIGHT = 0,
	LEFT,
	UP,
	DOWN,
	UP_LEFT,
	UP_RIGHT,
	DOWN_LEFT,
	DOWN_RIGHT,
};





struct Cell {
public:

	bool enabled = true;
	bool active = false;
	Cell *l, *r, *u, *d, *ul, *ur, *dl, *dr;
	CELL_TYPE type = CELL_TYPE::CONWAY;
	Cell(){
	}

	Cell(CELL_TYPE type)
		: Cell()
	{
		this->type = type;
	}

	
};

typedef void (*CellUpdateFunction) (const Cell&, Cell&);

class Board
{
private:
	int width,height;
	std::vector<std::vector<Cell>> board_cells;
	std::vector<std::vector<Cell>> buffer_board;
	std::unordered_map<CELL_TYPE, CellUpdateFunction> function_map;

	void PopulateBoardNeighbours(std::vector<std::vector<Cell>>& board);
public:
	Board(int x, int y);
	Cell* GetCell(std::vector<std::vector<Cell>>& board, int i, int j);

	void Debug();
	int debug_count_neighbours(const Cell& c)
	{
		int count = 0;

		auto count_cell = [] (Cell* c) {
			if (c != nullptr && c->active) return 1;
			else return 0;
		};

		count += count_cell(c.l);
		count += count_cell(c.r);
		count += count_cell(c.u);
		count += count_cell(c.d);
		count += count_cell(c.ur);
		count += count_cell(c.ul);
		count += count_cell(c.dr);
		count += count_cell(c.dl);
		
		return count;
	}

	int GetWidth() { return width; }
	int GetHeight() { return height; }

	std::vector<std::vector<int>> GetIntegerRep(CELL_TYPE type);

	void SetCellActive(int i, int j, bool val);
	bool GetCellActive(int i, int j);

	void Update();
};


