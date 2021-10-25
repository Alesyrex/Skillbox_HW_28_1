#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>

class Swimmer
{
    int swimmerID;
    std::string name = "unknown";
    double speed;
    int result;
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
    int getSwimmerID() {return swimmerID;}
    double getSpeed() {return speed;}
    std::string getName() {return name;}
    int getResult() {return result;}
    void setResult() {result++;}
    void setDistance(const double distanceLength)
    {
        distance += speed;
        if (distance > distanceLength) distance = distanceLength;
    }
};

void swim(double distanceLength, Swimmer *swimmer)
{
    while (swimmer->getDistance() < distanceLength)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        swimmer->setDistance(distanceLength);
        swimmer->setResult();
        std::cout << "Swimmer " << swimmer->getName() << " swam " << swimmer->getDistance() << " meters." << std::endl;
    }
}

bool comp (Swimmer *a, Swimmer *b)
{
    return (a->getResult() < b->getResult());
}

int main() {

    const double distanceLength = 30;
    const int countSwimmer = 3;
    std::vector<Swimmer*> swimmers;
    std::vector<std::thread> swimThr;

    for (int i=0;i < countSwimmer;++i)
    {
        swimmers.push_back(new Swimmer(i+1));
    }
    for (int i=0;i < countSwimmer;++i)
    {
        swimThr.push_back(std::thread (swim,distanceLength,std::ref(swimmers[i])));
    }

    for (auto& i : swimThr)
    {
        i.join();
    }

    sort (swimmers.begin(), swimmers.end(), comp);
    for (int i=0;i < countSwimmer;++i)
    {
        std::cout << i+1 << ". " << swimmers[i]->getName() << "\t\tresult: " << swimmers[i]->getResult() << " sec." << std::endl;
    }

    return 0;
}
