#ifndef PLANNER_PLANSSERVICE_H
#define PLANNER_PLANSSERVICE_H

#include <memory>
#include <vector>

class Plan;
class View;
class SqlConnection;
class PlansService {

public:
    PlansService() = default;
    virtual ~PlansService() = default;
    virtual void create_plan(View& view);
    virtual void read_plans(View& view, std::vector<std::unique_ptr<Plan>>& plans);
    virtual void update_plan(Plan& plan, View& view);
    virtual void mark_done(Plan& plan);
    virtual void remove_plan(const Plan& plan);
    virtual const std::string& get_description() const;
    virtual const std::string& get_category() const;
protected:
    std::string description;
    std::string category;
};

#endif //PLANNER_PLANSSERVICE_H
