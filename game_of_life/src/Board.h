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
	INVALID,
	BLANK
};



struct Cell {
private:

public:
	bool enabled = true;
	bool updated = false;
	Cell *l, *r, *u, *d, *ul, *ur, *dl, *dr;
	CELL_TYPE type = CELL_TYPE::BLANK;
	Cell(){
		l = r = u = d = ul = ur = dl = dr = nullptr;
	}

	Cell(CELL_TYPE type)
		: Cell()
	{
		this->type = type;
	}

	void SetType(CELL_TYPE nt)
	{
		type = nt;
	}

	CELL_TYPE GetType()
	{
		return type;
	}


	
};
typedef std::vector<std::vector<Cell>>(*BoardUpdateFunction) (const std::vector<std::vector<Cell>>&);


class Board
{
private:
	int width,height;
	std::vector<std::vector<Cell>> main_board;

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> cols;
	std::map<CELL_TYPE, glm::vec3> col_map =
	{
		{CELL_TYPE::SOLID, glm::vec3(1.0f, 1.0f, 1.0f)},
		{CELL_TYPE::CONWAY, glm::vec3(0.0f, 1.0f, 0.0f)},
		{CELL_TYPE::SAND, glm::vec3(0.5f, 0.3f, 0.2f)}
	};

	void PopulateBoardNeighbours(std::vector<std::vector<Cell>>& board);
	void UpdateCellPositionsAndColors();

	void MoveCell(Cell* src, Cell* dest);

public:
	Board(int x, int y);
	Cell* GetCell(std::vector<std::vector<Cell>>& board, int i, int j);
	Cell* GetMainBoardCell(int i, int j);
	void Debug();


	int GetWidth() { return width; }
	int GetHeight() { return height; }

	std::vector<std::vector<int>> GetIntegerRep(CELL_TYPE type);
	std::vector<int> GetIntegerRepFlat(CELL_TYPE type);
	std::vector<int> GetIntegerRepFlatAll();

	void SetCellType(int i, int j, CELL_TYPE new_type);
	CELL_TYPE GetCellType(int i, int j);

	std::vector<glm::vec3>& GetAllCellPositions() { UpdateCellPositionsAndColors(); return positions; }
	std::vector<glm::vec3>& GetAllCellColors() { UpdateCellPositionsAndColors(); return cols; }

	void Update();

	void UpdateConwayGeneration();
	void UpdateSand(Cell& cell);

	void Clear();
};


