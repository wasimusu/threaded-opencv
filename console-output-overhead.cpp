//
// Created by wasimkhan on 6/3/19.
//

#include <iostream>
#include <chrono>

using namespace std;

int main(){

    // Outputting to console at every step
    auto start = std::chrono::system_clock::now();
    int count = 0;
    for(int i=0; i<2000; i++) {
        cout<<"The longer the string the greater the overhead "<<count<<endl;
        count++;
    }
    auto stop = std::chrono::system_clock::now();
    std::cout<<"Overhead took " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " ms\n";


    // Outputting to console at every k steps
    auto start1 = std::chrono::system_clock::now();
    count = 0;
    for(int i=0; i<2000; i++) {
        count++;

        if(count % 10 == 0){cout<<"Reached "<<count<<endl;}
    }
    auto stop1 = std::chrono::system_clock::now();
    std::cout<<"If took " << std::chrono::duration_cast<std::chrono::milliseconds>(stop1 - start1).count() << " ms\n";

    return 0;
}
