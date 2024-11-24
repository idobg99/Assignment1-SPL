#include "Action.h"
#include <iostream>
using namespace std;
enum class SettlementType;
enum class FacilityCategory;
enum class ActionStatus;


BaseAction::BaseAction() : errorMsg(""),status(){};
ActionStatus BaseAction::getStatus() const{
    return status;
};
void BaseAction::complete(){
    status = ActionStatus::COMPLETED;
};    
void BaseAction::error(string errorMsg){
    cout << errorMsg << endl;   
    status = ActionStatus::ERROR; 
    errorMsg = errorMsg; 
};
const string &BaseAction::getErrorMsg() const{
    return errorMsg;
};




