#include "Board.h"

#include "Automata.h"


Cell* Board::GetCell(std::vector<std::vector<Cell>>& board, int i, int j)
{
	if (i < 0 || i >= board.size()) return nullptr;
	if (j < 0 || j >= board.size()) return nullptr;
	return &board[i][j];
}

void Board::Debug()
{
	/*for (int i = 0; i < main_board.size(); i++)
	{
		for (int j = 0; j < main_board[i].size(); j++)
		{
			Cell cell = main_board[i][j];
			std::cout << main_board[i][j].active << " ";
			
		}
		std::cout << std::endl;
	}*/
}

void Board::Clear()
{

	for (int i = 0; i < main_board.size(); i++)
	{
		for (int j = 0; j < main_board[i].size(); j++)
		{
			main_board[i][j].active = false;
		}
	}
}

std::vector<std::vector<int>> Board::GetIntegerRep(CELL_TYPE type)
{
	std::vector<std::vector<int>> res;
	for (int i = 0; i < main_board.size(); i++)
	{
		std::vector<int> v;
		for (int j = 0; j < main_board[i].size(); j++)
		{
			if (main_board[i][j].type == type && main_board[i][j].active)
				v.push_back(1);
			else
				v.push_back(0);
		}
		res.push_back(v);
	}

	return res;
}

void Board::SetCellActive(int i, int j, bool val)
{
	if (i < 0 || i >= main_board.size()) return;
	if (j < 0 || j >= main_board[i].size()) return;
	main_board[i][j].active = val;
}

bool Board::GetCellActive(int i, int j)
{
	if (i < 0 || i >= main_board.size()) return false;
	if (j < 0 || j >= main_board[i].size()) return false;
	return main_board[i][j].active;
}

void Board::Update()
{
	PopulateBoardNeighbours(main_board);
	std::vector<std::vector<Cell>> buffer_board;
	for (int i = 0; i < main_board.size(); i++)
	{
		std::vector<Cell> temp;
		for (int j = 0; j < main_board[i].size(); j++)
		{
			Cell c = function_map[main_board[i][j].type](main_board[i][j]);
			temp.push_back(c);
		}

		buffer_board.push_back(temp);
	}

	main_board.swap(buffer_board);

	
}

std::vector<int> Board::GetIntegerRepFlat(CELL_TYPE type)
{
	std::vector<int> v;
	for (int i = 0; i < main_board.size(); i++)
	{
		
		for (int j = 0; j < main_board[i].size(); j++)
		{
			if (main_board[i][j].type == type && main_board[i][j].active)
				v.push_back(1);
			else
				v.push_back(0);
		}
		
	}

	return v;
}

std::vector<int> Board::GetIntegerRepFlatAll()
{
	std::vector<int> v;
	for (int i = 0; i < main_board.size(); i++)
	{

		for (int j = 0; j < main_board[i].size(); j++)
		{
			if (main_board[i][j].active)
				v.push_back(1);
			else
				v.push_back(0);
		}

	}

	return v;
}

void Board::SetCellType(int i, int j, CELL_TYPE newType)
{
	Cell* c = GetCell(main_board, i, j);
	
	if (c != nullptr) c->type = newType;
}

CELL_TYPE Board::GetCellType(int i, int j)
{
	Cell* c = GetCell(main_board, i, j);

	if (c != nullptr) return c->type;
	else
		return CELL_TYPE::INVALID;
}


void Board::PopulateBoardNeighbours(std::vector<std::vector<Cell>>& board)
{
	for (int i = 0; i < board.size(); i++)
	{
		for (int j = 0; j < board[i].size(); j++)
		{
			Cell& c = board[i][j];
			
			c.l = GetCell(board, i, j - 1);
			c.r = GetCell(board, i, j + 1);
			c.u = GetCell(board, i + 1, j);
			c.d = GetCell(board, i - 1, j);

			c.ur = GetCell(board, i + 1, j+1);
			c.ul = GetCell(board, i + 1, j-1);
			c.dr = GetCell(board, i - 1, j+1);
			c.dl = GetCell(board, i - 1, j-1);
			
		}
	}
}

Board::Board(int x, int y)
	: width(x), height(y)
{
	//Create the cells
	main_board = std::vector<std::vector<Cell>>();

	for (int i = 0; i < height; i++)
	{
		std::vector<Cell> v;
		std::vector<Cell> b;
		for (int j = 0; j < width; j++)
		{
			v.push_back(Cell());
			b.push_back(Cell());
		}

		main_board.push_back(v);

	}

	//Populate cell neighbours
	PopulateBoardNeighbours(main_board);
	

	//SET THE FUNCTION MAPS
	function_map[CELL_TYPE::SOLID] = SolidUpdateFunction;
	function_map[CELL_TYPE::CONWAY] = ConwayUpdateFunctions;
}



