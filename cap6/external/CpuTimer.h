// AUTHOR: Victor Alessandrini, 2015
// VATH libray,in book "Shared Memory Application
// Programming"
// *********************************************
// CpuTimer.h
//
// Measurine execution times. Reports wall, user
// (total computation time of all threads) and 
// system times in Pthreads. In Windows and C++11, 
// only wall times are reported
// ---------------------------------------

#ifndef CPU_TIMER_H
#define CPU_TIMER_H

#include <iostream>
#include <chrono>

class CpuTimer
{

public:
    CpuTimer() = default;

    void Start()
    {
        _start = std::chrono::steady_clock::now();
    }
    
    void Stop()
    {
        _stop = std::chrono::steady_clock::now();
    }
    
    void Report()
    {
        std::chrono::steady_clock::duration d_start = _start.time_since_epoch();
        std::chrono::steady_clock::duration d_stop = _stop.time_since_epoch();
        std::chrono::steady_clock::duration d = d_stop - d_start;
        std::chrono::duration<double, std::ratio<1, 1>> dsecs(d);
        std::cout << "Wall time is " << dsecs.count() << " seconds" << std::endl;
    }

private:
    std::chrono::steady_clock::time_point _start;
    std::chrono::steady_clock::time_point _stop;

};

#endif
