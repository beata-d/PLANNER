#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include "Plan.h"
#include "MockView.h"
#include <array>
#include "PlansService.h"

using namespace testing;

class PlansService_tests :
        public Test {
protected:
    PlansService plan_service;
    MockView view;
};

TEST_F(PlansService_tests, markDone_markTrue)
{
    Plan plan(1, "desc", "cat", false);
    plan_service.mark_done(plan);
    EXPECT_EQ(plan.is_done(), true);
}

TEST_F(PlansService_tests, markDone_markFalse)
{
    Plan plan(1, "desc", "cat", true);
    plan_service.mark_done(plan);
    EXPECT_EQ(plan.is_done(), false);
}

TEST_F(PlansService_tests, createPlan_setCategoryAndDescription)
{
    EXPECT_CALL(view, get_description_from_user()).WillOnce(Return("desc"));
    EXPECT_CALL(view, get_category_from_user()).WillOnce(Return("cat"));

    plan_service.create_plan(view);
    EXPECT_EQ(plan_service.get_category(), "cat");
    EXPECT_EQ(plan_service.get_description(), "desc");
}

TEST_F(PlansService_tests, readPlans_clearVectorOfPlans)
{
    auto plan = std::make_unique<Plan>(Plan(1, "desc", "cat", false));
    auto plan2 = std::make_unique<Plan>(Plan(2, "desc2", "cat2", false));
    auto plan3 = std::make_unique<Plan>(Plan(3, "desc3", "cat3", false));

    std::vector<std::unique_ptr<Plan>> plans;
    plans.emplace_back(move(plan));
    plans.emplace_back(move(plan2));
    plans.emplace_back(move(plan3));
    EXPECT_EQ(plans.size(), 3);

    plan_service.read_plans(view, plans);
    EXPECT_EQ(plans.size(), 0);
}

TEST_F(PlansService_tests, updatePlan_updateBothCategoryAndDescritpion)
{
    Plan plan(1, "desc", "cat", false);

    EXPECT_CALL(view, get_description_from_user()).WillOnce(Return("desc2"));
    EXPECT_CALL(view, get_category_from_user()).WillOnce(Return("cat2"));

    plan_service.create_plan(view);
    EXPECT_EQ(plan_service.get_category(), "cat2");
    EXPECT_EQ(plan_service.get_description(), "desc2");
}

TEST_F(PlansService_tests, update_plan_updateOnlyCategory)
{
    Plan plan(1, "desc", "cat", false);

    EXPECT_CALL(view, get_description_from_user()).WillOnce(Return("desc"));
    EXPECT_CALL(view, get_category_from_user()).WillOnce(Return("cat2"));

    plan_service.create_plan(view);
    EXPECT_EQ(plan_service.get_category(), "cat2");
    EXPECT_EQ(plan_service.get_description(), "desc");
}

TEST_F(PlansService_tests, updatePlan_updateOnlyDescription)
{
    Plan plan(1, "desc", "cat", false);

    EXPECT_CALL(view, get_description_from_user()).WillOnce(Return("desc2"));
    EXPECT_CALL(view, get_category_from_user()).WillOnce(Return("cat"));

    plan_service.create_plan(view);
    EXPECT_EQ(plan_service.get_category(), "cat");
    EXPECT_EQ(plan_service.get_description(), "desc2");
}

TEST_F(PlansService_tests, updatePlan_updateNeitherCategoryNorDescription)
{
    Plan plan(1, "desc", "cat", false);

    EXPECT_CALL(view, get_description_from_user()).WillOnce(Return("desc"));
    EXPECT_CALL(view, get_category_from_user()).WillOnce(Return("cat"));

    plan_service.create_plan(view);
    EXPECT_EQ(plan_service.get_category(), "cat");
    EXPECT_EQ(plan_service.get_description(), "desc");
}