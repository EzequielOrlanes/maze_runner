#include <stdio.h>
#include <stack>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;
// Representação de uma posição
struct pos_t
{
	int i;
	int j;
};
static vector<vector<char>> maze(0);

pos_t load_maze(const char *file_name)
{
	pos_t initial_pos;
	string filename = file_name;
	ifstream file(filename);
	// Verificar se o arquivo foi aberto com sucesso
	if (!file.is_open())
	{
		cout << "Erro ao abrir o arquivo!" << endl;
	}
	// Ler as dimensões da primeira linha
	file >> num_rows >> num_cols;
	maze.resize(num_rows);
	for (int i = 0; i < num_rows; i++)
	{
		maze[i].resize(num_cols);
	}
	string linha_maze;
	// Ler o labirinto caractere por caractere
	for (int i = 0; i < num_rows; i++)
	{
		getline(file, linha_maze, '\n');
		for (int j = 0; j < num_cols; j++)
		{
			maze[i][j] = linha_maze[j];
			if (maze[i][j] == 'e')
			{
				initial_pos.i = i;
				initial_pos.j = j;
			}
		}
	}
	// Fechar o arquivo
	file.close();
	return initial_pos;
};
// Função que imprime o labirinto
void print_maze()
{
	for (int i = 1; i < num_rows; ++i)
	{
		for (int j = 0; j < num_cols; ++j)
		{
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
}
// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
bool walk(pos_t pos)
{
	// clean up terminal
	system("clear||cls");
	print_maze();
	if (maze[pos.i][pos.j] == 's')
		return true;
	if (pos.i < 0 || pos.i >= num_rows || pos.j < 0 || pos.j >= num_cols)
		return false;
	if (maze[pos.i][pos.j] == '#' || maze[pos.i][pos.j] == 'o')
		return false;
	if (maze[pos.i][pos.j] == 'x' || maze[pos.i][pos.j] == 'e')
		maze[pos.i][pos.j] = 'o';
	// walk up
	if (walk({pos.i, pos.j + 1}))
		return true;
	// walk down
	if (walk({pos.i, pos.j - 1}))
		return true;
	// walk left
	if (walk({pos.i - 1, pos.j}))
		return true;
	// walk right
	if (walk({pos.i + 1, pos.j}))
		return true;
	// exit not found
	return false;
}

int main(int argc, char *argv[])
{
	pos_t initial_pos = load_maze(argv[1]);
	bool exit_found = walk(initial_pos);
	if (exit_found)
	{
		cout << "Exit found.";
	}
	else
	{
		cout << "Exit not found.";
	}
	return 0;
}
