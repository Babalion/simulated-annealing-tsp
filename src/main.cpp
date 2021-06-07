#include "tsp.h"
#include <map>
#include <string>

int main(int argc, const char **argv) {
    // Create an empty instance
    TSPInstance instance;

    std::cout << "Select a distribution of cities:" << std::endl;
    std::cout << "Random (default=0)\nspecific tsp (1)\nnew-york (2)\n";
    int inp = 0;
    std::cin >> inp;
    switch (inp) {
        case 1: {
            std::cout << "file-path:" << std::endl;
            std::ifstream stream;
            std::string path;
            std::cin >> path;
            stream.open(path.c_str());
            if (!stream.is_open()) {
                std::cout << "Cannot open data file.";
                return 1;
            }
            instance.readTSPLIB(stream);
            stream.close();
            break;
        }
            /*case 2:
                instance=tsp_newYork();
                break;*/
        default: {
            std::cout << "How many cities?\n";
            int cit = 0;
            std::cin >> cit;
            instance.createRandom(cit);
        }
    }


    instance.calcDistanceMatrix();

    // Set up the optimizer 
    Optimizer optimizer;

    // Register the moves
    ChainReverseMove move1;
    SwapCityMove move2;
    RotateCityMove move3;
    optimizer.addMove(&move1);
    optimizer.addMove(&move2);
    optimizer.addMove(&move3);

    // Register the GUI
    // You can specify the dimensions of the window
    RuntimeGUI gui(1024, 1024);
    optimizer.addObserver(&gui);

    // The time the GUI stops after each iterations. Set to 0 to wait for a 
    // keypress
    gui.waitTime = 7;

    // Choose a cooling schedule
    GeometricCoolingSchedule schedule(500, 1e-2, 0.99);
    optimizer.coolingSchedule = &schedule;

    // Optimizer loop counts
    optimizer.outerLoops = 500;
    optimizer.innerLoops = 5000;
    // Update the GUI every 2000 iterations
    optimizer.notificationCycle = 1000;

    // Run the program
    std::vector<int> result;
    optimizer.optimize(instance, result);

    return 0;
}