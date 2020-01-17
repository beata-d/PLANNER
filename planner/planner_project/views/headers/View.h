#ifndef PLANNER_VIEW_H
#define PLANNER_VIEW_H

#include <ncurses.h>
#include <memory>
#include <vector>

class Plan;

class View {
public:
    View();
    virtual ~View() = default;
    View(const View&) = delete;
    View(View&&) = delete;
    View& operator=(const View&) = delete;
    View& operator=(View&&) = delete;
    void create_daily_plans_window(const std::vector<std::unique_ptr<Plan>>& plans, boost::gregorian::date date);
    void create_weekly_plans_window(const std::vector<std::unique_ptr<Plan>>& plans, boost::gregorian::date date);
    void
    create_monthly_plans_window(const std::vector<std::unique_ptr<Plan>>& plans, const std::string& month, int year);
    void create_annual_plans_window(const std::vector<std::unique_ptr<Plan>>& plans, int year);
    void create_menu_window();
    void create_output_window();
    void create_input_window();
    void clear_input_window() const;
    void clear_output_window() const;
    virtual std::string get_description_from_user() const;
    virtual std::string get_category_from_user() const;
    virtual std::string get_month_from_user() const;
    virtual std::string get_year_from_user() const;
    virtual std::array<std::string, 3> get_date_from_user() const;
    virtual char get_menu_option_from_user() const;
    virtual int choose_plan_to_change(int number_of_plans) const;
    void exit();
private:
    WINDOW* plans_window_ptr;
    WINDOW* menu_window_ptr;
    WINDOW* output_window_ptr;
    WINDOW* input_window_ptr;
    void create_plans_window(const std::vector<std::unique_ptr<Plan>>& plans);
    void clear_highlight_in_menu() const;
    static std::string create_header_line();
    static const int line_width = 145;
    static const int rows = 50;
};

#endif //PLANNER_VIEW_H
