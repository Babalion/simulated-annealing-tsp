#ifndef TSP_H
#define TSP_H

#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

#include "util.h"

#define DELETE_PTR(p) if((p) != 0) { delete (p); (p) = 0; }
#define DELETE_PTRA(p) if((p) != 0) { delete[] (p); (p) = 0; }

/**
 * A city is just a point in the 2D plane
 */
typedef std::pair<float, float> City;

/**
 * This is a TSP problem instance. It defines the number of cities and the 
 * pairwise distances. 
 */
class TSPInstance {
public:

    /**
     * Adds a single point to the list of cities
     */
    void addCity(const std::pair<float, float> &city) {
        cities.push_back(city);
    }

    /**
     * Creates a random TSP instance of n nodes
     */
    void createRandom(int n);

    /**
     * Creates a New-York-like TSP instance (which is a simple quadratic 2D grid) with given n rows
     */
    void createNewYork(int n);

    /**
     * Reads a TSPLIB instance from a stream
     */
    void readTSPLIB(std::istream &sin);

    /**
     * Sets up the distance matrix
     */
    void calcDistanceMatrix();

    /**
     * Calculates the length of a tour
     */
    float calcTourLength(const std::vector<int> &tour) const;

    /**
     * Returns the distance between cities i and j
     */
    float dist(int i, int j) const {
        return distances(i, j);
    }

    /**
     * Returns the distance between two cities
     */
    static float dist(const City &c1, const City &c2) {
        const float temp1 = c1.first - c2.first;
        const float temp2 = c1.second - c2.second;
        return std::sqrt((temp1 * temp1 + temp2 * temp2));
    }

    /**
     * Returns the cities
     */
    const std::vector<City> &getCities() const {
        return cities;
    }

private:
    /**
     * The positions of the cities
     */
    std::vector<City> cities;
    /**
     * The distance matrix
     */
    Matrix<float> distances;
};

/**
 * This is the optimizer. It implements the basic simulated annealing algorithm
 * and several neighborhood moves. 
 */
class Optimizer {
public:
    /**
     * The runtime configuration of the algorithm
     */
    class Config {
    public:
        Config() : temp(0), outer(0), inner(0), energy(0), bestEnergy(0), terminated(false) {}

        /**
         * The current temperature
         */
        float temp;
        /**
         * The current outer loop
         */
        int outer;
        /**
         * The current inner loop
         */
        int inner;
        /**
         * The current objective
         */
        float energy;
        /**
         * The currently best energy found
         */
        float bestEnergy;
        /**
         * Current state
         */
        std::vector<int> state;
        /**
         * The best state observed so far
         */
        std::vector<int> bestState;
        /**
         * Whether or not the system has terminated
         */
        bool terminated;
    };

    /**
     * This class implements an observer for the optimizer. The observers can 
     * watch the optimization process
     */
    class Observer {
    public:
        /**
         * This method is called by the optimizer
         */
        virtual void notify(const TSPInstance &instance, const Config &config) = 0;
    };

    /**
     * This class defines a cooling schedule
     */
    class CoolingSchedule {
    public:
        explicit CoolingSchedule(unsigned int steps, float initialTemp) : steps(steps), initialTemp(initialTemp) {}

        /**
         * Calculates the next temperature
         */
        virtual float nextTemp(const Config &config) const = 0;

        /**
         * Returns the initial temperature
         */
        [[maybe_unused]] float getInitialTemp() const {
            return initialTemp;
        }

        /**
        * Returns amount of steps to decrease to endTemp
        */
        unsigned int getSteps() const {
            return steps;
        }

    protected:
        unsigned int steps;
        float initialTemp;
    };

    /**
     * This is a move service class that allows the random sampling of cities
     */
    class MoveService {
    public:
        /**
         * Constructor
         */
        explicit MoveService(int numCities) :
                generator(std::random_device{}()),
                distribution(1, numCities - 1) {}

        /**
         * Returns a random city
         */
        int sample() {
            return distribution(generator);
        }

    private:
        /**
         * The random number generator
         */
        std::mt19937 generator;
        /**
         * The distribution over the cities
         */
        std::uniform_int_distribution<int> distribution;
    };

    /**
     * This class implements a single neighborhood move
     */
    class Move {
    public:
        /**
         * Computes a random neighbor according to some move strategy
         */
        virtual void propose(std::vector<int> &state) const = 0;

        /**
         * Sets the move service
         */
        void setMoveService(MoveService *_service) {
            service = _service;
        }

    protected:
        /**
         * The move service
         */
        MoveService *service{};
    };

    /**
     * Constructor with empty coolingSchedule
     */
    Optimizer() :
            innerLoops(1000),
            notificationCycle(1000),
            coolingSchedule(nullptr),
            outerLoops(100) {}

    /**
     * Constructor with given coolingSchedule
     */
    [[maybe_unused]] explicit Optimizer(CoolingSchedule *coolingSchedule) :
            innerLoops(1000),
            notificationCycle(1000),
            coolingSchedule(coolingSchedule) { outerLoops = static_cast<int>(coolingSchedule->getSteps()); }

    /**
     *Set a new cooling schedule and adjust the step-size
     * @param _coolingSchedule
     */
    void setCoolingSchedule(CoolingSchedule *_coolingSchedule) {
        Optimizer::coolingSchedule = _coolingSchedule;
        outerLoops = static_cast<int>(coolingSchedule->getSteps());
    }

    /**
     *
     * @return outerLoops
     */
    [[maybe_unused]] int getOuterLoops() const {
        return outerLoops;
    }

    /**
     * The number of inner iterations
     */
    int innerLoops;
    /**
     * The notification cycle. Every c iterations, the observers are notified
     */
    int notificationCycle;

    /**
     * Runs the optimizer on a specific problem instance
     */
    void optimize(const TSPInstance &instance, std::vector<int> &result) const;

    /**
     * Adds an observer
     */
    void addObserver(Observer *observer) {
        observers.push_back(observer);
    }

    /**
     * Adds a move
     */
    void addMove(Move *move) {
        moves.push_back(move);
    }

private:
    /**
     * A list of observers
     */
    std::vector<Observer *> observers;
    /**
     * A list of move classes
     */
    std::vector<Move *> moves;
    /**
     * The cooling schedule
     */
    CoolingSchedule *coolingSchedule;
    /**
     * The number of outer iterations
     */
    int outerLoops;
};

/**
 * This is a geometric cooling schedule like eq 4 from
 * http://www.scielo.org.mx/pdf/cys/v21n3/1405-5546-cys-21-03-00493.pdf
 */
class GeometricCoolingSchedule : public Optimizer::CoolingSchedule {
public:
    /**
     * Constructor with alpha, steps calculated
     */
    [[maybe_unused]] GeometricCoolingSchedule(float initialTemp, float endTemp, float alpha) :
            CoolingSchedule(ceil(log(endTemp / initialTemp) / log(alpha)), initialTemp),
            eTemp(endTemp),
            alpha(alpha) {}

    /**
    * Constructor with steps, alpha calculated
    */
    [[maybe_unused]] GeometricCoolingSchedule(float initialTemp, float endTemp, unsigned int steps) :
            CoolingSchedule(steps, initialTemp),
            eTemp(endTemp) {
        alpha = pow(endTemp / initialTemp, 1.0 / steps);
    }

    /**
     * Calculates the next temperature
     */
    float nextTemp(const Optimizer::Config &config) const override {
        return std::max(config.temp * alpha, eTemp);
    }

    /**
     * Returns the decreasing constant alpha
     */
    [[maybe_unused]] float getAlpha() const {
        return alpha;
    }

private:
    /**
     * End temperature
     */
    float eTemp;
    /**
     * alpha -> Decreasing constant
     */
    float alpha;
};

/**
 * This is a logarithmic cooling schedule ,like mentioned in eq 3 from
 * http://www.scielo.org.mx/pdf/cys/v21n3/1405-5546-cys-21-03-00493.pdf
 */
class LogarithmicCoolingSchedule : public Optimizer::CoolingSchedule {
public:
    /**
    * Constructor with steps
    */
    [[maybe_unused]] LogarithmicCoolingSchedule(float initialTemp, float endTemp, unsigned int steps) :
            CoolingSchedule(steps, initialTemp),
            eTemp(endTemp) {}

    /**
     * Calculates the next temperature
     */
    float nextTemp(const Optimizer::Config &config) const override {
        if (config.outer <= 0) {
            return initialTemp;
        }
        static const float a = initialTemp * log(2.0f);
        static const float b = (exp(a / eTemp) - 2.0f) / static_cast<float>(steps);

        float nextTemp = a / log(2 + b * static_cast<float>(config.outer));
        return std::max(nextTemp, eTemp);
    }

private:
    /**
     * End temperature
     */
    float eTemp;
};

/**
 * This move reverses the order of a chain
 */
class ChainReverseMove : public Optimizer::Move {
public:
    /**
     * Computes a random neighbor according to some move strategy
     */
    void propose(std::vector<int> &state) const override {
        // Sample two random cities and reverse the chain
        std::reverse(state.begin() + service->sample(), state.begin() + service->sample());
    }
};

/**
 * This move exchanges two cities in the current path
 */
class SwapCityMove : public Optimizer::Move {
public:
    /**
     * Computes a random neighbor according to some move strategy
     */
    void propose(std::vector<int> &state) const override {
        std::swap(state[service->sample()], state[service->sample()]);
    }
};

/**
 * This move rotates the current path
 */
class RotateCityMove : public Optimizer::Move {
public:
    /**
     * Computes a random neighbor according to some move strategy
     */
    void propose(std::vector<int> &state) const override {
        std::vector<int> c({service->sample(), service->sample(), service->sample()});
        std::sort(c.begin(), c.end());

        std::rotate(state.begin() + c[0],
                    state.begin() + c[1],
                    state.begin() + c[2]);
    }
};

/**
 * This is the runtime GUI that let's you watch what happens during the 
 * optimization procedure
 */
class RuntimeGUI : public Optimizer::Observer {
public:
    /**
     * Constructor
     */
    RuntimeGUI(int rows, int cols) : waitTime(25), gui(rows, cols, CV_8UC3) {
        // Open the window
        cv::namedWindow("GUI", 1);
    }

    /**
     * Destructor
     */
    virtual ~RuntimeGUI() {
        cv::destroyWindow("GUI");
    }

    /**
     * Paint the gui
     */
    void notify(const TSPInstance &instance, const Optimizer::Config &config) override;

    /**
     * The time the GUI pauses after each update. Set to 0 to let
     * it wait for a keypress
     */
    int waitTime;

private:
    /**
     * The GUI matrix
     */
    cv::Mat gui;
};

#endif