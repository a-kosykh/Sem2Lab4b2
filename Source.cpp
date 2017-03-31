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
	cout << "Day salary: " << daySalary_ << endl;
	cout << endl;
}
Worker *pF[2];
/*void threadfoo(unsigned int n) {
	WaitForSingleObject(hEvent, INFINITE);
	while (WaitForSingleObject(hEvent, 0) == WAIT_OBJECT_0) {
		pF[n]->print();
	}
}*/
void threadfoo(unsigned int n) {
	while (1) {
		while (WaitForSingleObject(hEvent, 0) == WAIT_OBJECT_0) {
			m.lock();
			pF[n]->print();
			m.unlock();
			WaitForSingleObject(hEvent, INFINITE);
		}
	}
}


int main() {
	hEvent = CreateEvent(0, 1, 0, L"MyEvent1");
	pF[0] = new Salary("Alex", "Kosykh", 1999, 100000);
	pF[1] = new Salary("Name", "Surname", 1998, 100022);
	thread th[2];

	th[0] = thread(threadfoo, 0);
	th[1] = thread(threadfoo, 1);
	
	th[0].join();
	th[1].join();
	CloseHandle(hEvent);
	system("pause");
	return 0;
}