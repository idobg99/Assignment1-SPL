#include "Settlement.h"

enum class SettlementType;

class Settlement {
    public:
        Settlement(const string &name, SettlementType type) :
        name (name), type(type) {};    
        const string &getName() const{
            return name;
        };
        SettlementType getType() const{
            return type;
        };

        const string toString() const{
            return name + " " + std::to_string(static_cast<int>(type));         
        };

        private:
            const string name;
            SettlementType type;

};
