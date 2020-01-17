#ifndef PLANNER_WEEKLYPLAN_H
#define PLANNER_WEEKLYPLAN_H

#include "Plan.h"
#include "AnnualPlan.h"

class WeeklyPlan :
        public AnnualPlan {
public:
    WeeklyPlan(std::string description, std::string category, int week_number, int year);
    WeeklyPlan(int id, std::string description, std::string category, int week_number, int year, bool done);
    ~WeeklyPlan() override = default;
    int get_week_number() const;
private:
    int week_number;
};

#endif //PLANNER_WEEKLYPLAN_H
