#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<Windows.h>
//#define OPEN_THREAD//开启线程处理，有问题且提升不大
#include"Network.h"

using namespace std;
int main() {
	Text t("D:/acpp/NN/Debug/read.txt");
	vector<double> goal = { 0.5 };
	Matrix goal_ = goal;
	Network n(2,16,1);
	list<string>::iterator it;
	for (size_t i = 0; i < 10000; i++)
	{
		for ( it = t.str_list.begin();it!=t.str_list.end(); it++)
		{
			vector<double> input = Text<double>::unicode(*it);
			input.resize(16);
			Matrix input_ = input;
			input_.rotate();
			cout << n.output(input_);
			n.train(goal_);
		}

	}
	for (it = t.str_list.begin(); it != t.str_list.end(); it++)
	{
		vector<double> input = Text<double>::unicode(*it);
		input.resize(16);
		Matrix input_ = input;
		input_.rotate();
		n.output(input_);
		cout << "e:" << n.get_e(goal_) << endl;
	}

	Text t2("D:/acpp/NN/Debug/read1.txt");
	vector<double> input = Text<double>::unicode(*t2.str_list.begin());
	input.resize(16);
	Matrix input_ = input;
	cout << input_;
	input_.rotate();
	for (it = t.str_list.begin(); it != t.str_list.end(); it++)
	{
		vector<double> input = Text<double>::unicode(*it);
		input.resize(16);
		Matrix input_ = input;
		cout << input_;
	}
	cout<<"output:"<<n.output(input_);
	cout << "e:" << n.get_e(goal_)<< endl<<endl;
	n.print();

	return 0;
}