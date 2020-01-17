#ifndef PLANNER_ANNUALPLANSSERVICE_H
#define PLANNER_ANNUALPLANSSERVICE_H

#include "PlansService.h"

class AnnualPlan;

class AnnualPlansService :
        public PlansService {
public:
    AnnualPlansService() = default;
    ~AnnualPlansService() override = default;
    void create_plan(View& view) override;
    void read_plans(View& view, std::vector<std::unique_ptr<Plan>>& plans) override;
    void update_plan(Plan& plan, View& view) override;
    void mark_done(Plan& plan) override;
    void remove_plan(const Plan& plan) override;
    virtual void get_year_from_user_for_creating_plan(const View& view);
    virtual void get_year_from_user_for_reading_plans(const View& view);
    virtual int get_year() const;
    virtual void set_year(int year);
protected:
    int year;

};

#endif //PLANNER_ANNUALPLANSSERVICE_H
