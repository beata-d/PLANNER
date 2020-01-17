#ifndef PLANNER_WEEKLYPLANSSERVICE_H
#define PLANNER_WEEKLYPLANSSERVICE_H

#include <boost/date_time/gregorian/greg_date.hpp>
#include "PlansService.h"
#include "DailyPlansService.h"

class WeeklyPlan;

class WeeklyPlansService :
        public DailyPlansService {
public:
    WeeklyPlansService() = default;
    ~WeeklyPlansService() override = default;
    void create_plan(View& view) override;
    void read_plans(View& view, std::vector<std::unique_ptr<Plan>>& plans) override;
    void update_plan(Plan& plan, View& view) override;
    void mark_done(Plan& plan) override;
    void remove_plan(const Plan& plan) override;
    virtual void set_week_number();
    virtual void set_year();
    virtual int get_year() const;
private:
    int week_number;
    int year;
};

#endif //PLANNER_WEEKLYPLANSSERVICE_H
