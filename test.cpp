#include<iostream>
#include<Windows.h>
//#define OPEN_THREAD//开启线程处理，有问题且提升不大
#include"Network.h"

using namespace std;
int main() {
	Text a("D:/acpp/NN/Debug/read.txt");
	Network n(2,10,10);
	Matrix g;
	vector<double> go = {0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5 };
	g = go;
	g.rotate();
	Matrix ve;
	list<string>::iterator it = a.str_list.begin();
	for (int i = 0; i < 2000; i++) {
		it = a.str_list.begin();
		while (it != a.str_list.end()) {
			vector<double>v = a.unicode(*it);
			int siz = v.size();
			if (siz < 10)
				for (size_t i = 0; i < 10 - siz; i++)
				{
					v.emplace_back(0.0001);
				}
			if (siz > 10)
				for (size_t i = 0; i < siz - 10; i++)
				{
					v.pop_back();
				}
			ve = v;


			ve.rotate();
			n.output(ve);
			n.train(g);
			it++;
		}
	}
	cout << ve << endl;
	cout<< n.output(ve);
	cout <<"error:" << n.get_e(g) << endl;
	cout<<"end\n";
	Text b("D:/acpp/NN/Debug/read1.txt");
	it = b.str_list.begin();
	vector<double > ci=Text<>::unicode(*it);
	int siz = ci.size();
	if (siz < 10)
		for (size_t i = 0; i < 10 - siz; i++)
		{
			ci.emplace_back(0.0001);
		}
	if (siz > 10)
		for (size_t i = 0; i < siz - 10; i++)
		{
			ci.pop_back();
		}
	Matrix is = ci;
	is.rotate();
	cout << is<<endl;
	cout<<n.output(is);
	cout <<"error:" << n.get_e(g) << endl;
	return 0;
}