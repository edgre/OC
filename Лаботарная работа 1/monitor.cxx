#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

using namespace std;

std::mutex m;
std::condition_variable cv;
bool ready = false;
vector<int> a;

void provide ()
{
	std::unique_lock<std::mutex> lk(m);
	if (ready)
	{
		m.unlock();
		return;
	}
	ready = true;
	a.push_back(1); a.push_back(2);
	cout<<"provided\n";
	cv.notify_one();
}

void consume ()
{
	std::unique_lock<std::mutex> lk(m);
	while (!ready)
	{   
		cv.wait(lk, [] {return ready;});
		cout<<"awoke\n";
	}
	ready = false;
	if (a.empty()) cout<<"empty";
	else for (int i=0; i<a.size(); i++) cout<<a[i]<<' ';
	cout<<endl;
	cout<<"consumed\n";
}

int main()
{
	std::thread provider (provide);
	std::thread consumer(consume);
	
	consumer.join();
	provider.join();
	
	
}
