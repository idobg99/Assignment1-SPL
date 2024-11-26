#include <Simulation.h>
#include <iostream>
#include <fstream>
#include <Auxiliary.h>
#include <Settlement.h>
#include  <Facility.h>
#include <Plan.h>
#include <SelectionPolicy.h>
#include <Action.h>
using namespace std;

Simulation::Simulation(const string &configFilePath):isRunning(false), planCounter(0), 
    actionsLog(), plans(), settlements(), facilitiesOptions() {
    ifstream configFile(configFilePath);
    string line;

    while (getline(configFile, line)){       
        vector<string> inf(Auxiliary::parseArguments(line));
        if (inf[0]=="settlement"){            
            addSettlement(new Settlement(inf[1],SettlementType(static_cast<SettlementType>((stoi(inf[2])))) ));
        }
        else if (inf[0]=="facility"){
            addFacility(FacilityType(inf[1],static_cast<FacilityCategory>((stoi(inf[2]))),stoi(inf[3]),stoi(inf[4]), stoi(inf[5]), stoi(inf[6])));
        }
        else if (inf[0]=="plan"){
            SelectionPolicy* policy = stringToPolicy(inf[2]);
            if (policy!=nullptr) {           
                addPlan(getSettlement(inf[1]),policy);
            }                  
        }        
    }
    configFile.close();
};

void Simulation::start(){
    open();

    while (isRunning) {
        string userCommand;
        cin >> userCommand;
        vector<string> inf(Auxiliary::parseArguments(userCommand));

        if (inf[0]=="step"){   //need to complete in action class.            
            addAction(new SimulateStep(stoi(inf[1])));
        }
        else if (inf[0]=="plan"){           
            addAction(new AddPlan (inf[1], inf[2]));                   
        }
        else if (inf[0]=="settlement"){
            SettlementType type = SettlementType(static_cast<SettlementType>((stoi(inf[2])))); 
            if (isSettlementExists(inf[1])){
                addAction(new AddSettlement(inf[1],type));
            }
            else{
                addSettlement(new Settlement(inf[1],type));
                addAction(new AddSettlement(inf[1],type)); 
            }                                            
        }
        else if (inf[0]=="facility"){           
            addAction(new AddFacility((inf[1]),static_cast<FacilityCategory>((stoi(inf[2]))),stoi(inf[3]),stoi(inf[4]),stoi(inf[5]),stoi(inf[6]))); 
        }

        else if (inf[0]=="planStatus"){           
            addAction(new PrintPlanStatus(stoi(inf[1]))); 
        }
        else if (inf[0]=="log"){           
            addAction(new PrintActionsLog()); 
        }

        
        //to complete - executing the commands accordding to the user

        // Exit condition
        if (userCommand == "close") {           
            close();
            break;
            }    
}};

void Simulation::addPlan(const Settlement *settlement, SelectionPolicy *selectionPolicy){
    Plan newPlan(planCounter, *settlement, selectionPolicy,  facilitiesOptions);
    plans.push_back(newPlan);
    planCounter++;
};

void Simulation::addAction(BaseAction *action){
    actionsLog.push_back(action);
    (*action).act(*this);    
};

bool Simulation::addSettlement(Settlement *settlement){
    if (Simulation::isSettlementExists((*settlement).getName())) {return false;}
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
    for (Settlement *s:settlements){
        if ((*s).getName()==settlementName) {return true;}
    }
    return false;
};
        
Settlement* Simulation::getSettlement(const string &settlementName){
    for (Settlement* s:settlements){
        if ((*s).getName()==settlementName) {return s;}
    }    
};
bool Simulation::isPlanExist(const int planId){
    if (planId>plans.size()){ return false;}
    return true;
};

Plan &Simulation::getPlan(const int planID){
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

SelectionPolicy* Simulation::stringToPolicy (const string policy){
    if (policy=="nve"){
        return new NaiveSelection();
    }
    else if (policy=="bal"){
        return new BalancedSelection(0,0,0);  //need to check if it is the right input
    }
    else if (policy=="eco"){
        return new EconomySelection();
    }
    else if (policy=="env"){
        return new SustainabilitySelection();
    }
    else return;
};

const string Simulation::getAction(int actionNumber) const{
    BaseAction &actionToString = *actionsLog[actionNumber] ;
    return (actionToString).toString(); 
};

const int Simulation::getNumOfActions() const{
    return actionsLog.size();
}; 


    


