#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<random>
#include<time.h>
#include<iostream>
#include<ostream>
#include<iomanip>
#include<vector>

double sigmoid(double x) {
	double out=1/(1+exp(-x));
	if (out < 1.0e-8) {
		std::cout << "梯度消失置0\n";
		return 0;
	}
	return out;
}

double ss_back(double x) {
	return (1 - x) * x;
}


class net
{
public:
	net(){
		this->r = 0;
		this->c = 0;
		this->n = nullptr;
		//std::cout << this << "0  default\n";
	}
	explicit net(int r, int c) {
 		this->r = r;
		this->c = c;
		n = new double* [r];
		for (int i = 0; i < r; i++)
		{
			n[i] = new double[c];
		}
		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < c; j++)
			{
				n[i][j] = ((double)rand()) / RAND_MAX;
			}
		}
		//std::cout << this <<"num default\n";
	};
	net(const net& old):r(old.r),c(old.c),n(new double* [old.r]) {
		for (int i = 0; i < this->r; i++)
		{
			this->n[i] = new double[this->c];
			memcpy(this->n[i], old.n[i], old.c * sizeof(double));
		}
		//std::cout << this <<"copy\n";
	}//实质const net&=net&&底层，但net&&引用的地址在代码中没有对应变量(临时)，但const net是有的
	net(net&& old) :r(old.r), c(old.c), n(old.n) {
		old.n = nullptr;
		//std::cout<<this << "move\n";
	}
	~net() {
		//std::cout << "delete" << this << std::endl;
		if (n != nullptr) {
			for (int i = 0; i < r; i++)
			{
				if (n[i] != nullptr) {
					delete n[i];
					n[i] = nullptr;
				}
			}
			delete n;
			n = nullptr;
		}

	}
	void print() {
		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < c; j++)
			{
				printf("%3.4lf ", n[i][j]);
			}
			printf("\n");
		}
	}
	net& operator=(net&& old) {
		this->c = old.c;
		this->n = old.n;
		this->r = old.r;
		old.n = nullptr;
		return *this;
	}
	net& operator=(const net& old) {
		this->c = old.c;
		this->r = old.r;
		this->n = new double* [this->r];
		for (int i = 0; i < this->r; i++)
		{
			n[i] = new double[this->c];
			memcpy(n[i], old.n[i], old.c*sizeof(double));
		}
		return *this;
	}
	net operator*(net& mutipul) {

		net putn(this->r,mutipul.c);
		for (int i = 0; i < this->r; i++)
		{
			for (int j = 0; j < mutipul.c; j++)
			{
				double in=0;
				for (int ti = 0; ti < this->c; ti++)
				{
					in += this->n[i][ti] * mutipul.n[ti][j];
				}
				putn.n[i][j] = in;
			}
		}
		return putn;
	}
	net operator-(net& sub) {
		net putn(sub.r, sub.c);
		for (int i = 0; i < sub.r; i++)
		{
			for (int j = 0; j < sub.c; j++)
			{
				putn.n[i][j] = this->n[i][j]-sub.n[i][j];
			}
		}
		return putn;
	}
	net operator+(net& add) {
		net putn(add.r, add.c);
		for (int i = 0; i < add.r; i++)
		{
			for (int j = 0; j < add.c; j++)
			{
				putn.n[i][j] = this->n[i][j] + add.n[i][j];
			}
		}
		return putn;
	}
	void fun_handle(double(*fun)(double)){
		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < c; j++)
			{
				n[i][j] = fun(n[i][j]);
			}
		}
	}
	void rotate() {
		double** nn;
		nn = new double* [c];
		for (int i = 0; i < c; i++)
		{
			nn[i] = new double[r];
		}
		for (int i = 0; i <this->r; i++)
		{
			for (int j = 0; j < this->c; j++)
			{
				nn[j][i] = this->n[i][j];
			}
		}
		for (int i = 0; i < r; i++)
		{
				delete n[i];
		}
		delete n;
		n = nn;
		int t = c;
		c = r; r = t;
	}
	friend std::ostream& operator<<(std::ostream& out, const net& t) {
		//out.precision(4);
		for (int i = 0; i < t.r; i++)
		{
			for (int j = 0; j < t.c; j++)
			{
				out<<std::fixed<< std::setprecision(4) << t.n[i][j] << " ";
			}
			out << std::endl;
		}
		//out.unsetf(out.flags());
		return out;
	}//友元函数并不是成员函数,不含this指针,operator<<(std::ostream& out, const net& t)非a.operator<<(std::ostream& out, const net& t)
	double** n;
	int r;
	int c;
};
//std::ostream& operator<<(std::ostream& out, const net& t) {
//	for (int i = 0; i < t.r; i++)
//	{
//		for (int j = 0; j < t.c; j++)
//		{
//			out << t.n[i][j] << " ";
//		}
//		out << std::endl;
//	}
//	return out;
//}

class layer
{
public:
	net w;
	net input;
	net output;
	net e;
	net ss;
	double v=0.9;
	layer(int r, int c):w(r,c), input(c,1),output(r,1),e(r,1),ss(r,1){

	}
	net oput(net& input) {
		this->input = input;
		this->output = w * input;
		this->output.fun_handle(sigmoid);
		this->ss = this->output;
		ss.fun_handle(ss_back);
		return this->output;
	}
	void updata() {
		for (int i = 0; i < ss.r; i++)
		{
			for (int j = 0; j < ss.c; j++)
			{
				ss.n[i][j] = ss.n[i][j] * e.n[i][j];
			}
		}
		input.rotate();
		net dw = ss * input;
		w = w - dw;

	}
	double getE(net& goal) {
		double sum=0;
		for (int i = 0; i < e.r; i++)
		{
			for (int j = 0; j < e.c; j++)
			{
				double t = (this->output.n[i][j] - goal.n[i][j]);
				this->e.n[i][j] = 0.5*t*t;
				if (t < 0)
					this->e.n[i][j] = -this->e.n[i][j];
				sum += fabs(e.n[i][j]);

			}
		}
		return sum;
	
	}
	~layer() {

	}
};


int main(void) {

	using namespace std;
	layer a(3, 3);
	net g(3, 1);
	g.n[0][0] = 0.8;
	g.n[1][0] = 0.2;
	g.n[2][0] = 0.5;
	net in(3, 1);
	in.n[0][0] = 0.6;
	in.n[1][0] = 0.3;
	in.n[2][0] = 0.1;

	for (int i = 0; i < 500; i++) {
		a.oput(in);
		a.getE(g);
		a.updata();
		cout << a.e<<endl;
	}

	return 0;
}