#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Plan.h"
#include "MockView.h"
#include <array>
#include "DailyPlansService.h"
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace testing;

class DailyPlansService_tests :
        public Test {
protected:
    DailyPlansService daily_plan_service;
    MockView view;
};

TEST_F(DailyPlansService_tests, getDateFromUserForReadingPlans_pastDate)
{
    //create example past date and extract day, month and year to use these parameters in View::get_date_from_user()
    boost::gregorian::date current_date = boost::gregorian::day_clock::local_day();
    int past_day = current_date.day()-1;
    int past_month = current_date.month().as_number()-1;
    int past_year = current_date.year()-1;
    boost::gregorian::date past_date(past_year, past_month, past_day);

    EXPECT_CALL(view, get_date_from_user()).WillOnce(
            Return(std::array<std::string, 3> {std::to_string(past_day), std::to_string(past_month),
                                               std::to_string(past_year)}));

    EXPECT_EQ(daily_plan_service.get_date_from_user(view, true), past_date);
}

TEST_F(DailyPlansService_tests, getDateFromUserForReadingPlans_currentDate)
{
    //create a current date and extract day, month and year to use these parameters in View::get_date_from_user()
    boost::gregorian::date current_date = boost::gregorian::day_clock::local_day();
    std::string current_day = std::to_string(current_date.day());
    std::string current_month = std::to_string(current_date.month().as_number());
    std::string current_year = std::to_string(current_date.year());
    EXPECT_CALL(view, get_date_from_user()).WillOnce(
            Return(std::array<std::string, 3> {current_day, current_month, current_year}));

    EXPECT_EQ(daily_plan_service.get_date_from_user(view, true), current_date);
}

TEST_F(DailyPlansService_tests, getDateFromUserForReadingPlans_futureDate)
{
    //create example future date and extract day, month and year to use these parameters in View::get_date_from_user()
    boost::gregorian::date current_date = boost::gregorian::day_clock::local_day();
    int future_day = current_date.day()+1;
    int future_month = current_date.month().as_number()+1;
    int future_year = current_date.year()+1;
    EXPECT_CALL(view, get_date_from_user()).WillOnce(
            Return(std::array<std::string, 3> {std::to_string(future_day), std::to_string(future_month),
                                               std::to_string(future_year)}));

    boost::gregorian::date future_date(future_year, future_month, future_day);
    EXPECT_EQ(daily_plan_service.get_date_from_user(view, true), future_date);
}

TEST_F(DailyPlansService_tests, getDateFromUserForReadingPlans_emptyDayField)
{
    boost::gregorian::date current_date = boost::gregorian::day_clock::local_day();
    EXPECT_CALL(view, get_date_from_user()).WillOnce(
            Return(std::array<std::string, 3> {"", std::to_string(current_date.month()),
                                               std::to_string(current_date.year())}));

    EXPECT_EQ(daily_plan_service.get_date_from_user(view, true), current_date);
}

TEST_F(DailyPlansService_tests, getDateFromUserForReadingPlans_emptyMonthField)
{
    boost::gregorian::date current_date = boost::gregorian::day_clock::local_day();
    EXPECT_CALL(view, get_date_from_user()).WillOnce(
            Return(std::array<std::string, 3> {std::to_string(current_date.day()), "",
                                               std::to_string(current_date.year())}));

    EXPECT_EQ(daily_plan_service.get_date_from_user(view, true), current_date);
}

TEST_F(DailyPlansService_tests, getDateFromUserForReadingPlans_emptyYearField)
{
    boost::gregorian::date current_date = boost::gregorian::day_clock::local_day();
    EXPECT_CALL(view, get_date_from_user()).WillOnce(
            Return(std::array<std::string, 3> {std::to_string(current_date.day()), std::to_string(current_date.month()),
                                               ""}));

    EXPECT_EQ(daily_plan_service.get_date_from_user(view, true), current_date);
}

TEST_F(DailyPlansService_tests, getDateFromUserForReadingPlans_invalidDate)
{
    boost::gregorian::date current_date = boost::gregorian::day_clock::local_day();
    EXPECT_CALL(view, get_date_from_user()).WillOnce(
            Return(std::array<std::string, 3> {"31", "02", "2020"})).WillOnce(
            Return(std::array<std::string, 3> {std::to_string(current_date.day()), std::to_string(current_date.month()),
                                               std::to_string(current_date.year())}));

    EXPECT_EQ(daily_plan_service.get_date_from_user(view, true), current_date);
}

TEST_F(DailyPlansService_tests, getDateFromUserForCreatingPlans_emptyDayField)
{
    boost::gregorian::date current_date = boost::gregorian::day_clock::local_day();
    EXPECT_CALL(view, get_date_from_user()).WillOnce(
            Return(std::array<std::string, 3> {"", std::to_string(current_date.month()),
                                               std::to_string(current_date.year())}));

    EXPECT_EQ(daily_plan_service.get_date_from_user(view, false), current_date+boost::gregorian::days(1));
}

TEST_F(DailyPlansService_tests, getDateFromUserForCreatingPlans_emptyMonthField)
{
    boost::gregorian::date current_date = boost::gregorian::day_clock::local_day();
    EXPECT_CALL(view, get_date_from_user()).WillOnce(
            Return(std::array<std::string, 3> {std::to_string(current_date.day()), "",
                                               std::to_string(current_date.year())}));

    EXPECT_EQ(daily_plan_service.get_date_from_user(view, false), current_date);
}

TEST_F(DailyPlansService_tests, getDateFromUserForCreatingPlans_emptyYearField)
{
    boost::gregorian::date current_date = boost::gregorian::day_clock::local_day();
    EXPECT_CALL(view, get_date_from_user()).WillOnce(
            Return(std::array<std::string, 3> {std::to_string(current_date.day()), std::to_string(current_date.month()),
                                               ""}));

    EXPECT_EQ(daily_plan_service.get_date_from_user(view, false), current_date);
}

TEST_F(DailyPlansService_tests, getDateFromUserForCreatingPlans_invalidDate)
{
    boost::gregorian::date current_date = boost::gregorian::day_clock::local_day();
    EXPECT_CALL(view, get_date_from_user()).WillOnce(
            Return(std::array<std::string, 3> {"31", "02", "2020"})).WillOnce(
            Return(std::array<std::string, 3> {std::to_string(current_date.day()), std::to_string(current_date.month()),
                                               std::to_string(current_date.year())}));

    EXPECT_EQ(daily_plan_service.get_date_from_user(view, false), current_date);
}

TEST_F(DailyPlansService_tests, getDateFromUserForCreatingPlans_pastDate)
{
    boost::gregorian::date current_date = boost::gregorian::day_clock::local_day();
    int past_day = current_date.day()-1;
    int past_month = current_date.month().as_number()-1;
    int past_year = current_date.year()-1;
    boost::gregorian::date past_date(past_year, past_month, past_day);

    EXPECT_CALL(view, get_date_from_user()).WillOnce(
            Return(std::array<std::string, 3> {std::to_string(past_day), std::to_string(past_month),
                                               std::to_string(past_year)})).WillOnce(
            Return(std::array<std::string, 3> {std::to_string(current_date.day()), std::to_string(current_date.month()),
                                               std::to_string(current_date.year())}));

    EXPECT_EQ(daily_plan_service.get_date_from_user(view, false), current_date);
}

TEST_F(DailyPlansService_tests, getDateFromUserForCreatingPlans_currentDate)
{
    boost::gregorian::date current_date = boost::gregorian::day_clock::local_day();
    std::string current_day = std::to_string(current_date.day());
    std::string current_month = std::to_string(current_date.month().as_number());
    std::string current_year = std::to_string(current_date.year());
    EXPECT_CALL(view, get_date_from_user()).WillOnce(
            Return(std::array<std::string, 3> {current_day, current_month, current_year}));

    EXPECT_EQ(daily_plan_service.get_date_from_user(view, false), current_date);
}

TEST_F(DailyPlansService_tests, getDateFromUserForCreatingPlans_futureDate)
{
    boost::gregorian::date current_date = boost::gregorian::day_clock::local_day();
    int future_day = current_date.day()+1;
    int future_month = current_date.month().as_number()+1;
    int future_year = current_date.year()+1;
    EXPECT_CALL(view, get_date_from_user()).WillOnce(
            Return(std::array<std::string, 3> {std::to_string(future_day), std::to_string(future_month),
                                               std::to_string(future_year)}));

    boost::gregorian::date future_date(future_year, future_month, future_day);
    EXPECT_EQ(daily_plan_service.get_date_from_user(view, false), future_date);
}
