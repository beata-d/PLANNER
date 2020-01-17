#include "AnnualPlansService.h"
#include "AnnualPlan.h"
#include "SqlConnection.h"
#include "View.h"

void AnnualPlansService::create_plan(View& view)
{
    PlansService::create_plan(view);
    AnnualPlan plan(description, category, year);
    SqlConnection sql_connection;
    sql_connection.add_plan(plan);
}

void AnnualPlansService::read_plans(View& view, std::vector<std::unique_ptr<Plan>>& plans)
{
    PlansService::read_plans(view, plans);
    SqlConnection sql_connection;
    sql_connection.read_all_annual_plans(plans, year);
    view.create_annual_plans_window(plans, year);
}

void AnnualPlansService::update_plan(Plan& plan, View& view)
{
    PlansService::update_plan(plan, view);
    SqlConnection sql_connection;
    sql_connection.update_annual_plan(plan);
}

void AnnualPlansService::mark_done(Plan& plan)
{
    PlansService::mark_done(plan);
    SqlConnection sql_connection;
    sql_connection.mark_annual_done(plan);
}

void AnnualPlansService::remove_plan(const Plan& plan)
{
    SqlConnection sql_connection;
    sql_connection.delete_annual_plan(plan);
}

void AnnualPlansService::get_year_from_user_for_creating_plan(const View& view)
{
    bool is_int = true;
    std::string s_year;
    do {
        s_year = view.get_year_from_user();

        //if user didn't enter a year - set it to the current one
        if (s_year.empty()) {
            year = boost::gregorian::day_clock::local_day().year();
        }
            //check whether a year is an integer type
        else {
            is_int = s_year.find_first_not_of("0123456789")==std::string::npos;
            if (is_int) {
                year = stoi(s_year);
            }
        }
    }
    //repeat until entered year is integer and is not past year
    while (!is_int || ( year<boost::gregorian::day_clock::local_day().year() ));
}

void AnnualPlansService::get_year_from_user_for_reading_plans(const View& view)
{
    bool is_int = true;
    std::string s_year;
    do {
        s_year = view.get_year_from_user();

        //if user didn't enter a year - set it to the current one
        if (s_year.empty()) {
            year = boost::gregorian::day_clock::local_day().year();
        }
            //check whether a year is an integer type
        else {
            is_int = s_year.find_first_not_of("0123456789")==std::string::npos;
            if (is_int) {
                year = stoi(s_year);
            }
        }
    }
    //repeat until entered year is integer; it can be past, current or future year
    while (!is_int);
}

int AnnualPlansService::get_year() const
{
    return year;
}

void AnnualPlansService::set_year(int year)
{
    AnnualPlansService::year = year;
}
