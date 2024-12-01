#include "Plan.h"
#include <iostream>
#include <algorithm>
using std::cout;
using std::endl;

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, 
            const vector<FacilityType> &facilityOptions): 
    plan_id(planId),
    settlement(const_cast<Settlement &>(settlement)), // Remove constness of settlement reference
    selectionPolicy(selectionPolicy),
    status(PlanStatus::AVALIABLE),
    facilities(),
    underConstruction(),
    facilityOptions(facilityOptions),
    life_quality_score(0),
    economy_score(0),
    environment_score(0),
    constructionLimit() {

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
    } 
    else {
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
    static const string nve = "nve";
    static const string bal = "bal";
    static const string env = "env";
    static const string eco = "eco";

    return (selectionPolicy->toString() == "nve") ? nve : 
           (selectionPolicy->toString() == "bal") ? bal :
           (selectionPolicy->toString() == "env") ? env : eco;
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
    auto it = std::remove_if(
    underConstruction.begin(),
    underConstruction.end(),
    [this](Facility* f) {
        if (f->step() == FacilityStatus::OPERATIONAL) {
            this->life_quality_score = this->life_quality_score + f->getLifeQualityScore();
            this->economy_score = this->economy_score + f->getEconomyScore();
            this->environment_score = this->environment_score + f->getEnvironmentScore();
            facilities.push_back(f); 
            return true; // Mark for removal from underConstruction
            }
            return false; // Keep in underConstruction
        }
    );
    underConstruction.erase(it, underConstruction.end());

    // step 4 - by playbook
    // Changed the status based on the number of facilities under construction
    if (underConstruction.size() == constructionLimit) {status = PlanStatus::BUSY;}
    else {status = PlanStatus::AVALIABLE;}
};

const int Plan::getPlanId() const{
    return plan_id;
};

// Rule of 5

// Destructor
Plan::~Plan() {
    delete selectionPolicy; // Free the dynamically allocated selectionPolicy

    // Free the facilities and underConstruction pointers
    for (Facility* facility : facilities) {
        delete facility;
    }
    for (Facility* facility : underConstruction) {
        delete facility;
    }
}

// Copy constructor
Plan::Plan(const Plan &other) 
    : plan_id(other.plan_id),
      settlement(other.settlement), 
      selectionPolicy(other.selectionPolicy->clone()), 
      status(other.status),
      facilities(),
      underConstruction(),
      facilityOptions(other.facilityOptions), 
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score),
      constructionLimit(other.constructionLimit) {

    // Deep copy the facilities vector
    for (Facility* facility : other.facilities) {
        facilities.push_back(new Facility(*facility));
    }

    // Deep copy the underConstruction vector
    for (Facility* facility : other.underConstruction) {
        underConstruction.push_back(new Facility(*facility));
    }
}

/*// Copy assignment operator 
Plan& Plan::operator=(const Plan &other) {
    if (this != &other) { // Avoid self-assignment
        // Clean up existing resources
        delete selectionPolicy;
        for (Facility* facility : facilities) {
            delete facility;
        }
        facilities.clear();

        for (Facility* facility : underConstruction) {
            delete facility;
        }
        underConstruction.clear();

        // Copy primitive and directly copyable members
        plan_id = other.plan_id;
        settlement = other.settlement; // Reference remains the same
        selectionPolicy = other.selectionPolicy->clone(); // Clone the selectionPolicy
        status = other.status;
        facilityOptions = other.facilityOptions; // Reference remains the same
        life_quality_score = other.life_quality_score;
        economy_score = other.economy_score;
        environment_score = other.environment_score;
        constructionLimit = other.constructionLimit;

        // Deep copy the facilities vector
        for (Facility* facility : other.facilities) {
            facilities.push_back(new Facility(*facility));
        }

        // Deep copy the underConstruction vector
        for (Facility* facility : other.underConstruction) {
            underConstruction.push_back(new Facility(*facility));
        }
    }
    return *this;
}*/

// Move constructor 
Plan::Plan(Plan &&other) noexcept 
    : plan_id(other.plan_id),
      settlement(other.settlement),
      selectionPolicy(other.selectionPolicy),
      status(other.status),
      facilities(std::move(other.facilities)),
      underConstruction(std::move(other.underConstruction)),
      facilityOptions(other.facilityOptions),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score),
      constructionLimit(other.constructionLimit)
       {

    other.selectionPolicy = nullptr; // Nullify the source pointer to avoid double deletion
}

/*//Move assignment operator
Plan& Plan::operator=(Plan &&other) noexcept {
    if (this != &other) { // Avoid self-assignment
        // Clean up existing resources
        delete selectionPolicy;
        for (Facility* facility : facilities) {
            delete facility;
        }
        facilities.clear();

        for (Facility* facility : underConstruction) {
            delete facility;
        }
        underConstruction.clear();

        // Transfer ownership of resources
        plan_id = other.plan_id;
        settlement = other.settlement;
        selectionPolicy = other.selectionPolicy;
        status = other.status;
        facilityOptions = other.facilityOptions;
        life_quality_score = other.life_quality_score;
        economy_score = other.economy_score;
        environment_score = other.environment_score;
        constructionLimit = other.constructionLimit;
        facilities = std::move(other.facilities);
        underConstruction = std::move(other.underConstruction);

        // Nullify the source pointer to avoid double deletion
        other.selectionPolicy = nullptr;
    }
    return *this;
}*/


