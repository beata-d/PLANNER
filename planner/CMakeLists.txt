cmake_minimum_required(VERSION 3.10)
project(planner)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra -pedantic")

add_executable(planner
        planner_project/plans/sources/Plan.cpp planner_project/plans/headers/Plan.h
        planner_project/plan_services/sources/PlansService.cpp planner_project/plan_services/headers/PlansService.h
        planner_project/facade/sources/Facade.cpp planner_project/facade/headers/Facade.h
        planner_project/sql_connection/sources/SqlConnection.cpp planner_project/sql_connection/headers/SqlConnection.h
        planner_project/views/sources/View.cpp planner_project/views/headers/View.h ../planner_project/plans/sources/DailyPlan.cpp
        planner_project/plans/headers/DailyPlan.h planner_project/plans/sources/WeeklyPlan.cpp
        planner_project/plans/headers/WeeklyPlan.h planner_project/plans/sources/MonthlyPlan.cpp
        planner_project/plans/headers/MonthlyPlan.h planner_project/plans/sources/AnnualPlan.cpp
        planner_project/plans/headers/AnnualPlan.h planner_project/plan_services/sources/DailyPlansService.cpp
        planner_project/plan_services/headers/DailyPlansService.h planner_project/plan_services/sources/WeeklyPlansService.cpp
        planner_project/plan_services/headers/WeeklyPlansService.h planner_project/plan_services/sources/MonthlyPlansService.cpp
        planner_project/plan_services/headers/MonthlyPlansService.h planner_project/plan_services/sources/AnnualPlansService.cpp
        planner_project/plan_services/headers/AnnualPlansService.h planner_project/login_data/LoginData.cpp
        planner_project/login_data/LoginData.h)

add_executable(planner_tests
        planner_project/plans/sources/Plan.cpp planner_project/plans/headers/Plan.h
        planner_project/plan_services/sources/PlansService.cpp planner_project/plan_services/headers/PlansService.h
        planner_project/sql_connection/sources/SqlConnection.cpp planner_project/sql_connection/headers/SqlConnection.h
        planner_project/views/sources/View.cpp planner_project/views/headers/View.h planner_project/plans/sources/DailyPlan.cpp
        planner_project/plans/headers/DailyPlan.h planner_project/plans/sources/WeeklyPlan.cpp
        planner_project/plans/headers/WeeklyPlan.h planner_project/plans/sources/MonthlyPlan.cpp
        planner_project/plans/headers/MonthlyPlan.h planner_project/plans/sources/AnnualPlan.cpp
        planner_project/plans/headers/AnnualPlan.h planner_project/plan_services/sources/DailyPlansService.cpp
        planner_project/plan_services/headers/DailyPlansService.h planner_project/plan_services/sources/WeeklyPlansService.cpp
        planner_project/plan_services/headers/WeeklyPlansService.h planner_project/plan_services/sources/MonthlyPlansService.cpp
        planner_project/plan_services/headers/MonthlyPlansService.h planner_project/plan_services/sources/AnnualPlansService.cpp
        planner_project/plan_services/headers/AnnualPlansService.h planner_project/login_data/LoginData.cpp
        planner_project/login_data/LoginData.h

        tests/main.cpp tests/planService_tests/PlanService_tests.cpp tests/MockView.h
        tests/planService_tests/DailyPlansService_tests.cpp tests/planService_tests/MonthlyPlansService_tests.cpp
        tests/planService_tests/AnnualPlansService_tests.cpp)


#boost
find_package(Boost REQUIRED date_time)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(planner ${Boost_LIBRARIES})
target_link_libraries(planner_tests ${Boost_LIBRARIES})
#end boost

#ncurses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(planner ncurses)
target_link_libraries(planner_tests ncurses)
#end ncurses

#cppconn
target_link_libraries(planner mysqlcppconn)
target_link_libraries(planner_tests mysqlcppconn)

#my headers
include_directories("planner_project/facade/headers" "planner_project/plans/headers"
        "planner_project/plan_services/headers" "planner_project/sql_connection/headers"
        "planner_project/views/headers" "planner_project/login_data"
        "tests" "tests/planService_tests")

#gtests
add_subdirectory(tests/google_tests_lib/googletest-master)
include_directories(tests/google_tests_lib/googletest-master/googletest/include)
include_directories(tests/google_tests_lib/googletest-master/googlemock/include)

target_link_libraries(planner_tests gtest gtest_main)
target_link_libraries(planner_tests gmock gmock_main)
#end gtsest
