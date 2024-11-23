#include "Simulation.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <Auxiliary.h>
#include <Settlement.h>
#include  <Facility.h>
#include <Plan.h>
#include <SelectionPolicy.h>
using namespace std;

Simulation::Simulation(const string &configFilePath):isRunning(false), planCounter(0), 
actionsLog(), plans(), settlements(), facilitiesOptions() {
    ifstream configFile(configFilePath);
    string line;
    while (getline(configFile, line)){       
        vector<string> inf(Auxiliary::parseArguments(line));
        if (inf[0]=="settlement"){
            addSettlement(Settlement (inf[1],SettlementType(static_cast<SettlementType>((stoi(inf[2])))) ));
        }
        else if (inf[0]=="facility"){
            addFacility(FacilityType(inf[1],static_cast<FacilityCategory>((stoi(inf[2]))),stoi(inf[3]),stoi(inf[4]), stoi(inf[5]), stoi(inf[6])));
        }
        //else if (inf[0]=="plan"){
          //  NaiveSelection *s  = *NaiveSelection();
            //addPlan(getSettlement(inf[1]),s));  

       // }
        configFile.close();
}};

void start(){
    isRunning = true;

    while (isRunning) {
        string userCommand;
        std::cin >> userCommand;
        
        //Adding the commands accordding to the user

        // Exit condition
        if (userCommand == "close") {
            isRunning = false;
            close();
            break;
            }    
}};

void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy){
    Plan newPlan(planCounter, settlement, selectionPolicy,  facilitiesOptions);
    plans.push_back(newPlan);
    planCounter++;
};

void addAction(BaseAction *action){
    actionsLog.push_back(action);
};

bool addSettlement(Settlement settlement){
    if (isSettlementExists(settlement.getName())) {return false;}
    settlements.push_back(settlement);
    return true;
};

bool addFacility(FacilityType facility){
    for (FacilityType f: facilitiesOptions){
        if (f.getName()==facility.getName()) {return false;}
    }
    facilitiesOptions.push_back(facility);
    return true;
};

bool isSettlementExists(const string &settlementName){
    for (Settlement s:settlements){
        if (s.getName()==settlementName) {return true;}
    }
    return false;
};
        
Settlement &getSettlement(const string &settlementName){
    for (Settlement s:settlements){
        if (s.getName()==settlementName) {return s;}
    }    
};

Plan &getPlan(const int planID){
    return plans[planID];
};

void step();
void close();
void open();


bool isRunning;
int planCounter; //For assigning unique plan IDs
vector<BaseAction*> actionsLog;
vector<Plan> plans;
vector<Settlement> settlements;       
vector<FacilityType> facilitiesOptions;
    