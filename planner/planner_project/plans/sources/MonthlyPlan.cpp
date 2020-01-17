#include "MonthlyPlan.h"

MonthlyPlan::MonthlyPlan(std::string description, std::string category, std::string month, int year)
        :AnnualPlan(std::move(description), std::move(category), year), month {std::move(month)}
{
}

MonthlyPlan::MonthlyPlan(int id, std::string description, std::string category, std::string month, bool done,
        int year)
        :AnnualPlan(id, std::move(description), std::move(category), done, year), month {std::move(month)}
{
}

const std::string& MonthlyPlan::get_month() const
{
    return month;
}



