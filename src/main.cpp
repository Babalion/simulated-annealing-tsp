#include "tsp.h"
#include <map>
#include <string>


int main(int argc, const char **argv) {
#ifdef VERBOSE_MODE
    std::cout << "argc=" << argc << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << "argv[i]=" << argv[i] << std::endl;
    }
#endif
    // Set up a random problem instance
    TSPInstance instance;
    if (argc == 2 && std::string(argv[1]) == "--help") {
        std::cout << "tsp [OPTIONS] [ARG]" << std::endl << std::endl;
        std::cout << "Creates and tries to solve a Travelling-Salesman-Problem with simulated annealing algorithm."
                  << std::endl
                  << std::endl;

        std::cout << "OPTIONS" << std::endl << std::endl;
        std::cout << std::left << std::setw(25) << "\t--help" << "Show this help" << std::endl;
        std::cout << std::left << std::setw(25) << "\t--random-map [N]"
                  << "Creates N random distributed cities on the field. N is the number of cities" << std::endl;
        std::cout << std::left << std::setw(25) << "\t--new-york [N]"
                  << "Creates a quadratic 2D-Grid with N nodes. The lowest energy is simply 1000*N" << std::endl;
        std::cout << std::left << std::setw(25) << "\t--tsp-file [PATH]" << "Initialize a given tsp-file" << std::endl;
        return 0;
    }
    if (argc == 3 && std::string(argv[1]) == "--tsp-file") {
        std::ifstream stream;
        stream.open(argv[2]);
        if (!stream.is_open()) {
            std::cout << "Cannot open data file.";
            return 1;
        }
        instance.readTSPLIB(stream);
        stream.close();
    }
    if (argc == 3 && std::string(argv[1]) == "--random-map") {
        instance.createRandom(std::stoi(argv[2]));
    }
    if (argc == 3 && std::string(argv[1]) == "--new-york") {
        instance.createNewYork(std::stoi(argv[2]));
    }
    if (argc == 1) {
        instance.createRandom(50);
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
    GeometricCoolingSchedule schedule(100, 1e-3, 0.99);
    optimizer.coolingSchedule = &schedule;

    // Optimizer loop counts
    optimizer.outerLoops = 1000;
    optimizer.innerLoops = 2000;
    // Update the GUI every 2000 iterations
    optimizer.notificationCycle = 1000;

    // Run the program
    std::vector<int> result;
    optimizer.optimize(instance, result);
    schedule=GeometricCoolingSchedule(20,1e-3,0.99);
    optimizer.optimize(instance, result);
    optimizer.optimize(instance, result);
    optimizer.optimize(instance, result);

    return 0;
}