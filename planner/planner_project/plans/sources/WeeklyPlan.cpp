#include "WeeklyPlan.h"

WeeklyPlan::WeeklyPlan(std::string description, std::string category, int week_number, int year)
        :AnnualPlan(std::move(description), std::move(category), year), week_number {week_number}
{
}

WeeklyPlan::WeeklyPlan(int id, std::string description, std::string category, int week_number, int year, bool done)
        :AnnualPlan(id, std::move(description), std::move(category), done, year), week_number {week_number}
{
}

int WeeklyPlan::get_week_number() const
{
    return week_number;
}

