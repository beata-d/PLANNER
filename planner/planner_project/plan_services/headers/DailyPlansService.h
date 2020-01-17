#ifndef PLANNER_DAILYPLANSSERVICE_H
#define PLANNER_DAILYPLANSSERVICE_H

#include <boost/date_time/gregorian/greg_date.hpp>
#include "PlansService.h"

class DailyPlan;
class View;

class DailyPlansService :
        public PlansService {
public:
    DailyPlansService() = default;
    ~DailyPlansService() override = default;
    void create_plan(View& view) override;
    void read_plans(View& view, std::vector<std::unique_ptr<Plan>>& plans) override;
    void update_plan(Plan& plan, View& view) override;
    void mark_done(Plan& plan) override;
    void remove_plan(const Plan& plan) override;
    void set_date(const boost::gregorian::date& date);
    virtual boost::gregorian::date get_date_from_user(const View& view, bool for_reading_plans);
    virtual const boost::gregorian::date& get_date() const;
private:
    boost::gregorian::date get_date_from_user_for_reading_plans(const View& view);
    boost::gregorian::date get_date_from_user_for_creating_plan(const View& view);
    static bool is_int(const std::string& str);
    boost::gregorian::date date;
};

#endif //PLANNER_DAILYPLANSSERVICE_H
