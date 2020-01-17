#include <boost/date_time/gregorian/gregorian.hpp>
#include "SqlConnection.h"
#include "DailyPlan.h"
#include "DailyPlansService.h"
#include "View.h"

void DailyPlansService::create_plan(View& view)
{
    PlansService::create_plan(view);
    DailyPlan plan(description, category, date);
    SqlConnection sql_connection;
    sql_connection.add_plan(plan);
}

void DailyPlansService::mark_done(Plan& plan)
{
    PlansService::mark_done(plan);
    SqlConnection sql_connection;
    sql_connection.mark_daily_done(plan);
}

//change description or category of a plan
void DailyPlansService::update_plan(Plan& plan, View& view)
{
    PlansService::update_plan(plan, view);
    SqlConnection sql_connection;
    sql_connection.update_daily_plan(plan);
}

void DailyPlansService::read_plans(View& view, std::vector<std::unique_ptr<Plan>>& plans)
{
    PlansService::read_plans(view, plans);
    SqlConnection sql_connection;
    sql_connection.read_all_daily_plans(plans, date);
    view.create_daily_plans_window(plans, date);
}

void DailyPlansService::remove_plan(const Plan& plan)
{
    SqlConnection sql_connection;
    sql_connection.delete_daily_plan(plan);
}

boost::gregorian::date DailyPlansService::get_date_from_user(const View& view, bool for_reading_plans)
{
    bool exception_flag = true;

    //get a day, month and year from a user; repeat until a date is correct
    while (exception_flag) {
        try {
            if (for_reading_plans) {
                date = get_date_from_user_for_reading_plans(view);
            }
            else {
                date = get_date_from_user_for_creating_plan(view);
            }
            exception_flag = false;
        }
        catch (std::out_of_range&) {
        }
    }
    return date;
}

boost::gregorian::date DailyPlansService::get_date_from_user_for_reading_plans(const View& view)
{
    int day = 0, month = 0, year = 0;
    std::array<std::string, 3> array_date; //contains day,month,year

    //get day, month and year from user
    //if any parameter is empty - set it to the current one
    //if value entered by user is an integer value - convert it to int
    do {
        array_date = view.get_date_from_user();
        if (array_date[0].length()==0) {
            day = boost::gregorian::day_clock::local_day().day().as_number();
        }
        else if (is_int(array_date[0])) {
            day = stoi(array_date[0]);
        }

        if (array_date[1].length()==0) {
            month = boost::gregorian::day_clock::local_day().month().as_number();
        }
        else if (is_int(array_date[1])) {
            month = stoi(array_date[1]);
        }

        if (array_date[2].length()==0) {
            year = boost::gregorian::day_clock::local_day().year();
        }
        else if (is_int(array_date[2])) {
            year = stoi(array_date[2]);
        }

        //when a date is invalid (e.g. 30.02) throw an exception
        try {
            date = boost::gregorian::date(year, month, day);

        }
        catch (std::out_of_range&) {
            throw;
        }
    }
        //continue if user entered values that are not ints
    while (date.is_not_a_date());

    return date;
}

boost::gregorian::date DailyPlansService::get_date_from_user_for_creating_plan(const View& view)
{
    int day = 0, month = 0, year = 0;
    std::array<std::string, 3> array_date; //contains day,month,year

    //get day, month and year from user
    //if any parameter is empty - set it to the current one
    //if value entered by user is an integer value - convert it to int
    do {
        array_date = view.get_date_from_user();
        if (array_date[0].length()==0) {
            day = boost::gregorian::day_clock::local_day().day().as_number()+1;
        }
        else if (is_int(array_date[0])) {
            day = stoi(array_date[0]);
        }

        if (array_date[1].length()==0) {
            month = boost::gregorian::day_clock::local_day().month().as_number();
        }
        else if (is_int(array_date[1])) {
            month = stoi(array_date[1]);
        }

        if (array_date[2].length()==0) {
            year = boost::gregorian::day_clock::local_day().year();
        }
        else if (is_int(array_date[2])) {
            year = stoi(array_date[2]);
        }

        //when a date is invalid (e.g. 30.02) throw an exception
        try {
            date = boost::gregorian::date(year, month, day);

        }
        catch (std::out_of_range&) {
            throw;
        }
    }
        //continue if user entered values that are not ints or if entered date is from the past
    while (date.is_not_a_date() || date<boost::gregorian::day_clock::local_day());

    return date;
}

void DailyPlansService::set_date(const boost::gregorian::date& date)
{
    DailyPlansService::date = date;
}

bool DailyPlansService::is_int(const std::string& str)
{
    return str.find_first_not_of("0123456789")==std::string::npos;
}

const boost::gregorian::date& DailyPlansService::get_date() const
{
    return date;
}
