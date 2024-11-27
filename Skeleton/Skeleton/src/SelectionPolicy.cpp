#include "SelectionPolicy.h"
#include <stdexcept>   // For exceptions
#include <limits>      // For numeric limits
#include <algorithm>   // For std::max and std::min

// NaiveSelection Constructor 
NaiveSelection::NaiveSelection():lastSelectedIndex(-1) {};

// Returns the next facility - useing circular advancment
const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::out_of_range("No facilities available for selection.");
    }

    // Increment lastSelectedIndex and wrap around using modulo to achieve circular behavior
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
};

// ToString
const string NaiveSelection::toString() const {
    return "nve";
}

// Creates deep copy
NaiveSelection* NaiveSelection::clone() const {
    return new NaiveSelection(*this);
}




// BalancedSelection Constructor 
BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore):
        LifeQualityScore(LifeQualityScore),
        EconomyScore(EconomyScore),
        EnvironmentScore(EnvironmentScore) {};

// Returns the facility that minimizes the difference between the maximum and minimum scores
const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {

    int minDistance = std::numeric_limits<int>::max();  // Initialize minimum distance to a large value
    const FacilityType* selectedFacility = nullptr;     // Pointer to track the best facility

    for (const FacilityType& facility : facilitiesOptions) {
        // Calculate new scores after selecting this facility
        int newLifeQuality = LifeQualityScore + facility.getLifeQualityScore();
        int newEconomy = EconomyScore + const_cast<FacilityType&>(facility).getEconomyScore(); // Issue: facility.getEconomyScore() should ideally be const.
        int newEnvironment = EnvironmentScore + facility.getEnvironmentScore();

        // Find the max and min scores and calculate the balance distance
        int maxScore = std::max({newLifeQuality, newEconomy, newEnvironment});
        int minScore = std::min({newLifeQuality, newEconomy, newEnvironment});
        int balanceDistance = maxScore - minScore;

        // Update the selected facility if a smaller balance distance is found
        if (balanceDistance < minDistance) {
            minDistance = balanceDistance;
            selectedFacility = &facility;
        }
    }

    return *selectedFacility;
};

// Provides a string representation of the current scores for debugging or logging
const string BalancedSelection::toString() const {
    return "bal";
    // return "Balanced Selection Policy: Life Quality-" + std::to_string(LifeQualityScore) + " , EconomyScore-" 
    //                                                   + std::to_string(EconomyScore) + " , EnvironmentScore-"
    //                                                   + std::to_string(EnvironmentScore);
};

// Creates a copy of the current object
BalancedSelection* BalancedSelection::clone() const {
    return new BalancedSelection(*this);
};


// EconomySelection Constructor 
EconomySelection::EconomySelection(): lastSelectedIndex(0) {};

// Returns the next facility with the ECONOMY category
const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    std::size_t startIndex = lastSelectedIndex; // Start searching from the last selected index.
    std::size_t i = startIndex;

    do {
        const FacilityType& facility = facilitiesOptions[i];
        if (facility.getCategory() == FacilityCategory::ECONOMY) {  // Check if the facility matches the ECONOMY category.
            lastSelectedIndex = (i + 1) % facilitiesOptions.size();  // Update the index for circular selection.
            return facility;
        }

        i = (i + 1) % facilitiesOptions.size();  // Move to the next index.
    } while (i != startIndex);  // Loop back to the starting point.

    throw std::runtime_error("No economy facility found");  // Exception if no suitable facility is found.
};

// ToString
const string EconomySelection::toString() const {
    return "eco";
    //return "Economy Selection Policy - index: " + std::to_string(lastSelectedIndex);
};


// Creates a deep copy
EconomySelection* EconomySelection::clone() const {
    return new EconomySelection(*this);
};


// SustainabilitySelection Constructor
SustainabilitySelection::SustainabilitySelection():lastSelectedIndex(0) {};

// Implements selection logic to find the next facility with the ENVIRONMENT category.
const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    std::size_t startIndex = lastSelectedIndex; // Start searching from the last selected index.
    std::size_t i = startIndex;

    do {
        const FacilityType& facility = facilitiesOptions[i];
        if (facility.getCategory() == FacilityCategory::ENVIRONMENT) {  // Check if the facility matches the ENVIRONMENT category.
            lastSelectedIndex = (i + 1) % facilitiesOptions.size();  // Update the index for circular selection.
            return facility;
        }

        i = (i + 1) % facilitiesOptions.size();  // Move to the next index.
    } while (i != startIndex);  // Loop back to the starting point.

    throw std::runtime_error("No sustainability facility found");  // Exception if no suitable facility is found.
};

// ToString
const string SustainabilitySelection::toString() const {
    return "env";
    // return "Sustainability Selection Policy - index: " + std::to_string(lastSelectedIndex);
};


// Creates deep copy
SustainabilitySelection* SustainabilitySelection::clone() const {
    return new SustainabilitySelection(*this);
};
