#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Plan.h"
#include "MockView.h"
#include "AnnualPlansService.h"
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace testing;

class AnnualPlansService_tests :
        public Test {
protected:
    AnnualPlansService annual_plan_service;
    MockView view;
};

TEST_F(AnnualPlansService_tests, getYearFromUserForReadingPlan_emptyYearString)
{
    EXPECT_CALL(view, get_year_from_user()).WillOnce(Return(""));
    annual_plan_service.get_year_from_user_for_reading_plans(view);

    int current_year = boost::gregorian::day_clock::local_day().year();
    EXPECT_EQ(annual_plan_service.get_year(), current_year);
}

TEST_F(AnnualPlansService_tests, getYearFromUserForReadingPlan_yearNotInt)
{
    EXPECT_CALL(view, get_year_from_user()).WillOnce(Return("201x")).WillOnce(Return("2020"));
    annual_plan_service.get_year_from_user_for_reading_plans(view);

    EXPECT_EQ(annual_plan_service.get_year(), 2020);
}

TEST_F(AnnualPlansService_tests, getYearFromUserForReadingPlan_validYearName)
{
    EXPECT_CALL(view, get_year_from_user()).WillOnce(Return("2022"));
    annual_plan_service.get_year_from_user_for_reading_plans(view);

    EXPECT_EQ(annual_plan_service.get_year(), 2022);
}

TEST_F(AnnualPlansService_tests, getYearFromUserForCreatingPlan_emptyYearString)
{
    EXPECT_CALL(view, get_year_from_user()).WillOnce(Return(""));
    annual_plan_service.get_year_from_user_for_creating_plan(view);

    int current_year = boost::gregorian::day_clock::local_day().year();
    EXPECT_EQ(annual_plan_service.get_year(), current_year);
}

TEST_F(AnnualPlansService_tests, getYearFromUserForCreatingPlan_yearNotInt)
{
    int current_year = boost::gregorian::day_clock::local_day().year();
    EXPECT_CALL(view, get_year_from_user()).WillOnce(Return("201x")).WillOnce(Return(std::to_string(current_year)));
    annual_plan_service.get_year_from_user_for_creating_plan(view);

    EXPECT_EQ(annual_plan_service.get_year(), current_year);
}

TEST_F(AnnualPlansService_tests, getYearFromUserForCreatingPlan_pastYear)
{
    int past_year = boost::gregorian::day_clock::local_day().year()-1;
    int next_year = boost::gregorian::day_clock::local_day().year()+1;
    EXPECT_CALL(view, get_year_from_user()).WillOnce(Return(std::to_string(past_year))).WillOnce(
            Return(std::to_string(next_year)));
    annual_plan_service.get_year_from_user_for_creating_plan(view);

    EXPECT_EQ(annual_plan_service.get_year(), next_year);
}

TEST_F(AnnualPlansService_tests, getYearFromUserForCreatingPlan_futureYear)
{
    int future_year = boost::gregorian::day_clock::local_day().year()+1;
    EXPECT_CALL(view, get_year_from_user()).WillOnce(Return(std::to_string(future_year)));
    annual_plan_service.get_year_from_user_for_creating_plan(view);

    EXPECT_EQ(annual_plan_service.get_year(), future_year);
}
