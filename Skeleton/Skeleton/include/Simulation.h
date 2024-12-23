#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include <fstream>
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);
        void start();
        void addPlan(const Settlement *settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        bool isPlanExist(const int planId);
        Settlement *getSettlement(const string &settlementName);
        Plan &getPlan(const int planID);       
        void step();
        void close();
        void open();       
        ~Simulation();
        Simulation(Simulation &other);

        //Adi added:
        SelectionPolicy* stringToPolicy (const string &policy);
        const string getAction(int actionNumber) const;
        const int getNumOfActions() const;
        const int getNumOfPlans() const;
        Simulation(Simulation &&other);
        Simulation& operator=(const Simulation&& other);


        //Ido added:
        Simulation& operator=(const Simulation& other);

    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;
};