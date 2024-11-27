#include "Action.h"
#include <iostream>
using namespace std;
enum class SettlementType;
enum class FacilityCategory;
enum class ActionStatus;

//Implementing BaseAction class:
BaseAction::BaseAction(){} : errorMsg(""),status(), stringStatus("")
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

//Implementing SimulateStep Action class:
SimulateStep::SimulateStep(const int numOfSteps):numOfSteps(numOfSteps){};
void SimulateStep::act(Simulation &simulation) {};  //to complete after the step methods.

const string SimulateStep::toString() const {
    return "SimulateStep " + to_string(numOfSteps)+ " COMPLETED";
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
    return "plan "+ settlementName +" "+ selectionPolicy + stringStatus;
};
AddPlan* AddPlan::clone()const{
    return new AddPlan(*this); 
};

//Implementing AddSettlement Action class:
AddSettlement::AddSettlement(const string &settlementName,SettlementType settlementType):settlementName(settlementName),settlementType(settlementType){}; 
void AddSettlement::act(Simulation &simulation){
    if (simulation.isSettlementExists(settlementName)){
        error("Settlement already exists");
    }
    else {
        complete();
    }
};

AddSettlement *AddSettlement::clone() const {
    return new AddSettlement(*this);
};
const string AddSettlement::toString() const {
    return "settlement "+ settlementName+ " " +to_string(static_cast<int>(settlementType)) +stringStatus;
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
    return "facility "+ facilityName+" "+to_string(static_cast<int>(facilityCategory))+" "+
    to_string(price)+" "+to_string(lifeQualityScore)+" "+to_string(economyScore)+" "+to_string(environmentScore)+stringStatus;
};

//Implementing PrintPlanStatus Action class:
PrintPlanStatus::PrintPlanStatus(int planId):planId(planId){};
void PrintPlanStatus::act(Simulation &simulation) {
    if (!simulation.isPlanExist(planId)){
        error("Plan doesn’t exist");
    }
    else{
        Plan plan = simulation.getPlan(planId);
        plan.printStatus();
        complete();
    }   
};
PrintPlanStatus *PrintPlanStatus::clone() const {return new PrintPlanStatus(*this);};
const string PrintPlanStatus::toString() const {
    return "planStatus "+ planId +stringStatus;
};

//Implementing ChangePlanPolicy Action class:
ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy): planId(planId),newPolicy(newPolicy) {};
void ChangePlanPolicy::act(Simulation &simulation){
    if (!simulation.isPlanExist(planId)){
        error ("Cannot change selection policy");
    }
    else{
        Plan plan = simulation.getPlan(planId);
        if (newPolicy== plan.getSelectionPolicy()){  //to change in selectionPolicy class
            error ("Cannot change selection policy");
        }
        else{
            string prevPolicy = plan.getSelectionPolicy(); 
            SelectionPolicy* newPol = simulation.stringToPolicy(newPolicy);
            plan.setSelectionPolicy(newPol);
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

//Implementing PrintActionsLog Action class:
PrintActionsLog::PrintActionsLog(){};
void PrintActionsLog::act(Simulation &simulation) {
    for (int i = 0;i<simulation.getNumOfActions();i++){
        cout<< simulation.getAction(i) <<endl;
    }
};
PrintActionsLog *PrintActionsLog::clone() const{
    return new PrintActionsLog(*this);
};
const string PrintActionsLog::toString() const {
    return "PrintActionsLog COMPLETED";
};
    



