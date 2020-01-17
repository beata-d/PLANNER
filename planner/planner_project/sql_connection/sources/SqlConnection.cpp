#include <iostream>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/gregorian/parsers.hpp>
#include "SqlConnection.h"
#include "Plan.h"
#include "MonthlyPlan.h"
#include "WeeklyPlan.h"
#include "DailyPlan.h"
#include "AnnualPlan.h"
#include "LoginData.h"

SqlConnection::SqlConnection()
{
    connect();
}

SqlConnection::~SqlConnection()
{
    connection_ptr->close();
}

void SqlConnection::connect()
{
    std::string my_login;
    std::string my_password;
    LoginData login_data;
    try {
        login_data.open_file();
        my_login = login_data.read_login();
        my_password = login_data.read_password();
        login_data.close_file();
    }
    catch (std::ios_base::failure& e) {
        std::cerr
                << "There is a problem with the file"
                << "\n" << e.what()
                << std::endl;
    }
    std::string schema = "planner";

    try {
        //initialize a connection_ptr
        driver_ptr = get_driver_instance();
        connection_ptr = std::unique_ptr<sql::Connection>(driver_ptr->connect("localhost", my_login, my_password));
        //connect to the MySQL database
        connection_ptr->setSchema(schema);
    }
    catch (sql::SQLException& e) {
        print_error_info(e);
    }
}

void SqlConnection::read_all_daily_plans(std::vector<std::unique_ptr<Plan >>& plans, const boost::gregorian::date& d)
{
    try {
        prepared_statement_ptr = std::unique_ptr<sql::PreparedStatement>(connection_ptr->prepareStatement(
                "SELECT * FROM daily WHERE date = ? ORDER BY done, category"));
        std::string str_date = boost::gregorian::to_iso_extended_string(d);
        prepared_statement_ptr->setString(1, str_date);
        prepared_statement_ptr->execute();
        result_set_ptr = std::unique_ptr<sql::ResultSet>(prepared_statement_ptr->getResultSet());

        int id;
        std::string description;
        std::string category;
        bool done;

        while (result_set_ptr->next()) {
            id = result_set_ptr->getInt("id");
            description = result_set_ptr->getString("description");
            category = result_set_ptr->getString("category");
            done = result_set_ptr->getBoolean("done");
            plans.emplace_back(std::unique_ptr<Plan>(new DailyPlan(id, description, category, d, done)));
        }
    }
    catch (sql::SQLException& e) {
        print_error_info(e);
    }
}

void SqlConnection::read_all_weekly_plans(std::vector<std::unique_ptr<Plan >>& plans, int week_number, int year)
{
    try {
        prepared_statement_ptr = std::unique_ptr<sql::PreparedStatement>(connection_ptr->prepareStatement(
                "SELECT * FROM weekly WHERE (weekNo = ? AND year = ?) ORDER BY done, category"));
        prepared_statement_ptr->setInt(1, week_number);
        prepared_statement_ptr->setInt(2, year);
        prepared_statement_ptr->execute();
        result_set_ptr = std::unique_ptr<sql::ResultSet>(prepared_statement_ptr->getResultSet());

        int id;
        std::string description;
        std::string category;
        bool done;

        while (result_set_ptr->next()) {
            id = result_set_ptr->getInt("id");
            description = result_set_ptr->getString("description");
            category = result_set_ptr->getString("category");
            done = result_set_ptr->getBoolean("done");
            plans.emplace_back(
                    std::unique_ptr<Plan>(new WeeklyPlan(id, description, category, week_number, year, done)));
        }
    }
    catch (sql::SQLException& e) {
        print_error_info(e);
    }
}

void
SqlConnection::read_all_monthly_plans(std::vector<std::unique_ptr<Plan >>& plans, const std::string& month, int year)
{
    try {
        prepared_statement_ptr = std::unique_ptr<sql::PreparedStatement>(connection_ptr->prepareStatement(
                "SELECT * FROM monthly WHERE month = ? && year = ? ORDER BY done, category"));
        prepared_statement_ptr->setString(1, month);
        prepared_statement_ptr->setInt(2, year);
        prepared_statement_ptr->execute();
        result_set_ptr = std::unique_ptr<sql::ResultSet>(prepared_statement_ptr->getResultSet());

        int id;
        std::string description;
        std::string category;
        bool done;

        while (result_set_ptr->next()) {
            id = result_set_ptr->getInt("id");
            description = result_set_ptr->getString("description");
            category = result_set_ptr->getString("category");
            done = result_set_ptr->getBoolean("done");
            year = result_set_ptr->getInt("year");
            plans.emplace_back(
                    std::unique_ptr<Plan>(new MonthlyPlan(id, description, category, month, done, year)));
        }
    }
    catch (sql::SQLException& e) {
        print_error_info(e);
    }
}

void SqlConnection::read_all_annual_plans(std::vector<std::unique_ptr<Plan >>& plans, int year)
{
    try {
        prepared_statement_ptr = std::unique_ptr<sql::PreparedStatement>(connection_ptr->prepareStatement(
                "SELECT * FROM annual WHERE year = ? ORDER BY done, category"));
        prepared_statement_ptr->setInt(1, year);
        prepared_statement_ptr->execute();
        result_set_ptr = std::unique_ptr<sql::ResultSet>(prepared_statement_ptr->getResultSet());

        int id;
        std::string description;
        std::string category;
        bool done;

        while (result_set_ptr->next()) {
            id = result_set_ptr->getInt("id");
            description = result_set_ptr->getString("description");
            category = result_set_ptr->getString("category");
            done = result_set_ptr->getBoolean("done");
            plans.emplace_back(std::unique_ptr<Plan>(new AnnualPlan(id, description, category, year, done)));
        }
    }
    catch (sql::SQLException& e) {
        print_error_info(e);
    }
}

void SqlConnection::delete_daily_plan(const Plan& plan)
{
    try {
        prepared_statement_ptr = std::unique_ptr<sql::PreparedStatement>(
                connection_ptr->prepareStatement("DELETE FROM daily WHERE id = ? "));
        prepared_statement_ptr->setInt(1, plan.get_id());
        prepared_statement_ptr->execute();

    }
    catch (sql::SQLException& e) {
        print_error_info(e);
    }
}

void SqlConnection::delete_weekly_plan(const Plan& plan)
{
    try {
        prepared_statement_ptr = std::unique_ptr<sql::PreparedStatement>(
                connection_ptr->prepareStatement("DELETE FROM weekly WHERE id = ? "));
        prepared_statement_ptr->setInt(1, plan.get_id());
        prepared_statement_ptr->execute();
    }
    catch (sql::SQLException& e) {
        print_error_info(e);
    }
}

void SqlConnection::delete_monthly_plan(const Plan& plan)
{
    try {
        prepared_statement_ptr = std::unique_ptr<sql::PreparedStatement>(
                connection_ptr->prepareStatement("DELETE FROM monthly WHERE id = ? "));
        prepared_statement_ptr->setInt(1, plan.get_id());
        prepared_statement_ptr->execute();
    }
    catch (sql::SQLException& e) {
        print_error_info(e);
    }
}

void SqlConnection::delete_annual_plan(const Plan& plan)
{
    try {
        prepared_statement_ptr = std::unique_ptr<sql::PreparedStatement>(
                connection_ptr->prepareStatement("DELETE FROM annual WHERE id = ? "));
        prepared_statement_ptr->setInt(1, plan.get_id());
        prepared_statement_ptr->execute();
    }
    catch (sql::SQLException& e) {
        print_error_info(e);
    }
}

void SqlConnection::add_plan(const DailyPlan& plan)
{
    std::string str_date = to_iso_extended_string(plan.get_date());

    try {
        prepared_statement_ptr = std::unique_ptr<sql::PreparedStatement>(connection_ptr->prepareStatement(
                "INSERT INTO daily (description, category, date ) VALUES (?, ?, ?)"));
        prepared_statement_ptr->setString(1, plan.get_description());
        prepared_statement_ptr->setString(2, plan.get_category());
        prepared_statement_ptr->setString(3, str_date);
        prepared_statement_ptr->execute();
    }
    catch (sql::SQLException& e) {
        print_error_info(e);
    }
}

void SqlConnection::add_plan(const WeeklyPlan& plan)
{
    try {
        prepared_statement_ptr = std::unique_ptr<sql::PreparedStatement>(connection_ptr->prepareStatement(
                "INSERT INTO weekly (description, category, weekNo, year ) VALUES (?, ?, ?, ?)"));
        prepared_statement_ptr->setString(1, plan.get_description());
        prepared_statement_ptr->setString(2, plan.get_category());
        prepared_statement_ptr->setInt(3, plan.get_week_number());
        prepared_statement_ptr->setInt(4, plan.get_year());
        prepared_statement_ptr->execute();
    }
    catch (sql::SQLException& e) {
        print_error_info(e);
    }
}

void SqlConnection::add_plan(const MonthlyPlan& plan)
{
    try {
        prepared_statement_ptr = std::unique_ptr<sql::PreparedStatement>(connection_ptr->prepareStatement(
                "INSERT INTO monthly (description, category, month, year ) VALUES (?, ?, ?, ?)"));
        prepared_statement_ptr->setString(1, plan.get_description());
        prepared_statement_ptr->setString(2, plan.get_category());
        prepared_statement_ptr->setString(3, plan.get_month());
        prepared_statement_ptr->setInt(4, plan.get_year());
        prepared_statement_ptr->execute();
    }
    catch (sql::SQLException& e) {
        print_error_info(e);
    }
}

void SqlConnection::add_plan(const AnnualPlan& plan)
{
    try {
        prepared_statement_ptr = std::unique_ptr<sql::PreparedStatement>(connection_ptr->prepareStatement(
                "INSERT INTO annual (description, category, year ) VALUES (?, ?, ?)"));
        prepared_statement_ptr->setString(1, plan.get_description());
        prepared_statement_ptr->setString(2, plan.get_category());
        prepared_statement_ptr->setInt(3, plan.get_year());
        prepared_statement_ptr->execute();
    }
    catch (sql::SQLException& e) {
        print_error_info(e);
    }
}

void SqlConnection::mark_daily_done(const Plan& plan)
{
    try {
        prepared_statement_ptr = std::unique_ptr<sql::PreparedStatement>(
                connection_ptr->prepareStatement("UPDATE daily SET done = ? WHERE id=?"));
        prepared_statement_ptr->setBoolean(1, plan.is_done());
        prepared_statement_ptr->setInt(2, plan.get_id());
        prepared_statement_ptr->execute();
    }
    catch (sql::SQLException& e) {
        print_error_info(e);
    }
}

void SqlConnection::mark_weekly_done(const Plan& plan)
{
    try {
        prepared_statement_ptr = std::unique_ptr<sql::PreparedStatement>(
                connection_ptr->prepareStatement("UPDATE weekly SET done = ? WHERE id=?"));
        prepared_statement_ptr->setBoolean(1, plan.is_done());
        prepared_statement_ptr->setInt(2, plan.get_id());
        prepared_statement_ptr->execute();
    }
    catch (sql::SQLException& e) {
        print_error_info(e);
    }
}

void SqlConnection::mark_monthly_done(const Plan& plan)
{
    try {
        prepared_statement_ptr = std::unique_ptr<sql::PreparedStatement>(
                connection_ptr->prepareStatement("UPDATE monthly SET done = ? WHERE id=?"));
        prepared_statement_ptr->setBoolean(1, plan.is_done());
        prepared_statement_ptr->setInt(2, plan.get_id());
        prepared_statement_ptr->execute();
    }
    catch (sql::SQLException& e) {
        print_error_info(e);
    }
}

void SqlConnection::mark_annual_done(const Plan& plan)
{
    try {
        prepared_statement_ptr = std::unique_ptr<sql::PreparedStatement>(
                connection_ptr->prepareStatement("UPDATE annual SET done = ? WHERE id=?"));
        prepared_statement_ptr->setBoolean(1, plan.is_done());
        prepared_statement_ptr->setInt(2, plan.get_id());
        prepared_statement_ptr->execute();
    }
    catch (sql::SQLException& e) {
        print_error_info(e);
    }
}

void SqlConnection::update_daily_plan(const Plan& plan)
{
    try {
        prepared_statement_ptr = std::unique_ptr<sql::PreparedStatement>(connection_ptr->prepareStatement(
                "UPDATE daily SET description=?, category=? WHERE id=?"));
        prepared_statement_ptr->setInt(3, plan.get_id());
        prepared_statement_ptr->setString(1, plan.get_description());
        prepared_statement_ptr->setString(2, plan.get_category());
        prepared_statement_ptr->execute();
    }
    catch (sql::SQLException& e) {
        print_error_info(e);
    }
}

void SqlConnection::update_weekly_plan(const Plan& plan)
{
    try {
        prepared_statement_ptr = std::unique_ptr<sql::PreparedStatement>(connection_ptr->prepareStatement(
                "UPDATE weekly SET description=?, category=? WHERE id=?"));
        prepared_statement_ptr->setInt(3, plan.get_id());
        prepared_statement_ptr->setString(1, plan.get_description());
        prepared_statement_ptr->setString(2, plan.get_category());
        prepared_statement_ptr->execute();
    }
    catch (sql::SQLException& e) {
        print_error_info(e);
    }
}

void SqlConnection::update_monthly_plan(const Plan& plan)
{
    try {
        prepared_statement_ptr = std::unique_ptr<sql::PreparedStatement>(connection_ptr->prepareStatement(
                "UPDATE monthly SET description=?, category=? WHERE id=?"));
        prepared_statement_ptr->setInt(3, plan.get_id());
        prepared_statement_ptr->setString(1, plan.get_description());
        prepared_statement_ptr->setString(2, plan.get_category());
        prepared_statement_ptr->execute();
    }
    catch (sql::SQLException& e) {
        print_error_info(e);
    }
}

void SqlConnection::update_annual_plan(const Plan& plan)
{
    try {
        prepared_statement_ptr = std::unique_ptr<sql::PreparedStatement>(connection_ptr->prepareStatement(
                "UPDATE annual SET description=?, category=? WHERE id=?"));
        prepared_statement_ptr->setInt(3, plan.get_id());
        prepared_statement_ptr->setString(1, plan.get_description());
        prepared_statement_ptr->setString(2, plan.get_category());
        prepared_statement_ptr->execute();
    }
    catch (sql::SQLException& e) {
        print_error_info(e);
    }
}

void SqlConnection::print_error_info(const sql::SQLException& e)
{
    std::cerr
            << "There is a problem with the database"
            << "\nMySQL error code: "
            << e.getErrorCode()
            << "\t"
            << e.what()
            << std::endl;
}

