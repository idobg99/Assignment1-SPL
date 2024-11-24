#include "SelectionPolicy.h"
#include <stdexcept>
#include <limits> 
#include <algorithm>

NaiveSelection::NaiveSelection():lastSelectedIndex(0){};

const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::out_of_range("No facilities available for selection.");
    }

    // Circular selection of facilities
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
};
const string NaiveSelection::toString() const {
    return "Naive Selection Policy - index: " + std::to_string(lastSelectedIndex);
}
NaiveSelection* NaiveSelection::clone() const {
    return new NaiveSelection(*this);
}

BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore):
        LifeQualityScore(LifeQualityScore),
        EconomyScore(EconomyScore),
        EnvironmentScore(EnvironmentScore) {};
const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::out_of_range("No facilities available for selection.");
    }

    int minDistance = std::numeric_limits<int>::max();
    const FacilityType* selectedFacility = nullptr;

    for (const FacilityType& facility : facilitiesOptions) {
        // Calculate the impact on scores after adding this facility
        int newLifeQuality = LifeQualityScore + facility.getLifeQualityScore();
        int newEconomy = EconomyScore + const_cast<FacilityType&>(facility).getEconomyScore();
        int newEnvironment = EnvironmentScore + facility.getEnvironmentScore();

        int maxScore = std::max({newLifeQuality, newEconomy, newEnvironment});
        int minScore = std::min({newLifeQuality, newEconomy, newEnvironment});
        int balanceDistance = maxScore - minScore;

        if (balanceDistance < minDistance) {
            minDistance = balanceDistance;
            selectedFacility = &facility;
        }
    }

    if (!selectedFacility) {
        throw std::logic_error("Unable to select a balanced facility.");
    }

    return *selectedFacility;
};
