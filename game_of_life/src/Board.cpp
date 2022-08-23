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
	for (int i = 0; i < board_cells.size(); i++)
	{
		for (int j = 0; j < board_cells[i].size(); j++)
		{
			std::cout << debug_count_neighbours(board_cells[i][j]) << " ";
		}
		std::cout << std::endl;
	}
}

std::vector<std::vector<int>> Board::GetIntegerRep(CELL_TYPE type)
{
	std::vector<std::vector<int>> res;
	for (int i = 0; i < board_cells.size(); i++)
	{
		std::vector<int> v;
		for (int j = 0; j < board_cells[i].size(); j++)
		{
			if (board_cells[i][j].type == type && board_cells[i][j].active)
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
	if (i < 0 || i >= board_cells.size()) return;
	if (j < 0 || j >= board_cells[i].size()) return;
	board_cells[i][j].active = val;
}

bool Board::GetCellActive(int i, int j)
{
	if (i < 0 || i >= board_cells.size()) return false;
	if (j < 0 || j >= board_cells[i].size()) return false;
	return board_cells[i][j].active;
}

void Board::Update()
{
	for (int i = 0; i < board_cells.size(); i++)
	{
		for (int j = 0; j < board_cells[i].size(); j++)
		{
			function_map[board_cells[i][j].type](board_cells[i][j], buffer_board[i][j]);
		}
	}

	board_cells.swap(buffer_board);
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
	board_cells = std::vector<std::vector<Cell>>();
	buffer_board = std::vector<std::vector<Cell>>();

	for (int i = 0; i < height; i++)
	{
		std::vector<Cell> v;
		std::vector<Cell> b;
		for (int j = 0; j < width; j++)
		{
			v.push_back(Cell());
			b.push_back(Cell());
		}

		board_cells.push_back(v);
		buffer_board.push_back(b);
	}

	//Populate cell neighbours
	PopulateBoardNeighbours(board_cells);
	PopulateBoardNeighbours(buffer_board);
	

	//SET THE FUNCTION MAPS
	function_map[CELL_TYPE::SOLID] = SolidUpdateFunction;
	function_map[CELL_TYPE::CONWAY] = ConwayUpdateFunctions;
}



