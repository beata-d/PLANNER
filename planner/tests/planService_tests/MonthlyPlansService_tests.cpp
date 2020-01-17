#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Plan.h"
#include "MockView.h"
#include <array>
#include "MonthlyPlansService.h"
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace testing;

class MonthlyPlansService_tests :
        public Test {
protected:
    MonthlyPlansService monthly_plan_service;
    MockView view;
};

TEST_F(MonthlyPlansService_tests, getMonthFromUserForReadingPlans_emptyString)
{
    EXPECT_CALL(view, get_month_from_user()).WillOnce(Return(""));
    monthly_plan_service.get_month_from_user_for_reading_plans(view);

    std::string month = boost::gregorian::day_clock::local_day().month().as_long_string();
    EXPECT_EQ(monthly_plan_service.get_month(), month);
}

TEST_F(MonthlyPlansService_tests, getMonthFromUserForReadingPlans_invalidMonthName)
{
    std::string current_month = boost::gregorian::day_clock::local_day().month().as_long_string();
    EXPECT_CALL(view, get_month_from_user()).WillOnce(Return("sfsfsf")).WillOnce(Return(current_month));
    monthly_plan_service.get_month_from_user_for_reading_plans(view);

    EXPECT_EQ(monthly_plan_service.get_month(), current_month);
}

TEST_F(MonthlyPlansService_tests, getMonthFromUserForReadingPlans_pastMonth)
{
    boost::gregorian::date d = boost::gregorian::day_clock::local_day();
    std::string past_month = ( d-boost::gregorian::months(1) ).month().as_long_string();
    EXPECT_CALL(view, get_month_from_user()).WillOnce(Return(past_month));
    monthly_plan_service.get_month_from_user_for_reading_plans(view);

    EXPECT_EQ(monthly_plan_service.get_month(), past_month);
}

TEST_F(MonthlyPlansService_tests, getMonthFromUserForReadingPlans_currentMonth)
{
    std::string current_month = boost::gregorian::day_clock::local_day().month().as_long_string();
    EXPECT_CALL(view, get_month_from_user()).WillOnce(Return(current_month));
    monthly_plan_service.get_month_from_user_for_reading_plans(view);

    EXPECT_EQ(monthly_plan_service.get_month(), current_month);
}

TEST_F(MonthlyPlansService_tests, getMonthFromUserForReadingPlans_futureMonth)
{
    boost::gregorian::date d = boost::gregorian::day_clock::local_day();
    std::string future_month = ( d+boost::gregorian::months(1) ).month().as_long_string();
    EXPECT_CALL(view, get_month_from_user()).WillOnce(Return(future_month));
    monthly_plan_service.get_month_from_user_for_reading_plans(view);

    EXPECT_EQ(monthly_plan_service.get_month(), future_month);
}

TEST_F(MonthlyPlansService_tests, getMonthFromUserForCreatingPlans_emptyString)
{
    EXPECT_CALL(view, get_month_from_user()).WillOnce(Return(""));
    monthly_plan_service.get_month_from_user_for_creating_plan(view);

    std::string month = boost::gregorian::day_clock::local_day().month().as_long_string();
    EXPECT_EQ(monthly_plan_service.get_month(), month);
}

TEST_F(MonthlyPlansService_tests, getMonthFromUserForCreatingPlans_invalidMonthName)
{
    std::string current_month = boost::gregorian::day_clock::local_day().month().as_long_string();
    EXPECT_CALL(view, get_month_from_user()).WillOnce(Return("sfsfsf")).WillOnce(Return(current_month));
    monthly_plan_service.get_month_from_user_for_creating_plan(view);

    EXPECT_EQ(monthly_plan_service.get_month(), current_month);
}

TEST_F(MonthlyPlansService_tests, getMonthFromUserForCreatingPlans_pastMonthCurrentYear)
{
    boost::gregorian::date d = boost::gregorian::day_clock::local_day();
    std::string current_month = boost::gregorian::day_clock::local_day().month().as_long_string();
    std::string past_month = ( d-boost::gregorian::months(1) ).month().as_long_string();
    EXPECT_CALL(view, get_month_from_user()).WillOnce(Return(past_month)).WillOnce(Return(current_month));

    int current_year = boost::gregorian::day_clock::local_day().year();
    monthly_plan_service.set_year(current_year);
    monthly_plan_service.get_month_from_user_for_creating_plan(view);

    EXPECT_EQ(monthly_plan_service.get_month(), current_month);
}

TEST_F(MonthlyPlansService_tests, getMonthFromUserForCreatingPlans_currentMonthCurrentYear)
{
    std::string current_month = boost::gregorian::day_clock::local_day().month().as_long_string();
    EXPECT_CALL(view, get_month_from_user()).WillOnce(Return(current_month));

    int current_year = boost::gregorian::day_clock::local_day().year();
    monthly_plan_service.set_year(current_year);
    monthly_plan_service.get_month_from_user_for_creating_plan(view);

    EXPECT_EQ(monthly_plan_service.get_month(), current_month);
}

TEST_F(MonthlyPlansService_tests, getMonthFromUserForCreatingPlans_futureMonthCurrentYear)
{
    boost::gregorian::date d = boost::gregorian::day_clock::local_day();
    std::string current_month = boost::gregorian::day_clock::local_day().month().as_long_string();
    std::string future_month = ( d+boost::gregorian::months(1) ).month().as_long_string();
    EXPECT_CALL(view, get_month_from_user()).WillOnce(Return(future_month));

    int current_year = boost::gregorian::day_clock::local_day().year();
    monthly_plan_service.set_year(current_year);
    monthly_plan_service.get_month_from_user_for_creating_plan(view);

    EXPECT_EQ(monthly_plan_service.get_month(), future_month);
}

TEST_F(MonthlyPlansService_tests, getMonthFromUserForCreatingPlans_pastMonthFutureYear)
{
    boost::gregorian::date d = boost::gregorian::day_clock::local_day();
    std::string current_month = boost::gregorian::day_clock::local_day().month().as_long_string();
    std::string past_month = ( d-boost::gregorian::months(1) ).month().as_long_string();
    EXPECT_CALL(view, get_month_from_user()).WillOnce(Return(past_month));

    int future_year = boost::gregorian::day_clock::local_day().year()+1;
    monthly_plan_service.set_year(future_year);
    monthly_plan_service.get_month_from_user_for_creating_plan(view);

    EXPECT_EQ(monthly_plan_service.get_month(), past_month);
}