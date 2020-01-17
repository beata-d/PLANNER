#ifndef PLANNER_FACADE_H
#define PLANNER_FACADE_H

#include <boost/date_time/gregorian/gregorian.hpp>

class Plan;
class PlansService;
class View;

class Facade {
public:
    Facade();
    ~Facade() = default;
    Facade(const Facade&) = delete;
    Facade(Facade&&) = delete;
    Facade& operator=(const Facade&) = delete;
    Facade& operator=(Facade&&) = delete;
    void perform_appropriate_action(char ch);
    std::unique_ptr<Plan> choose_plan_to_change();
    const std::unique_ptr<View>& get_view() const;
    void start();
private:
    void print_daily_plans();
    void print_weekly_plans();
    void print_monthly_plans();
    void print_annual_plans();
    void add_daily_plan();
    void add_weekly_plan();
    void add_monthly_plan();
    void add_annual_plan();
    void update_plan();
    void mark_plan_done();
    void remove_plan();
    std::vector<std::unique_ptr<Plan>> plans;
    std::unique_ptr<View> view;
    std::unique_ptr<PlansService> plan_service;
};

#endif //PLANNER_FACADE_H
