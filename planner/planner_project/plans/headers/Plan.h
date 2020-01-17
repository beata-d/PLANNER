#ifndef PLANNER_PLAN_H
#define PLANNER_PLAN_H

#include <string>
#include <ostream>
#include <boost/date_time/gregorian/greg_date.hpp>

class Plan {
public:
    Plan() = default;
    Plan(std::string description, std::string category);
    Plan(int id, std::string description, std::string category, bool done);
    virtual ~Plan() = default;
    std::string print() const;
    void set_description(const std::string& description);
    void set_category(const std::string& category);
    void set_done(bool done);
    int get_id() const;
    const std::string& get_description() const;
    const std::string& get_category() const;
    bool is_done() const;

private:
    int id;
    std::string description;
    std::string category;
    bool done;
};

#endif //PLANNER_PLAN_H
