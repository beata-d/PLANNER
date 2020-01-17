#ifndef PLANNER_MONTHLYPLANSSERVICE_H
#define PLANNER_MONTHLYPLANSSERVICE_H

#include "AnnualPlansService.h"

class MonthlyPlan;

class MonthlyPlansService :
        public AnnualPlansService {
public:
    MonthlyPlansService() = default;
    ~MonthlyPlansService() override = default;
    void create_plan(View& view) override;
    void read_plans(View& view, std::vector<std::unique_ptr<Plan>>& plans) override;
    void update_plan(Plan& plan, View& view) override;
    void mark_done(Plan& plan) override;
    void remove_plan(const Plan& plan) override;
    virtual void get_month_from_user_for_reading_plans(const View& view);
    virtual void get_month_from_user_for_creating_plan(const View& view);
    virtual const std::string& get_month() const;
private:
    std::string month;
    static std::array<std::string, 12> months_array;
};

#endif //PLANNER_MONTHLYPLANSSERVICE_H
