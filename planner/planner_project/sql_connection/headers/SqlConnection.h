#ifndef PLANNER_SQLCONNECTION_H
#define PLANNER_SQLCONNECTION_H

#include <string>
#include <memory>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <vector>
#include <boost/date_time/gregorian/gregorian.hpp>

class Plan;
class MonthlyPlan;
class AnnualPlan;
class WeeklyPlan;
class DailyPlan;

class SqlConnection {
public:
    SqlConnection();
    ~SqlConnection();
    SqlConnection(const SqlConnection&) = delete;
    SqlConnection(SqlConnection&&) = delete;
    SqlConnection& operator=(const SqlConnection&) = delete;
    SqlConnection& operator=(SqlConnection&&) = delete;
    void read_all_daily_plans(std::vector<std::unique_ptr<Plan >>& plans, const boost::gregorian::date& d);
    void read_all_weekly_plans(std::vector<std::unique_ptr<Plan >>& plans, int week_number, int year);
    void read_all_monthly_plans(std::vector<std::unique_ptr<Plan >>& plans, const std::string& month, int year);
    void read_all_annual_plans(std::vector<std::unique_ptr<Plan >>& plans, int year);
    void delete_daily_plan(const Plan& plan);
    void delete_weekly_plan(const Plan& plan);
    void delete_monthly_plan(const Plan& plan);
    void delete_annual_plan(const Plan& plan);
    void add_plan(const DailyPlan& plan);
    void add_plan(const WeeklyPlan& plan);
    void add_plan(const MonthlyPlan& plan);
    void add_plan(const AnnualPlan& plan);
    void update_daily_plan(const Plan& plan);
    void update_weekly_plan(const Plan& plan);
    void update_monthly_plan(const Plan& plan);
    void update_annual_plan(const Plan& plan);
    void mark_daily_done(const Plan& plan);
    void mark_weekly_done(const Plan& plan);
    void mark_monthly_done(const Plan& plan);
    void mark_annual_done(const Plan& plan);

private:
    sql::Driver* driver_ptr;
    std::unique_ptr<sql::Connection> connection_ptr;
    std::unique_ptr<sql::ResultSet> result_set_ptr;
    std::unique_ptr<sql::PreparedStatement> prepared_statement_ptr;
    void connect();
    static void print_error_info(const sql::SQLException& e);
};

#endif //PLANNER_SQLCONNECTION_H
