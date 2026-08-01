#include "components.h"



class menuEngine{
    public:
        struct menu{
            std::string_view title=nullptr;
            gsl::span<const menu>submenu={};
            using menuAction=void(*)(const menu&,const displayEngine& display);
            menuAction action;
        };

        struct menuFrame{
            gsl::span<const menu>menu={};
            uint8_t rowIndex=0;
        };

        static constexpr uint8_t maxDepth=5;
        std::array<menuFrame,maxDepth>menuHistory={};
        uint8_t currentDepth=0;

        explicit menuEngine(menuFrame menu)
                :menuHistory{menu},currentDepth(0){}

        void up(){}

        void down(){}

        void back(){}

        void select(){}
};



