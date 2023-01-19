/*#include<stdio.h>
#include<string.h>
#include<math.h>
#include<malloc.h>
#include <stdlib.h>
#include <time.h>
#include<iostream>
#include"Matrix.h"
#include"Rnn.h"
int main(int agrv,char*agrc[])
{
	//for (size_t i = 0; i < 1000; i++)
	//{
	//	Matrix a1(3, 3);
	//	Matrix a2(3, 3);
	//
	//	a2.vconcat(a1);
	//	Matrix a3(a2);
	//	Matrix a4;
	//	a4=a3.vconcat(a2);
	//	std::cout <<"4\n  " << a4 ;
	//	std::cout<< "\n\n";
	//	std::cout << "3\n  " << a3;
	//	std::cout << "\n\n";
	//	std::cout << "2\n  " << a2;
	//
	//}

	Rnn a(5, 5);
	Matrix i(1, 5);
	i = i * 0.10;
	std::cout << i << "\n";
	std::cout<<a.OutPut(i);
	return 0;
}
*/