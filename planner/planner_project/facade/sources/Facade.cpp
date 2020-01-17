#include <boost/date_time/gregorian/gregorian.hpp>
#include "DailyPlansService.h"
#include "WeeklyPlansService.h"
#include "MonthlyPlansService.h"
#include "AnnualPlansService.h"
#include "Facade.h"
#include "Plan.h"
#include "PlansService.h"
#include "View.h"

int main()
{
    Facade facade;
    facade.start();

    while (true) {
        char ch = facade.get_view()->get_menu_option_from_user();
        if (ch=='q') {
            facade.get_view()->exit();
            return 0;
        }
        else {
            facade.perform_appropriate_action(ch);
        }
    }
}

//print menu and plans for a current day
void Facade::start()
{
    boost::gregorian::date today = boost::gregorian::day_clock::local_day();
    auto daily_plan_service = std::make_unique<DailyPlansService>(DailyPlansService());
    daily_plan_service->set_date(today);
    plan_service = move(daily_plan_service);
    plan_service->read_plans(*view, plans);
    get_view()->create_menu_window();
    get_view()->create_output_window();
    get_view()->create_input_window();
}

//call the appropriate functions depending on the option selected by the user from the menu
void Facade::perform_appropriate_action(char ch)
{
    switch (ch) {
    case 'd':
        print_daily_plans();
        break;
    case 'w':
        print_weekly_plans();
        break;
    case 'm':
        print_monthly_plans();
        break;
    case 'y':
        print_annual_plans();
        break;
    case 'a':
        add_daily_plan();
        break;
    case 'e':
        add_weekly_plan();
        break;
    case 'o':
        add_monthly_plan();
        break;
    case 'n':
        add_annual_plan();
        break;
    case 'u':
        update_plan();
        break;
    case 'r':
        remove_plan();
        break;
    case 'k':
        mark_plan_done();
        break;
    default:
        break;
    }
}

//choose from a list a plan which you want to change
std::unique_ptr<Plan> Facade::choose_plan_to_change()
{
    int plan_number = get_view()->choose_plan_to_change(plans.size());
    return move(plans[plan_number]);
}

void Facade::print_daily_plans()
{
    auto daily_plan_service = std::make_unique<DailyPlansService>(DailyPlansService());
    daily_plan_service->get_date_from_user(*view, true);
    plan_service = ( move(daily_plan_service) );
    plan_service->read_plans(*view, plans);
}

void Facade::print_weekly_plans()
{
    auto weekly_plan_service = std::make_unique<WeeklyPlansService>(WeeklyPlansService());
    weekly_plan_service->get_date_from_user(*view, true);
    weekly_plan_service->set_week_number();
    weekly_plan_service->set_year();
    plan_service = ( move(weekly_plan_service) );
    plan_service->read_plans(*view, plans);
}

void Facade::print_monthly_plans()
{
    auto monthly_plan_service = std::make_unique<MonthlyPlansService>(MonthlyPlansService());
    monthly_plan_service->get_month_from_user_for_reading_plans(*view);
    monthly_plan_service->get_year_from_user_for_reading_plans(*view);
    plan_service = ( move(monthly_plan_service) );
    plan_service->read_plans(*view, plans);
}

void Facade::print_annual_plans()
{
    auto annual_plan_service = std::make_unique<AnnualPlansService>(AnnualPlansService());
    annual_plan_service->get_year_from_user_for_reading_plans(*view);
    plan_service = ( move(annual_plan_service) );
    plan_service->read_plans(*view, plans);
}

void Facade::add_daily_plan()
{
    auto daily_plan_service = std::make_unique<DailyPlansService>(DailyPlansService());
    daily_plan_service->get_date_from_user(*view, false);
    daily_plan_service->create_plan(*view);
    plan_service = ( move(daily_plan_service) );
    plan_service->read_plans(*view, plans);
}

void Facade::add_weekly_plan()
{
    auto weekly_plan_service = std::make_unique<WeeklyPlansService>(WeeklyPlansService());
    weekly_plan_service->get_date_from_user(*view, false);
    weekly_plan_service->create_plan(*view);
    plan_service = ( move(weekly_plan_service) );
    plan_service->read_plans(*view, plans);
}

void Facade::add_monthly_plan()
{
    auto monthly_plan_service = std::make_unique<MonthlyPlansService>(MonthlyPlansService());
    monthly_plan_service->get_month_from_user_for_creating_plan(*view);
    monthly_plan_service->get_year_from_user_for_creating_plan(*view);
    monthly_plan_service->create_plan(*view);
    plan_service = ( move(monthly_plan_service) );
    plan_service->read_plans(*view, plans);
}

void Facade::add_annual_plan()
{
    auto annual_plan_service = std::make_unique<AnnualPlansService>(AnnualPlansService());
    annual_plan_service->get_year_from_user_for_creating_plan(*view);
    annual_plan_service->create_plan(*view);
    plan_service = ( move(annual_plan_service) );
    plan_service->read_plans(*view, plans);
}

void Facade::update_plan()
{
    if (plans.empty()) {
        return;
    }
    else {
        std::unique_ptr<Plan> plan = nullptr;
        plan = choose_plan_to_change();
        plan_service->update_plan(*plan, *view);
        plan_service->read_plans(*view, plans);
    }
}

void Facade::mark_plan_done()
{
    if (plans.empty()) {
        return;
    }
    else {
        std::unique_ptr<Plan> plan = nullptr;
        plan = choose_plan_to_change();
        plan_service->mark_done(*plan);
        plan_service->read_plans(*view, plans);
    }
}

void Facade::remove_plan()
{
    if (plans.empty()) {
        return;
    }
    else {
        std::unique_ptr<Plan> plan = nullptr;
        plan = choose_plan_to_change();
        plan_service->remove_plan(*plan);
        plan_service->read_plans(*view, plans);
    }
}

Facade::Facade()
        :view {std::make_unique<View>()}, plan_service {std::make_unique<PlansService>()}
{
}

const std::unique_ptr<View>& Facade::get_view() const
{
    return view;
}
