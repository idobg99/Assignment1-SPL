#include "Facility.h"
enum class FacilityStatus;
enum class FacilityCategory;

class FacilityType {

    public:
        FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score):
        name (name), category (category), price (price), lifeQuality_score (lifeQuality_score), economy_score(economy_score), environment_score (environment_score) {} ;
        const string &getName() const{
            return name;
        };
        int getCost() const{
            return price;
        };
        int getLifeQualityScore() const{
            return lifeQuality_score; 
        };
        int getEnvironmentScore() const{
            return environment_score;
        };
        int getEconomyScore() const{
            return economy_score;
        };
        FacilityCategory getCategory() const{
            return category;
        };

    protected:
        const string name;
        const FacilityCategory category;
        const int price;
        const int lifeQuality_score;
        const int economy_score;
        const int environment_score;
};

class Facility: public FacilityType {

    public:
        Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score):
        FacilityType (name, category, price, lifeQuality_score, economy_score, environment_score),settlementName(settlementName)        
        {
            setStatus(FacilityStatus::UNDER_CONSTRUCTIONS);                   
            timeLeft = price;
        };

        Facility(FacilityType &type, const string &settlementName):
        FacilityType (name, category, price, lifeQuality_score, economy_score, environment_score),settlementName(settlementName)        
        {
            setStatus(FacilityStatus::UNDER_CONSTRUCTIONS);                   
            timeLeft = price;
        };

        const string &getSettlementName() const{
            return settlementName;
        };
        const int getTimeLeft() const{
            return timeLeft;
        };

        FacilityStatus step(){
            if (status == FacilityStatus::UNDER_CONSTRUCTIONS){
            timeLeft--;
            }

            if (timeLeft==0){
                setStatus(FacilityStatus::OPERATIONAL);
            }
        };
        void setStatus(FacilityStatus status){
            status = status;
        };
        const FacilityStatus& getStatus() const{
            return status;
        };
        const string toString() const{
            return "facility " +name+ " "+ std::to_string(static_cast<int>(category))+" "+ std::to_string(price)+ " "+ 
            std::to_string(lifeQuality_score)+ " "+ std::to_string(economy_score)+ " "+ std::to_string(environment_score);      
        };

    private:
        const string settlementName;
        FacilityStatus status;
        int timeLeft;
};
