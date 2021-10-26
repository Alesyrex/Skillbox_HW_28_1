#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>
#include <mutex>

std::map<int,std::string> final_result;
std::mutex mtx;

class Swimmer
{
    int swimmerID;
    std::string name = "unknown";
    int result;
    double speed;
    double distance;

public:
    Swimmer(int inSwimmerID = 1)
           : swimmerID(inSwimmerID), speed(0), distance (0), result(0)
    {
        std::cout << "Enter swimmer-" << inSwimmerID << " name:";
        std::cin >> name;
        std::cout << "Enter swimmer-" << inSwimmerID << " speed (m/s):";
        std::cin >> speed;
        if (speed > 5)
        {
            std::cout << "Selected maximum speed of 5 m/s." << std::endl;
            speed = 5;
        }
        else if (speed <= 0)
        {
            std::cout << "Selected minimum speed of 0.1 m/s." << std::endl;
            speed = 0.1;
        }
    }
    double getDistance() {return distance;}
    std::string getName() {return name;}
    int getResult() {return result;}
    void setResult() {result++;}
    void setDistance(const double distanceLength)
    {
        distance += speed;
        if (distance > distanceLength) distance = distanceLength;
    }
};

void swim(double distanceLength, Swimmer &swimmer)
{
    while (swimmer.getDistance() < distanceLength)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        swimmer.setDistance(distanceLength);
        swimmer.setResult();
        mtx.lock();
        std::cout << "Swimmer " << swimmer.getName() << " swam " << swimmer.getDistance() << " meters." << std::endl;
        mtx.unlock();
    }
    mtx.lock();
    final_result.insert(std::pair<int,std::string>(swimmer.getResult(),swimmer.getName()));
    mtx.unlock();
}

int main() {

    const double distanceLength = 30;
    const int countSwimmer = 4;
    std::vector<Swimmer> swimmers;
    std::vector<std::thread> swimThr;

    for (int i=0;i < countSwimmer;++i)
    {
        swimmers.push_back(Swimmer(i+1));
    }
    for (int i=0;i < countSwimmer;++i)
    {
        swimThr.push_back(std::thread (swim,distanceLength,std::ref(swimmers[i])));
    }

    for (auto& i : swimThr)
    {
        i.join();
    }

    for (auto it = final_result.begin();it != final_result.end();++it)
    {
        std::cout << it->second << "\tresult: " << it->first << " sec." << std::endl;
    }

    return 0;
}
