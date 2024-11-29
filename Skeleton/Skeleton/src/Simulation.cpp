 #include "Simulation.h"
 #include <iostream>
 #include <fstream>
 #include "Auxiliary.h"
 #include "Settlement.h"
 #include "Facility.h"
 #include "Plan.h"
 #include "SelectionPolicy.h"
 #include "Action.h"
 #include "globals.h"
 using namespace std;

Simulation::Simulation(const string &configFilePath):isRunning(false), planCounter(0), 
     actionsLog(), plans(), settlements(), facilitiesOptions() {
     ifstream configFile(configFilePath);
     string line;

     while (getline(configFile, line)){       
         vector<string> inf(Auxiliary::parseArguments(line));
         if (inf[0]=="settlement"&&inf.size() == 3){            
             addSettlement(new Settlement(inf[1],SettlementType(static_cast<SettlementType>((stoi(inf[2])))) ));
         }
         else if (inf[0]=="facility"&&inf.size()==7){
             addFacility(FacilityType(inf[1],static_cast<FacilityCategory>((stoi(inf[2]))),stoi(inf[3]),stoi(inf[4]), stoi(inf[5]), stoi(inf[6])));
         }
         else if (inf[0]=="plan"&&inf.size()==3){
             SelectionPolicy* policy = stringToPolicy(inf[2]);          
             addPlan(getSettlement(inf[1]),policy);                                         
         }       
     }
     configFile.close();
};

void Simulation::start(){
    open();

    while (isRunning) {
        string userCommand;
        getline(cin, userCommand);
        vector<string> inf(Auxiliary::parseArguments(userCommand));

        if (inf[0] == "step" && inf.size() == 2) {             
            addAction(new SimulateStep(stoi(inf[1])));
        }
        else if (inf[0] == "plan" && inf.size() == 3) {           
            addAction(new AddPlan(inf[1], inf[2]));                   
        }
        else if (inf[0] == "settlement" && inf.size() == 3) {
            SettlementType type = SettlementType(static_cast<SettlementType>((stoi(inf[2]))));
            addAction(new AddSettlement(inf[1],type)); 
            if (!isSettlementExists(inf[1])){
                addSettlement(new Settlement(inf[1],type));
            }   
        }                                            
        else if (inf[0] == "facility" && inf.size() == 7) {           
            addAction(new AddFacility((inf[1]),static_cast<FacilityCategory>((stoi(inf[2]))),stoi(inf[3]),stoi(inf[4]),stoi(inf[5]),stoi(inf[6]))); 
        }
        else if (inf[0] == "planStatus" && inf.size() == 2) {           
            addAction(new PrintPlanStatus(stoi(inf[1]))); 
        }
         else if (inf[0] == "changePolicy" && inf.size() == 3) {           
            addAction(new ChangePlanPolicy(stoi(inf[1]), inf[2])); 
        }
        else if (inf[0] == "log" && inf.size() == 1 ){           
            addAction(new PrintActionsLog()); 
        }
        else if (inf[0] == "backup" && inf.size() == 1) {           
            addAction(new BackupSimulation()); 
        }
        else if (inf[0] == "restore" && inf.size() == 1) {
            addAction(new RestoreSimulation());
        }
        // Exit condition
        else if (inf[0] == "close" && inf.size() == 1) {
            addAction(new Close()); 
            close();
            break;        
        }                                   
    }
};

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
        if (f.getName()==facility.getName()) {return false;}  // CHECK VALIDITY OF == OPERATOR FOR NAME STRING
    }
    facilitiesOptions.push_back(facility);
    return true;
};

bool Simulation::isSettlementExists(const string &settlementName){
    for (Settlement *s:settlements){
        if ((s->getName())==settlementName) {
            return true;
        }
    }
    return false;
};
        
Settlement* Simulation::getSettlement(const string &settlementName){
    for (Settlement* s:settlements){
        if ((*s).getName()==settlementName) {return s;}                    
    }    
    return nullptr; 
};

bool Simulation::isPlanExist(const int planId){
    if (planId >= plans.size()){
         return false;
    }
    return true;
};

Plan &Simulation::getPlan(const int planID){   
    return plans[planID];
};

void Simulation::step(){
    // Iterate over all palns
    for (Plan p : plans) {
        p.step();
    }
};

void Simulation::close(){
    isRunning = false;
    cout << "The simulation has finished with success"<< endl;
};
void Simulation::open(){
    isRunning = true;
    cout << "The simulation has started"<< endl;
};

SelectionPolicy* Simulation::stringToPolicy (const string &policy){
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
    return nullptr;
};


const string Simulation::getAction(int actionNumber) const{
    BaseAction &actionToString = *actionsLog[actionNumber] ;
    return (actionToString).toString(); 
};

const int Simulation::getNumOfActions() const{
    return actionsLog.size();
}; 

const int Simulation::getNumOfPlans() const{
    return plans.size();
}; 


//Disctructor for Simulation:
Simulation::~Simulation(){
    for(BaseAction* a: actionsLog){
        delete a;
    } 
    for (Settlement* s: settlements){
        delete s;
    }
};

/*// Copy costructor:
Simulation::Simulation(Simulation &other){  
    for (Settlement* s: other.settlements){
        addSettlement(new Settlement(*s));
    }
    for (BaseAction* a:other.actionsLog){
        addAction(a->clone());
    }
    for (Plan p: other.plans){
        Plan newP = Plan(p);
        plans.push_back(newP);
        planCounter++;
    }
};      */


Simulation::Simulation(Simulation &other) : 
    isRunning(other.isRunning),
    planCounter(other.planCounter),
    facilitiesOptions(other.facilitiesOptions) {

    // Deep copy settlements
    for (Settlement* settlement : other.settlements) {
        settlements.push_back(new Settlement(*settlement));
    }

    // Deep copy plans
    for (const Plan& plan : other.plans) {
        plans.push_back(plan); // ASSIMING PLAN HAS A DEEP COPY CONSTRUCTOR - IMPLEMENT!!!
    }

    // Deep copy actions log
    for (BaseAction* action : other.actionsLog) {
        actionsLog.push_back(action->clone()); // Assuming BaseAction has a virtual clone method
    }
};

Simulation& Simulation::operator=(const Simulation& other) {
    if (this != &other) {  // Avoid self-assignment
        // Clean up existing resources
        for (Settlement* settlement : settlements) {
            delete settlement;
        }
        settlements.clear();

        for (BaseAction* action : actionsLog) {
            delete action;
        }
        actionsLog.clear();

        // Copy primitive and directly copyable members
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        facilitiesOptions = other.facilitiesOptions;

        // Deep copy settlements
        for (Settlement* settlement : other.settlements) {
            settlements.push_back(new Settlement(*settlement));
        }

        // Deep copy plans
        plans.clear();
        for (const Plan& plan : other.plans) {
            plans.push_back(plan);  // Assuming Plan has a deep copy constructor
        }

        // Deep copy actions log
        for (BaseAction* action : other.actionsLog) {
            actionsLog.push_back(action->clone());  // Assuming BaseAction has a clone method
        }
    }

    return *this;
}


    


