
#include "pch.h"

#include <iostream>
#include <array>
#include <stack>
#include <ctime>

size_t least_prime_divisor(size_t n)
{
	for (size_t i = 2; i * i <= n; ++i)
		if (n % i == 0)
			return i;
	return n;
}

//структура, хранящая координаты
struct Square 
{
	size_t x;
	size_t y;
	size_t w;
};

template <size_t N>
class Table
{
public:
	//конструктор
	Table(size_t n)
		: length(least_prime_divisor(n)), multiplier(n / length), currArea(length * length), height(), currSolution(), bestSolution(), currEvaluation(0), bestEvaluation(length + 4)
	{
		height.fill(0);
	}

	//осовная функция, использующая бэктрекинг
	void solve()
	{
		Square square = { 0, 0, length };
		set(square);
		while (!currSolution.empty())
		{
			//заполнение поля, пока вариант оптимален
			while (currEvaluation + 1 < bestEvaluation && !filled())
			{
				size_t abscissa = next_position();
				square = { abscissa, height[abscissa], max_width(abscissa) };
				set(square);
			}

			//сохранение результата
			if (filled() && currEvaluation < bestEvaluation && currEvaluation != 1)
			{
				save();
			}

			do
			{
				relax();
			} 
			while (currEvaluation >= bestEvaluation);
		}
	}

	//вставка квадрата
	void set(const Square& square)
	{
		++currEvaluation;
		currArea -= square.w * square.w;
		for (auto iter = height.begin() + square.x; iter != height.begin() + square.x + square.w; ++iter)
			*iter += square.w;
		currSolution.push(square);
		//std::cout << "set" << std::endl;
		//output();
	}

	//удаление квадрата
	Square reset()
	{
		--currEvaluation;
		Square square = currSolution.top();
		currArea += square.w * square.w;
		currSolution.pop();
		for (auto iter = height.begin() + square.x; iter != height.begin() + square.x + square.w; ++iter)
			*iter -= square.w;
		//std::cout << "reset" << std::endl;
		//output();
		return (square);
	}

	//проверка на заполненность поля
	bool filled()
	{
		return (currArea == 0);
	}

	size_t next_position()
	{
		size_t minimum_index = 0;
		for (size_t i = 0; i < length; ++i)
			if (height[i] < height[minimum_index])
				minimum_index = i;
		return (minimum_index);
	}

	size_t max_width(size_t abscissa)
	{
		size_t width = 0;
		while (length - abscissa > width && length - height[abscissa + width] > width && width < length - 1 && height[abscissa] >= height[abscissa + width])
		{
			++width;
		}
		return (width);
	}

	void output()
	{
		std::cout << "Current Number:" << currEvaluation << std::endl;
		std::stack<Square> outputSolution = currSolution;
		std::cout << "Current Stack:" << std::endl;
		while (!outputSolution.empty())
		{
			Square outputsquare = outputSolution.top();
			std::cout << outputsquare.x * multiplier + 1 << " " << outputsquare.y * multiplier + 1 << " " << outputsquare.w * multiplier << std::endl;
			outputSolution.pop();
		}
	}

	//вывод результата
	void print_answer()
	{
		std::stack<Square> tempSolution = bestSolution;
		std::cout << bestEvaluation << std::endl;
		while (!tempSolution.empty())
		{
			Square square = tempSolution.top();
			std::cout << square.x * multiplier + 1 << " " << square.y * multiplier + 1 << " " << square.w * multiplier << std::endl;
			tempSolution.pop();
		}
	}

	void save()
	{
		bestSolution = currSolution;
		bestEvaluation = currEvaluation;
	}

	void relax()
	{
		Square square;
		do
		{
			square = reset();
		} while (square.w == 1 && currEvaluation);
		if (square.w != 1)
		{
			--square.w;
			set(square);
		}
	}
private:
	size_t length;
	size_t multiplier;
	size_t currArea;
	std::array<size_t, N> height;
	std::stack<Square> currSolution;
	std::stack<Square> bestSolution;
	size_t currEvaluation;
	size_t bestEvaluation;
};


int main()
{
	clock_t start, stop;
	start = clock();
	size_t N;
	std::cin >> N;

	Table<40> desk(N);
	desk.solve();
	desk.print_answer();

	stop = clock();
	std::cout << "time:" << (stop - start) / CLOCKS_PER_SEC;

	return (EXIT_SUCCESS);
}
