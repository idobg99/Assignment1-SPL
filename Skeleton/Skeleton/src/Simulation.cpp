#include "Simulation.h"
#include <iostream>
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
            Simulation::addSettlement(Settlement (inf[1],SettlementType(static_cast<SettlementType>((stoi(inf[2])))) ));
        }
        else if (inf[0]=="facility"){
            Simulation::addFacility(FacilityType(inf[1],static_cast<FacilityCategory>((stoi(inf[2]))),stoi(inf[3]),stoi(inf[4]), stoi(inf[5]), stoi(inf[6])));
        }
        else if (inf[0]=="plan"){
            if (inf[2]== "eco"){
                Simulation::addPlan(Simulation::getSettlement(inf[1]),new NaiveSelection());
            }    
            //to complete- 3 other policies        
        }        
    }
    configFile.close();
};

void Simulation::start(){
    Simulation::open();

    while (isRunning) {
        string userCommand;
        cin >> userCommand;
        
        //to complete - executing the commands accordding to the user

        // Exit condition
        if (userCommand == "close") {           
            Simulation::close();
            break;
            }    
}};

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy){
    Plan newPlan(planCounter, settlement, selectionPolicy,  facilitiesOptions);
    plans.push_back(newPlan);
    planCounter++;
};

void Simulation::addAction(BaseAction *action){
    actionsLog.push_back(action);
};

bool Simulation::addSettlement(Settlement settlement){
    if (isSettlementExists(settlement.getName())) {return false;}
    settlements.push_back(settlement);
    return true;
};

bool Simulation::addFacility(FacilityType facility){
    for (FacilityType f: facilitiesOptions){
        if (f.getName()==facility.getName()) {return false;}
    }
    facilitiesOptions.push_back(facility);
    return true;
};

bool Simulation::isSettlementExists(const string &settlementName){
    for (Settlement s:settlements){
        if (s.getName()==settlementName) {return true;}
    }
    return false;
};
        
Settlement &Simulation::getSettlement(const string &settlementName){
    for (Settlement s:settlements){
        if (s.getName()==settlementName) {return s;}
    }    
};

Plan &Simulation:: getPlan(const int planID){
    return plans[planID];
};

void Simulation::step(){};// to complete
void Simulation::close(){
    isRunning = false;
};
void Simulation::open(){
    isRunning = true;
    cout << "The simulation has started"<< endl;
};


