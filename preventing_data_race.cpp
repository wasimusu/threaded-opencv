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

void block_lock(string name){
    //The contigous block below is locked. Rest is not.
    unique_lock<mutex> locker(mu);
    cout<<"\nName : "<< name<<endl;
    locker.unlock();

    // I can do anything here. It's not locked and can enjoy parallel execution.
    int N = 10;
    for(int i = 0; i < N; i++){}
    cout<<name<<"\t"<<N<<endl;
}

void unsecure_print(string name){
    mu.lock();
    cout <<"Name : "<< name <<endl;
    mu.unlock();
    
    // In the event of an exception, mutex will remain locked.
    // If you forget to unlock the mutex, it will remain locked as well.
}


void doesnot_work_print(string name){
    cout <<"Name : "<< name <<endl;
    // It could print anything in any sequence. One thread might interrupt other and everything could 
    // end up in the same line.
    
    //Example1
    //Name : Wasim
    //Name : Akram
    //Name : Khan
    
    //Example2
    //Name : WasimName:Khan
    //
    //Name : Akram
    
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
