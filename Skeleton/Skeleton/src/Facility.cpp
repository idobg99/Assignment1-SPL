#include "Facility.h"
#include <iostream>
 using namespace std;
enum class FacilityStatus;
enum class FacilityCategory;

std::string StatustoString(FacilityStatus status) {
    switch (status) {
        case FacilityStatus::UNDER_CONSTRUCTIONS:
            return "UNDER_CONSTRUCTIONS";
        case FacilityStatus::OPERATIONAL:
            return "OPERATIONAL";
        default:
            return "UNKNOWN";
    }
}

FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score):
name (name), category (category), price (price), lifeQuality_score (lifeQuality_score), economy_score(economy_score), environment_score (environment_score) {} 
const string &FacilityType::getName() {
    return name;
};
int FacilityType::getCost() const {
    return price;
};
int FacilityType::getLifeQualityScore() const {
    return lifeQuality_score; 
};
int FacilityType::getEnvironmentScore() const {
    return environment_score;
};
int FacilityType::getEconomyScore() {
    return economy_score;
};
FacilityCategory FacilityType::getCategory() const {
    return category;
};

/*//FacilityType& FacilityType::operator=(const FacilityType&) = delete;
    // Move Assignment Operator (not possible with const members)
//FacilityType& FacilityType::operator=(FacilityType&&) = delete;

    // Move Constructor (construct a new object)
FacilityType::FacilityType(FacilityType&& other)
        : category(other.category), // Copy const members (still valid because they're constant)
          price(other.price),
          lifeQuality_score(other.lifeQuality_score),
          economy_score(other.economy_score),
          environment_score(other.environment_score) {}


FacilityType::FacilityType (FacilityType& other): category(other.category), // Copy const members (still valid because they're constant)
          price(other.price),
          lifeQuality_score(other.lifeQuality_score),
          economy_score(other.economy_score),
          environment_score(other.environment_score) {}
;*/



Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price,
                    const int lifeQuality_score, const int economy_score, const int environment_score):
                    FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score),
                    settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(price){};  
Facility::Facility(FacilityType &type, const string &settlementName):
        FacilityType(type), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(price){};   

const string &Facility::getSettlementName() const {
    return settlementName;
};
const int Facility::getTimeLeft() const {
    return timeLeft;
};
FacilityStatus Facility::step() {
    if (status == FacilityStatus::UNDER_CONSTRUCTIONS) {
        timeLeft--;
    }

    if (timeLeft == 0){
        setStatus(FacilityStatus::OPERATIONAL);
    }

    return Facility::getStatus();
};
void Facility::setStatus(FacilityStatus status){
    this->status = status;
};
const FacilityStatus& Facility::getStatus() const{
    return status;
};
const string Facility::toString() const{
    return "facility " + name + " " + std::to_string(static_cast<int>(category)) + " " + std::to_string(price) + " " + 
    std::to_string(lifeQuality_score)+ " "+ std::to_string(economy_score) + " " + std::to_string(environment_score);      
};

void Facility::reduceTimeLeft() { // ido added
    if (timeLeft > 0) {timeLeft = timeLeft - 1;}
};

