#pragma once
#include <vector>
#include <unordered_map>
#include <map>

#include <glm/glm.hpp>

#include "Shader.h"

enum class CELL_TYPE {
	SOLID,
	CONWAY,
	SAND, 
	INVALID
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
		l = r = u = d = ul = ur = dl = dr = nullptr;
	}

	Cell(CELL_TYPE type)
		: Cell()
	{
		this->type = type;
	}

	
};

typedef Cell (*CellUpdateFunction) (const Cell&);

class Board
{
private:
	int width,height;
	std::vector<std::vector<Cell>> main_board;
	std::unordered_map<CELL_TYPE, CellUpdateFunction> function_map;

	void PopulateBoardNeighbours(std::vector<std::vector<Cell>>& board);
public:
	Board(int x, int y);
	Cell* GetCell(std::vector<std::vector<Cell>>& board, int i, int j);

	void Debug();


	int GetWidth() { return width; }
	int GetHeight() { return height; }

	std::vector<std::vector<int>> GetIntegerRep(CELL_TYPE type);
	std::vector<int> GetIntegerRepFlat(CELL_TYPE type);
	std::vector<int> GetIntegerRepFlatAll();

	void SetCellActive(int i, int j, bool val);
	bool GetCellActive(int i, int j);

	void SetCellType(int i, int j, CELL_TYPE new_type);
	CELL_TYPE GetCellType(int i, int j);

	void Update();
};


