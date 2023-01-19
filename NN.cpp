#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<random>
#include<time.h>
#include<iostream>
#include<ostream>
#include<iomanip>
#include<vector>
#include<thread>
#include<mutex>
#include<condition_variable>

double sigmoid(double x) {
	double out=1/(1+exp(-x));
	if (out < 1.0e-8) {
		std::cout << "梯度消失置0\Matrix_data";
		return 0;
	}
	return out;
}

double ss_back(double x) {
	return (1 - x) * x;
}

std::mutex m;
class Matrix
{
public:
	Matrix(){
		this->r = 0;
		this->c = 0;
		this->n = nullptr;
		//std::cout << this << "0  default\n";
	}
	explicit Matrix(int r, int c) {
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
	Matrix(const Matrix& old):r(old.r),c(old.c),n(new double* [old.r]) {
		for (int i = 0; i < this->r; i++)
		{
			this->n[i] = new double[this->c];
			memcpy(this->n[i], old.n[i], old.c * sizeof(double));
		}
		//std::cout << this <<"copy\n";
	}//实质const net&=net&&底层，但net&&引用的地址在代码中没有对应变量(临时)，但const net是有的
	Matrix(Matrix&& old) :r(old.r), c(old.c), n(old.n) {
		old.n = nullptr;
		//std::cout<<this << "move\n";
	}
	~Matrix() {
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
	Matrix& operator=(Matrix&& old) {
		this->c = old.c;
		if (this->n != nullptr) {
			for (int i = 0; i < r; i++)
			{
					delete this->n[i];
					this->n[i] = nullptr;
			}
			delete n;
			this->n = nullptr;
		}
		this->n = old.n;
		this->r = old.r;
		old.n = nullptr;
		return *this;
	}
	Matrix& operator=(const Matrix& old) {
		if (this->n != nullptr) {
			for (int i = 0; i < this->r; i++)
			{
				delete this->n[i];
				this->n[i] = nullptr;
			}
			delete n;
			this->n = nullptr;
		}
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
	Matrix operator*(Matrix& mutipul) {

		Matrix putn(this->r,mutipul.c);
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
	Matrix operator-(Matrix& sub) {
		Matrix putn(sub.r, sub.c);
		for (int i = 0; i < sub.r; i++)
		{
			for (int j = 0; j < sub.c; j++)
			{
				putn.n[i][j] = this->n[i][j]-sub.n[i][j];
			}
		}
		return putn;
	}
	Matrix operator+(Matrix& add) {
		Matrix putn(add.r, add.c);
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
	friend std::ostream& operator<<(std::ostream& out, const Matrix& t) {
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
	
	Matrix w;
	Matrix input;
	Matrix output;
	Matrix e;
	Matrix ss;
	double v=0.9;
	layer(int r, int c):w(r,c), input(c,1),output(r,1),e(r,1),ss(r,1){

	}
	Matrix oput(Matrix& input) {
		this->input = input;
		this->output = w * input;
		this->output.fun_handle(sigmoid);
		this->ss = this->output;
		ss.fun_handle(ss_back);
		return this->output;
	}
	void updata() {
		
		std::unique_lock<std::mutex> graud(m);
		for (int i = 0; i < ss.r; i++)
		{
			for (int j = 0; j < ss.c; j++)
			{
				ss.n[i][j] = ss.n[i][j] * e.n[i][j];
			}
		}
		input.rotate();
		Matrix dw = ss * input;
		w = w - dw;

	}
	double getE(Matrix& goal) {
		double sum=0;
		for (int i = 0; i < e.r; i++)
		{
			for (int j = 0; j < e.c; j++)
			{
				double t = (this->output.n[i][j] - goal.n[i][j]);
				this->e.n[i][j] = t;
				sum += fabs(e.n[i][j]);

			}
		}
		return sum;
	
	}
	Matrix E_back(void) {
		Matrix bw=this->w;
		bw.rotate();
		return bw * e;

	}
	void setE(Matrix& E) {
		this->e = E;
	}
	~layer() {

	}
};

class network
{
public:
	int lay;
	std::vector<layer> netw;
	network(int*num,int s) {
		for (int i = 0; i < s-1; i++) {
			netw.emplace_back(num[i + 1], num[i]);
		}
	}
	Matrix output(Matrix& in) {
		Matrix t;
		t = in;
		for (int i = 0; i < netw.size(); i++)
		{
			t = netw[i].oput(t);
		}
		return t;
	}
	Matrix train(Matrix& goal) {
		Matrix t;
		netw[netw.size() - 1].getE(goal);
		for (int i = netw.size() - 1; i >=1; i--)
		{
			t = netw[i].E_back();
			netw[i - 1].setE(t);
		}
		for (int i = 0; i < netw.size(); i++)
		{
			//netw[i].updata();
			
			std::thread t(&layer::updata, &netw[i]);
			t.join();
			
		}
		return netw[netw.size() - 1].e;
	}
};


int main(void) {

	using namespace std;
	layer a(2, 3);
	layer b(3, 2);
	layer c(3, 3);
	Matrix g(3, 1);
	Matrix in(3, 1);
	Matrix t;
	int ne[6] = { 3,3,3,3,3,3 };
	network v(ne, 6);
	for (int i = 0; i < 600; i++)
	{
		t = in;
		v.output(in);
		cout<<v.train(g)<<endl;

		//
		////cout << t  << endl;
		//t = a.oput(t);
		//t = b.oput(t);
		//t = c.oput(t);
		//c.getE(g);
		//t = c.E_back();
		//b.setE(t);
		//t = b.E_back();
		//a.setE(t);
		//c.updata();
		//b.updata();
		//a.updata();
		////cout << a.w << endl;
		////cout << b.w << endl;
		////cout << c.w << endl;
		//cout << c.e << endl;
		////cout << "output:" << endl;
		////cout << c.output << endl;

	}

	
	return 0;
}