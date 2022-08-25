#include "Board.h"


Cell* Board::GetCell(std::vector<std::vector<Cell>>& board, int i, int j)
{
	if (i < 0 || i >= board.size()) return nullptr;
	if (j < 0 || j >= board.size()) return nullptr;
	return &board[i][j];
}

Cell* Board::GetMainBoardCell(int i, int j)
{
	return GetCell(main_board, i, j);
}

void Board::Debug()
{
	

	for (int i = 0; i < main_board.size(); i++)
	{
		for (int j = 0; j < main_board[i].size(); j++)
		{
			Cell cell = main_board[i][j];
			//std::cout << (int)main_board[i][j].type << " ";
			
			int count = 0;
			auto count_cell = [](Cell p, Cell* c) {
				if (c != nullptr && c->type == CELL_TYPE::CONWAY) return 1;
				else return 0;
			};

			count += count_cell(cell, cell.l);
			count += count_cell(cell, cell.r);
			count += count_cell(cell, cell.u);
			count += count_cell(cell, cell.d);
			count += count_cell(cell, cell.ur);
			count += count_cell(cell, cell.ul);
			count += count_cell(cell, cell.dr);
			count += count_cell(cell, cell.dl);

			std::cout << count << " ";
		}
		std::cout << std::endl;
	}
}

void Board::Clear()
{

	for (int i = 0; i < main_board.size(); i++)
	{
		for (int j = 0; j < main_board[i].size(); j++)
		{
			main_board[i][j].SetType(CELL_TYPE::BLANK);


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
			if(main_board[i][j].GetType() == type)
				v.push_back(1);
			else
				v.push_back(0);
		}
		res.push_back(v);
	}

	return res;
}


void Board::Update()
{
	for (int i = 0; i < main_board.size(); i++)
	{
		for (int j = 0; j < main_board[i].size(); j++)
		{
			if (main_board[i][j].updated) continue;
			switch (main_board[i][j].GetType())
			{
			case CELL_TYPE::SAND:
				UpdateSand(main_board[i][j]);
				break;
			case CELL_TYPE::SOLID:
				break;
			}
		}
	}
	UpdateConwayGeneration();

	PopulateBoardNeighbours(main_board);
}

void Board::UpdateConwayGeneration()
{
	std::vector<std::vector<Cell>> buffer_board = main_board;

	for (int i = 0; i < main_board.size(); i++)
	{
		for (int j = 0; j < main_board[i].size(); j++)
		{
			Cell cell = main_board[i][j];
			Cell& dest_cell = buffer_board[i][j];

			int count = 0;
			auto count_cell = [](Cell p, Cell* c) {
				if (c != nullptr && c->type == CELL_TYPE::CONWAY) return 1;
				else return 0;
			};
			count += count_cell(cell, cell.l);
			count += count_cell(cell, cell.r);
			count += count_cell(cell, cell.u);
			count += count_cell(cell, cell.d);
			count += count_cell(cell, cell.ur);
			count += count_cell(cell, cell.ul);
			count += count_cell(cell, cell.dr);
			count += count_cell(cell, cell.dl);

			if (cell.GetType() == CELL_TYPE::CONWAY)
			{
				if (count == 2 || count == 3)
				{
					dest_cell.SetType(CELL_TYPE::CONWAY);
				}
				else
				{
					dest_cell.SetType(CELL_TYPE::BLANK);
				}
			}
			else if (cell.GetType() == CELL_TYPE::BLANK)
			{
				if (count == 3)
				{
					dest_cell.SetType(CELL_TYPE::CONWAY);
				}
				else
				{
					dest_cell.SetType(CELL_TYPE::BLANK);
				}
			}
			else
			{
				dest_cell = cell;
			}


		}
	}

	main_board.swap(buffer_board);
}

void Board::UpdateSand(Cell& cell)
{
	
	if (cell.GetType() != CELL_TYPE::SAND) return;
	
	if (cell.d != nullptr && cell.d->GetType() == CELL_TYPE::BLANK)
	{
		int g = 3;
		Cell* c = &cell;
		Cell* d = cell.d;
		while (g != 0 && d != nullptr && d->GetType() == CELL_TYPE::BLANK)
		{
			if (d->updated) break;
			d->updated = true;
			c = d;
			d = d->d;
			g--;
		}
		MoveCell(&cell, c);
	}
	else if (cell.dl != nullptr && cell.dl->GetType() == CELL_TYPE::BLANK)
	{
		int g = 1;
		Cell* c = &cell;
		Cell* d = cell.dl;
		while (g != 0 && d != nullptr && d->GetType() == CELL_TYPE::BLANK)
		{
			if (d->updated) break;
			d->updated = true;
			c = d;
			d = d->dl;
			g--;
		}
		MoveCell(&cell, c);
	}
	else if (cell.dr != nullptr && cell.dr->GetType() == CELL_TYPE::BLANK)
	{
		int g = 1;
		Cell* c = &cell;
		Cell* d = cell.dr;
		while (g != 0 && d != nullptr && d->GetType() == CELL_TYPE::BLANK)
		{
			if (d->updated) break;
			d->updated = true;
			c = d;
			d = d->dr;
			g--;
		}
		MoveCell(&cell, c);
	}

	cell.updated = true;
}


std::vector<int> Board::GetIntegerRepFlat(CELL_TYPE type)
{
	std::vector<int> v;
	for (int i = 0; i < main_board.size(); i++)
	{
		
		for (int j = 0; j < main_board[i].size(); j++)
		{
			if (main_board[i][j].GetType() == type)
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
			if (main_board[i][j].GetType() != CELL_TYPE::BLANK)
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
			c.updated = false;
			c.l = GetCell(board, i, j - 1);
			c.r = GetCell(board, i, j + 1);
			c.d = GetCell(board, i + 1, j);
			c.u = GetCell(board, i - 1, j);

			c.dr = GetCell(board, i + 1, j+1);
			c.dl = GetCell(board, i + 1, j-1);
			c.ur = GetCell(board, i - 1, j+1);
			c.ul = GetCell(board, i - 1, j-1);
			
		}
	}

	
}

void Board::UpdateCellPositionsAndColors()
{
	positions.clear();
	cols.clear();
	// Update positions and colors
	for (int i = 0; i < main_board.size(); i++)
	{
		for (int j = 0; j < main_board[i].size(); j++)
		{
			positions.push_back(
				glm::vec3(j, i, 0)
			);

			CELL_TYPE t = GetCellType(i, j);
			cols.push_back(col_map[t]);
		}
	}
}

void Board::MoveCell(Cell* src, Cell* dest)
{
	dest->SetType(src->GetType());
	src->SetType(CELL_TYPE::BLANK);
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
	

}



