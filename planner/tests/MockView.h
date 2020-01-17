#ifndef PLANNER_MOCKVIEW_H
#define PLANNER_MOCKVIEW_H

#include <View.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class MockView :
        public View {
public:
    MOCK_METHOD(std::string, get_description_from_user, ( ), (const));
    MOCK_METHOD(std::string, get_category_from_user, ( ), (const));
    MOCK_METHOD((std::array<std::string, 3> ), get_date_from_user, ( ), (const));
    MOCK_METHOD(std::string, get_month_from_user, ( ), (const));
    MOCK_METHOD(std::string, get_year_from_user, ( ), (const));
    MOCK_METHOD(int, choose_plan_to_change, (int number_of_plans), (const));
};


#endif //PLANNER_MOCKVIEW_H
