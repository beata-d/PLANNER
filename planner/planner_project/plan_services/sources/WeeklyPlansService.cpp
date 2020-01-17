#include "WeeklyPlansService.h"
#include "SqlConnection.h"
#include "WeeklyPlan.h"
#include "View.h"

void WeeklyPlansService::create_plan(View& view)
{
    PlansService::create_plan(view);
    set_week_number();
    set_year();
    WeeklyPlan plan(description, category, week_number, year);
    SqlConnection sql_connection;
    sql_connection.add_plan(plan);
}

void WeeklyPlansService::mark_done(Plan& plan)
{
    PlansService::mark_done(plan);
    SqlConnection sql_connection;
    sql_connection.mark_weekly_done(plan);
}

void WeeklyPlansService::update_plan(Plan& plan, View& view)
{
    PlansService::update_plan(plan, view);
    SqlConnection sql_connection;
    sql_connection.update_weekly_plan(plan);
}

void WeeklyPlansService::read_plans(View& view, std::vector<std::unique_ptr<Plan>>& plans)
{
    PlansService::read_plans(view, plans);
    SqlConnection sql_connection;
    sql_connection.read_all_weekly_plans(plans, week_number, year);
    view.create_weekly_plans_window(plans, get_date());
}

void WeeklyPlansService::remove_plan(const Plan& plan)
{
    SqlConnection sql_connection;
    sql_connection.delete_weekly_plan(plan);
}

void WeeklyPlansService::set_week_number()
{
    week_number = get_date().week_number();
}

void WeeklyPlansService::set_year()
{
    year = get_date().year();
}

int WeeklyPlansService::get_year() const
{
    return year;
}

