#include "Settlement.h"

enum class SettlementType;

class Settlement {
    public:
        Settlement::Settlement(const string &name, SettlementType type) :
        name (name), type(type) {};    
        const string &getName() const{
            return name;
        };
        SettlementType getType() const{
            return type;
        };

        const string toString() const{
            if (type ==SettlementType::VILLAGE){
                return "Settlement " + name + "0" ;
            }
            else if (type ==SettlementType::CITY) {
                return "Settlement " + name + "1" ;
            }
            else{
                return "Settlement  " + name + "2" ;
            }           
        };

        private:
            const string name;
            SettlementType type;

};
