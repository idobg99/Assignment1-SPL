#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::vector;

enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step();
        void printStatus();
        const vector<Facility*> &getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;
        const string &getSelectionPolicy() const;

        // Ido added
        const int getPlanId() const;
        ~Plan();
        Plan(const Plan &other); // Copy constructor
        Plan& operator=(const Plan &other); // Copy assignment operator
        Plan(Plan &&other) noexcept; // Move constructor
        Plan& operator=(Plan &&other) noexcept; // Move assignment operator

    private:
        int plan_id;
        Settlement &settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;

        // Ido added
        std::size_t constructionLimit;
};