#include <stdio.h>
#include <stack>
#include <fstream>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <string.h>

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
std::mutex maze_mutex; // Mutex para proteger o acesso ao labirinto e à variável 'exit_found'

// Função que carrega o labirinto
pos_t load_maze(const char *file_name)
{
	pos_t initial_pos;
	string filename = file_name;
	ifstream file(filename);

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
			if (maze[i][j] == 'e') // Encontrar posição inicial
			{
				initial_pos.i = i;
				initial_pos.j = j;
			}
		}
	}
	file.close();
	return initial_pos;
}

// Função que imprime o labirinto
void print_maze()
{
	for (int i = 0; i < num_rows; ++i)
	{
		for (int j = 0; j < num_cols; ++j)
		{
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
}

// Função responsável pela navegação
bool walk(pos_t pos)
{
	system("clear||cls");
	print_maze();
	// Sincronizar acesso à variável global maze e exit_found
	std::lock_guard<std::mutex> lock(maze_mutex);

	if (maze[pos.i][pos.j] == 's')
	{
		return true;
	}
	if (pos.i < 0 || pos.i >= num_rows || pos.j < 0 || pos.j >= num_cols || maze[pos.i][pos.j] == '#' || maze[pos.i][pos.j] == 'o')
		return false;

	if (maze[pos.i][pos.j] == 'x' || maze[pos.i][pos.j] == 'e')
	{
		maze[pos.i][pos.j] = 'o'; // Marcar como visitado
	}

	// Criar threads para caminhos adicionais
	std::vector<std::thread> threads;
	// Direção 1: para cima
	if (pos.j + 1 < num_cols && maze[pos.i][pos.j + 1] != '#' && maze[pos.i][pos.j + 1] != 'o')
	{
		threads.emplace_back([&]()
							 {
			if (walk(pos_t{pos.i, pos.j + 1}))
			{
				return true;
			} });
	}

	// Direção 2: para baixo
	if (pos.j - 1 >= 0 && maze[pos.i][pos.j - 1] != '#' && maze[pos.i][pos.j - 1] != 'o')
	{
		threads.emplace_back([&]()
							 {
			if (walk(pos_t{pos.i, pos.j - 1}))
			{
				return true;
			} });
	}

	// Direção 3: para a esquerda
	if (pos.i - 1 >= 0 && maze[pos.i - 1][pos.j] != '#' && maze[pos.i - 1][pos.j] != 'o')
	{

		threads.emplace_back([&]()
							 {
			if (walk(pos_t{pos.i -1 , pos.j}))
			{
				return true;
			} });
	}

	// Direção 4: para a direita
	if (pos.i + 1 < num_rows && maze[pos.i + 1][pos.j] != '#' && maze[pos.i + 1][pos.j] != 'o')
	{

		threads.emplace_back([&]()
							 {
			if (walk(pos_t{pos.i +1 , pos.j}))
			{
				return true;
			} });
	}

	// Esperar todas as threads terminarem
	for (auto &t : threads)
	{
		if (t.joinable())
			t.join();
	}
	return false;
}

int main(int argc, char *argv[])
{
	pos_t initial_pos = load_maze(argv[1]);
	bool exit_found = walk(initial_pos);
	if (exit_found)
	{
		cout << "Exit found!" << endl;
	}
	else
	{
		cout << "Exit not found." << endl;
	}
	return 0;
}
