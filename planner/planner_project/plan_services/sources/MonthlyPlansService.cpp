#include "MonthlyPlansService.h"
#include "SqlConnection.h"
#include "MonthlyPlan.h"
#include "View.h"

void MonthlyPlansService::create_plan(View& view)
{
    PlansService::create_plan(view);
    MonthlyPlan plan(description, category, month, year);
    SqlConnection sql_connection;
    sql_connection.add_plan(plan);
}

void MonthlyPlansService::read_plans(View& view, std::vector<std::unique_ptr<Plan>>& plans)
{
    PlansService::read_plans(view, plans);
    SqlConnection sql_connection;
    sql_connection.read_all_monthly_plans(plans, month, year);
    view.create_monthly_plans_window(plans, month, year);
}

void MonthlyPlansService::update_plan(Plan& plan, View& view)
{
    PlansService::update_plan(plan, view);
    SqlConnection sql_connection;
    sql_connection.update_monthly_plan(plan);
}

void MonthlyPlansService::mark_done(Plan& plan)
{
    PlansService::mark_done(plan);
    SqlConnection sql_connection;
    sql_connection.mark_monthly_done(plan);
}

void MonthlyPlansService::remove_plan(const Plan& plan)
{
    SqlConnection sql_connection;
    sql_connection.delete_monthly_plan(plan);
}

void MonthlyPlansService::get_month_from_user_for_reading_plans(const View& view)
{
    do {
        month = view.get_month_from_user();

        //if user didn't enter a month - set it to the current one
        if (month.empty()) {
            month = boost::gregorian::day_clock::local_day().month().as_long_string();
        }

        //check whether a month entered by user is valid
    }
    while (find(std::begin(months_array), std::end(months_array), month)==std::end(months_array));
}

void MonthlyPlansService::get_month_from_user_for_creating_plan(const View& view)
{
    std::array<std::string, 12>::iterator monthIterator;
    int current_month = boost::gregorian::day_clock::local_day().month().as_number();
    int current_year = boost::gregorian::day_clock::local_day().year();
    int month_number = -1;
    do {
        month = view.get_month_from_user();

        //if user didn't enter a month - set it to the current one
        if (month.empty()) {
            month = boost::gregorian::day_clock::local_day().month().as_long_string();
        }

        //check whether a month entered by user is valid
        if (( monthIterator = find(months_array.begin(), months_array.end(), month) )!=months_array.end()) {
            month_number = monthIterator-months_array.begin()+1;
        }
    }
        //repeat until entered pair (month, year) is current or future
        //year is always greater then or equal to current_year (it is checked in get_year_from_user_for_reading_plans() in AnnualPlansService)
    while (( month_number<current_month && year==current_year ) || month_number==-1);
}

std::array<std::string, 12> MonthlyPlansService::months_array = {"January", "February", "March", "April", "May", "June",
                                                                 "July", "August", "September", "October", "November",
                                                                 "December"};

const std::string& MonthlyPlansService::get_month() const
{
    return month;
}
