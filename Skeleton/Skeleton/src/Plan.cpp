#include "Plan.h"
#include <iostream>
using std::cout;
using std::endl;

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, 
            const vector<FacilityType> &facilityOptions): 
    plan_id(planId),
    settlement(const_cast<Settlement &>(settlement)), // Remove constness of settlement reference
    selectionPolicy(selectionPolicy),
    facilityOptions(facilityOptions),
    status(PlanStatus::AVALIABLE),
    life_quality_score(0),
    economy_score(0),
    environment_score(0) {

        constructionLimit = (settlement.getType() == SettlementType::CITY) ? 2 :
                            (settlement.getType() == SettlementType::METROPOLIS) ? 3 :
                            1;
    };



// Getter for life quality score
const int Plan::getlifeQualityScore() const {
    return life_quality_score;
}

// Getter for economy score
const int Plan::getEconomyScore() const {
    return economy_score;
}

// Getter for environment score
const int Plan::getEnvironmentScore() const {
    return environment_score;
}

// Set a new selection policy
void Plan::setSelectionPolicy(SelectionPolicy *newSelectionPolicy) {
    if (selectionPolicy != nullptr) {
        delete selectionPolicy; // Free memory of old policy
    }
    selectionPolicy = newSelectionPolicy;
}

// Print Status
void Plan::printStatus() {
    cout << "Plan Status: " << (status == PlanStatus::AVALIABLE ? "AVALIABLE" : "BUSY") << endl;
}

// Get all facilities
const vector<Facility*> &Plan::getFacilities() const {
    return facilities;
}

// Add a facility to the plan
void Plan::addFacility(Facility* facility) {
    facilities.push_back(facility);
}

const string Plan::toString() const {
    string result = "Plan ID: " + std::to_string(plan_id) + "\n";
    result += "Settlement: " + settlement.getName() + "\n";

    // Explicit handling of PlanStatus enum
    result += "Plan Status: ";
    if (status == PlanStatus::AVALIABLE) {
        result += "AVALIABLE\n";
    } else {
        result += "BUSY\n";
    }

    result += "Life Quality Score: " + std::to_string(life_quality_score) + "\n";
    result += "Economy Score: " + std::to_string(economy_score) + "\n";
    result += "Environment Score: " + std::to_string(environment_score) + "\n";

    result += "Facilities:\n";
    for (auto facility : facilities) {
        result += " - " + facility->toString() + " (Operational)\n";
    }

    result += "Under Construction:\n";
    for (auto facility : underConstruction) {
        result += " - " + facility->toString() + " (Time Left: " + std::to_string(facility->getTimeLeft()) + ")\n";
    }

    return result;
}

const string &Plan::getSelectionPolicy() const {
    return selectionPolicy->toString();   //chnage it to the that are needed? like "bal", "env"
};

void Plan::step() {

    // Plan is available
    if (status == PlanStatus::AVALIABLE) {
        // step 2 - by playbook
        while(underConstruction.size() < constructionLimit) {
            const FacilityType& ft = selectionPolicy->selectFacility(facilityOptions);
            Facility* newFacility = new Facility(const_cast<FacilityType&>(ft), settlement.getName());
            underConstruction.push_back(newFacility);
        }
    }

    // step 3 - by playbook
    int fac_index = 0;
    for (Facility* f : underConstruction) {
        FacilityStatus fs = f->step();

        if (fs == FacilityStatus::OPERATIONAL) {
            underConstruction.erase(underConstruction.begin() + fac_index); // Remove from underConstruction
            facilities.push_back(f); // Add to facilities
            fac_index--; // CHECK FOR CORRECTNESS OF DELETEION AND MULTIPLE DELETIONS!!!!!!!!
        }

        fac_index++;
    }

    // step 4 - by playbook
    // Changed the status based on the number of facilities under construction
    if (underConstruction.size() == constructionLimit) {status = PlanStatus::BUSY;}
    else {status = PlanStatus::AVALIABLE;}
    
};
