#ifndef PLANNER_MONTHLYPLAN_H
#define PLANNER_MONTHLYPLAN_H

#include "Plan.h"
#include "AnnualPlan.h"

class MonthlyPlan :
        public AnnualPlan {
public:
    MonthlyPlan(std::string description, std::string category, std::string month, int year);
    MonthlyPlan(int id, std::string description, std::string category, std::string month, bool done, int year);
    ~MonthlyPlan() override = default;
    const std::string& get_month() const;
private:
    std::string month;
};

#endif //PLANNER_MONTHLYPLAN_H
