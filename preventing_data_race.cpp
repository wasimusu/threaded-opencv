// Example program
#include <iostream>
#include <string>
#include <cstdlib>
#include <thread>
#include <mutex>

using namespace std;
mutex mu;

void print(string name){
    lock_guard<mutex> guard(mu);
    cout <<"Name : "<< name <<endl;
}

void unsecure_print(string name){
    mu.lock();
    cout <<"Name : "<< name <<endl;
    mu.unlock();
    
    // In the event of an exception, mutex will remain locked.
    // If you forget to unlock the mutex, it will remain locked as well.
}

int main()
{
    string name1 = "Wasim";
    string name2 = "Akram";
    string name3 = "Khan";
    
    thread t1(print, name1);
    thread t2(print, name2);
    thread t3(print, name3);
    
    t1.join();
    t2.join();
    t3.join();
    cout<<"Finished"<<endl;

  return 0;
}
