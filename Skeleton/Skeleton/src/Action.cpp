#include "Action.h"
#include <iostream>
using namespace std;
enum class SettlementType;
enum class FacilityCategory;
enum class ActionStatus;

//Implementing BaseAction class:
BaseAction::BaseAction() : errorMsg(""),status(){};
ActionStatus BaseAction::getStatus() const{
    return status;
};
void BaseAction::complete(){
    status = ActionStatus::COMPLETED;
};    
void BaseAction::error(string errorMsg){
    errorMsg = errorMsg; 
    cout << errorMsg << endl;   
    status = ActionStatus::ERROR;    
};
const string BaseAction::statusToString(){
    if (status==ActionStatus::ERROR){
        return "ERROR";
    }
    else return "COMPLETED";
};

const string &BaseAction::getErrorMsg() const{
    return errorMsg;
};

//Implementing SimulateStep Action class:
SimulateStep::SimulateStep(const int numOfSteps):numOfSteps(numOfSteps){};
void SimulateStep::act(Simulation &simulation) {};  //to complete after the step methods.

const string SimulateStep::toString() const {
    return "SimulateStep " + numOfSteps;
};
SimulateStep* SimulateStep::clone() const {
    return new SimulateStep(*this);
};


//Implementing AddPlan Action class:
AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy):settlementName(settlementName),selectionPolicy(selectionPolicy){};

void AddPlan::act(Simulation &simulation){
    Settlement* settlement = simulation.getSettlement(settlementName); 
    SelectionPolicy* policy = simulation.stringToPolicy(selectionPolicy); 
    simulation.addPlan(settlement, policy);
    if (!(simulation.isSettlementExists(settlementName))|policy==nullptr){
                error("Cannot create this plan");
        } 
    else{
        complete();        
    }      
};
const string AddPlan::toString() const{
    return "plan "+ settlementName +" "+ selectionPolicy +" ";
};
AddPlan* AddPlan::clone()const{
    return new AddPlan(*this); 
};

//Implementing AddSettlement Action class:
AddSettlement::AddSettlement(const string &settlementName,SettlementType settlementType):settlementName(settlementName),settlementType(settlementType){}; 

void AddSettlement::act(Simulation &simulation){
    bool isNew = simulation.addSettlement(new Settlement(settlementName, settlementType));
    if (!isNew){
        error("Settlement already exists");
    }
    else {
        complete();
    }
};

AddSettlement *AddSettlement::clone() const {return new AddSettlement(*this);};
const string AddSettlement::toString() const {
    return "settlement "+ settlementName+ " " +to_string(static_cast<int>(settlementType));
};

//Implementing AddFacility Action class:
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
    return "facility";
};

//Implementing PrintPlanStatus Action class:
PrintPlanStatus::PrintPlanStatus(int planId):planId(planId){};
void PrintPlanStatus::act(Simulation &simulation) {
};
PrintPlanStatus *PrintPlanStatus::clone() const {};
const string PrintPlanStatus::toString() const {};
    



