#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <Windows.h>
using namespace std;
mutex m;

HANDLE hEvent;

class Worker {
protected:
	string name_;
	string sur_;
	unsigned int year_;
public:
	Worker(string n, string s, unsigned int year);
	virtual void print() = 0;
	~Worker() {}
};

Worker::Worker(string n, string s, unsigned int year)

{
	this->name_ = n;
	this->sur_ = s;
	this->year_ = year;
}

class Salary : public Worker {
protected:
	double monthSalary_;
	double daySalary_;
public:
	Salary(string n,
		string s,
		unsigned int year,
		double monthSalary_);
	void print();
};

Salary::Salary(string n, string s, unsigned int year, double monthSalary_)
	:
	Worker(n, s, year)
{
	this->monthSalary_ = monthSalary_;
	daySalary_ = monthSalary_ / 30;
}

void Salary::print()
{
	cout << name_ << endl;
	cout << sur_ << endl;
	cout << year_ << endl;
	cout << monthSalary_ << endl;
	cout << daySalary_ << endl;
	cout << endl;
}
Worker *pF[2];
void threadfoo(unsigned int n) {
	m.lock();
	WaitForSingleObject(hEvent, INFINITE);
	pF[n]->print();
	m.unlock();
	
}



int main() {
	hEvent = CreateEvent(0, 1, 0, L"MyEvent1");
	pF[0] = new Salary("Alex", "Kosykh", 1999, 100000);
	pF[1] = new Salary("domi", "asdas", 1998, 100022);
	thread th[2];

	for (unsigned int i = 0; i < 2; ++i) {
		th[i] = thread(threadfoo, i);
		th[i].join();
	}

	system("pause");
	return 0;
}