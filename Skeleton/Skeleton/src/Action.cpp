#include "Action.h" 
#include "globals.h" 
//#include "/workspaces/Assignment1-SPL/Skeleton/Skeleton/include/globals.h"
//#include "/workspaces/Assignment1-SPL/Skeleton/Skeleton/include/Action.h"
#include <iostream>
using namespace std;
enum class SettlementType;
enum class FacilityCategory;
enum class ActionStatus;

// Implementing BaseAction class:
BaseAction::BaseAction() : stringStatus(""), errorMsg(""), status(){}
ActionStatus BaseAction::getStatus() const{
    return status;
};
void BaseAction::complete(){
    status = ActionStatus::COMPLETED;
    stringStatus = " COMPLETED";
};    
void BaseAction::error(string errorMsg){
    errorMsg = errorMsg; 
    cout << errorMsg << endl;   
    status = ActionStatus::ERROR; 
    stringStatus = " ERROR";   
};

const string &BaseAction::getErrorMsg() const{
    return errorMsg;
};

// Implementing SimulateStep Action class:
SimulateStep::SimulateStep(const int numOfSteps): numOfSteps(numOfSteps){};
void SimulateStep::act(Simulation &simulation) {
    for (int i = 0; i < numOfSteps; ++i) {
        simulation.step();
    }
}; 

const string SimulateStep::toString() const {
    return "SimulateStep " + to_string(numOfSteps)+ " COMPLETED";
};
SimulateStep* SimulateStep::clone() const {
    return new SimulateStep(*this);
};

// Implementing AddPlan Action class:
AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy):settlementName(settlementName),selectionPolicy(selectionPolicy){};

void AddPlan::act(Simulation &simulation){
    Settlement* settlement = simulation.getSettlement(settlementName); 
    SelectionPolicy* policy = simulation.stringToPolicy(selectionPolicy);        
    if (!(simulation.isSettlementExists(settlementName))||settlement==nullptr||policy==nullptr){
        //simulation.addPlan(nullptr, nullptr);
        error("Cannot create this plan");       
    } 
    else {
        simulation.addPlan(settlement, policy);
        complete();        
    }      
};
const string AddPlan::toString() const{
    return "plan "+ settlementName +" "+ selectionPolicy + stringStatus;
};
AddPlan* AddPlan::clone()const{
    return new AddPlan(*this); 
};

// Implementing AddSettlement Action class:
AddSettlement::AddSettlement(const string &settlementName,SettlementType settlementType):settlementName(settlementName),settlementType(settlementType){}; 
void AddSettlement::act(Simulation &simulation){
    if (simulation.isSettlementExists(settlementName)){
        error("Settlement already exists");
    }
    else {
        simulation.addSettlement(new Settlement(settlementName,settlementType));
        complete();
    }
};

AddSettlement *AddSettlement::clone() const {
    return new AddSettlement(*this);
};
const string AddSettlement::toString() const {
    return "settlement "+ settlementName+ " " +to_string(static_cast<int>(settlementType)) +stringStatus;
};

// Implementing AddFacility Action class:
AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore):
facilityName(facilityName),facilityCategory(facilityCategory),price(price),lifeQualityScore(lifeQualityScore),economyScore(economyScore),environmentScore(environmentScore){};

void AddFacility::act(Simulation &simulation){
    FacilityType newFacilty(facilityName,facilityCategory,price,lifeQualityScore,economyScore,environmentScore);
    bool isNew = simulation.addFacility(newFacilty);
    if (!isNew){
        error("Facility already exists");
    }
    else{
        complete();
    }    
};
AddFacility *AddFacility::clone() const {return new AddFacility(*this);};
const string AddFacility::toString() const {
    return "facility "+ facilityName+" "+to_string(static_cast<int>(facilityCategory))+" "+
    to_string(price)+" "+to_string(lifeQualityScore)+" "+to_string(economyScore)+" "+to_string(environmentScore)+stringStatus;
};

// Implementing PrintPlanStatus Action class:
PrintPlanStatus::PrintPlanStatus(int planId): planId(planId){};
void PrintPlanStatus::act(Simulation &simulation) {
    if (!simulation.isPlanExist(planId)){
        error("Plan doesn’t exist");
    }
    else{
        Plan& plan = simulation.getPlan(planId);
        cout << plan.toString() << endl;
        complete();
    }
};
PrintPlanStatus *PrintPlanStatus::clone() const {return new PrintPlanStatus(*this);};
const string PrintPlanStatus::toString() const {
    return "planStatus "+ to_string(planId) +stringStatus;
};

// Implementing ChangePlanPolicy Action class:
ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy): planId(planId),newPolicy(newPolicy) {};
void ChangePlanPolicy::act(Simulation &simulation){
    if (!simulation.isPlanExist(planId)){
        error("Cannot change selection policy");
    }
    else{
        Plan& plan = simulation.getPlan(planId);

        if (newPolicy == plan.getSelectionPolicy()){  //to change in selectionPolicy class
            error("Cannot change selection policy to the same one");
        }
        else{
            string prevPolicy = plan.getSelectionPolicy(); 
            SelectionPolicy* newPol = simulation.stringToPolicy(newPolicy);
            plan.setSelectionPolicy(newPol);

            cout << plan.toString() << endl;

            cout << "planID: " + to_string(planId) <<endl;
            cout << "previousPolicy: " + prevPolicy <<endl;
            cout << "newPolicy: " +(*newPol).toString()<<endl;
            complete();
        }            
    }          
};

ChangePlanPolicy *ChangePlanPolicy::clone() const {
    return new ChangePlanPolicy(*this); 
};
const string ChangePlanPolicy::toString() const {
    return "ChangePlanPolicy " + to_string(planId)+" "+ newPolicy; 
};

// Implementing PrintActionsLog Action class:
PrintActionsLog::PrintActionsLog(){};
void PrintActionsLog::act(Simulation &simulation) {
    for (int i = 0;i<simulation.getNumOfActions() - 1;i++){
        cout<< simulation.getAction(i) <<endl;
    }
};
PrintActionsLog *PrintActionsLog::clone() const{
    return new PrintActionsLog(*this);
};
const string PrintActionsLog::toString() const {
    return "PrintActionsLog COMPLETED";
};
    
// Implementing BackupSimulation Action class:
BackupSimulation::BackupSimulation(){};

// Backup the simulation
// deleting the previos backup
// deep copying the current simulation to the backup global variable
void BackupSimulation::act(Simulation &simulation) {
    delete backup;
    backup = new Simulation(simulation);
    complete();
};

BackupSimulation *BackupSimulation::clone() const {
    return new BackupSimulation(*this);
};

const string BackupSimulation::toString() const {
    return "BackUpSimulation "+ stringStatus;
};

// Implementing RestoreSimulation Action calss:
RestoreSimulation::RestoreSimulation() {};

void RestoreSimulation::act(Simulation &simulation) {
    if (backup == nullptr) {
        error("no backup available");
    }
    else {
        simulation = *backup;
    }
};
RestoreSimulation *RestoreSimulation::clone() const{
    return new RestoreSimulation(*this);
};
const string RestoreSimulation::toString() const {
    return "RestoreSimulation:" + stringStatus;
};

// Implementing Close Action calss:
Close:: Close(){};
void Close::act(Simulation &simulation) {
    for (int i =0; i< simulation.getNumOfPlans();i++ ){
        Plan p(simulation.getPlan(i));
        cout << p.toString() << endl;
    }
    complete();    
};
Close *Close::clone() const {
    return new Close(*this);
};
const string Close::toString() const {
    return "Close: COMPLETED";
};