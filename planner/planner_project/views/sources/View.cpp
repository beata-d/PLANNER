#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/gregorian/parsers.hpp>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include "View.h"
#include "Plan.h"


View::View()
{
//    if default terminal window size is less than needed, resize it; COLS, LINES - variables managed by ncurses library
    if (COLS<line_width || LINES<line_width) {
        system(( "resize -s "+std::to_string(rows)+" "+std::to_string(line_width) ).c_str());
    }
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    start_color();
}

void View::exit()
{
    delwin(plans_window_ptr);
    delwin(menu_window_ptr);
    delwin(output_window_ptr);
    delwin(input_window_ptr);
    endwin();
}

//create window that contains a list of plans
void View::create_plans_window(const std::vector<std::unique_ptr<Plan>>& plans)
{
    //clear the window before printing a new list of plans
    werase(plans_window_ptr);

    //create a window
    int first_plans_window_line_position = 8;
    int first_column = 0;
    int rows = 40;
    int columns = line_width;
    plans_window_ptr = newwin(rows, columns, first_plans_window_line_position, first_column);
    //make window's borders invisible
    int space = ( int )' ';
    wborder(plans_window_ptr, space, space, space, space, space, space, space, space);

    //print a line with columns' names
    init_pair(2, COLOR_WHITE, COLOR_BLUE);
    wattron(plans_window_ptr, A_BOLD);
    wattron(plans_window_ptr, COLOR_PAIR(2));
    int y0 = 2;
    int x0 = 0;
    mvwprintw(plans_window_ptr, y0, x0, create_header_line().c_str());
    wattroff(plans_window_ptr, A_BOLD);
    wattroff(plans_window_ptr, COLOR_PAIR(2));

    //print plans
    for (unsigned int i = 0; i<plans.size(); ++i) {
        mvwprintw(plans_window_ptr, i+3, 0, plans[i]->print().c_str());
    }

    refresh();
}

//add to the plans window part that is specific for daily plans
void View::create_daily_plans_window(const std::vector<std::unique_ptr<Plan>>& plans, boost::gregorian::date date)
{
    //print plans
    create_plans_window(plans);

    //print first line that contains information about day of a week and date
    wattron(plans_window_ptr, A_BOLD);
    wattron(plans_window_ptr, COLOR_PAIR(2));
    mvwprintw(plans_window_ptr, 1, 0, date.day_of_week().as_short_string());
    mvwprintw(plans_window_ptr, 1, 3, "   ");
    mvwprintw(plans_window_ptr, 1, 5, to_iso_extended_string(date).c_str());
    wattroff(plans_window_ptr, A_BOLD);
    wattroff(plans_window_ptr, COLOR_PAIR(2));

    wrefresh(plans_window_ptr);
}

//add to the plans window part that is specific for weekly plans
void View::create_weekly_plans_window(const std::vector<std::unique_ptr<Plan>>& plans, boost::gregorian::date date)
{
    create_plans_window(plans);

    //print first line that contains information about a week
    wattron(plans_window_ptr, A_BOLD);
    wattron(plans_window_ptr, COLOR_PAIR(2));
    mvwprintw(plans_window_ptr, 1, 0, to_iso_extended_string(previous_weekday(date, boost::gregorian::greg_weekday(boost::gregorian::Monday))).c_str());
    mvwprintw(plans_window_ptr, 1, 10, " - ");
    mvwprintw(plans_window_ptr, 1, 13, to_iso_extended_string(next_weekday(date, boost::gregorian::greg_weekday(boost::gregorian::Sunday))).c_str());
    wattroff(plans_window_ptr, A_BOLD);
    wattroff(plans_window_ptr, COLOR_PAIR(2));

    wrefresh(plans_window_ptr);
}

//add to the plans window part that is specific for monthly plans
void
View::create_monthly_plans_window(const std::vector<std::unique_ptr<Plan>>& plans, const std::string& month, int year)
{
    create_plans_window(plans);

    //print first line that contains information about a month
    wattron(plans_window_ptr, A_BOLD);
    wattron(plans_window_ptr, COLOR_PAIR(2));
    mvwprintw(plans_window_ptr, 1, 0, month.c_str());
    mvwprintw(plans_window_ptr, 1, 3, "   ");
    mvwprintw(plans_window_ptr, 1, 5, std::to_string(year).c_str());
    wattroff(plans_window_ptr, A_BOLD);
    wattroff(plans_window_ptr, COLOR_PAIR(2));

    wrefresh(plans_window_ptr);
}

//add to the plans window part that is specific for annual plans
void View::create_annual_plans_window(const std::vector<std::unique_ptr<Plan>>& plans, int year)
{
    create_plans_window(plans);

    //print first line that contains information about a year
    wattron(plans_window_ptr, A_BOLD);
    wattron(plans_window_ptr, COLOR_PAIR(2));
    mvwprintw(plans_window_ptr, 1, 0, std::to_string(year).c_str());
    wattroff(plans_window_ptr, A_BOLD);
    wattroff(plans_window_ptr, COLOR_PAIR(2));

    wrefresh(plans_window_ptr);
}

//create a window that contains menu
void View::create_menu_window()
{
    //create a window
    int rows = 4;
    int columns = line_width;
    int y0 = 0;
    int x0 = 0;
    menu_window_ptr = newwin(rows, columns, y0, x0);
    //make window's borders invisible
    int space = ( int )' ';
    wborder(menu_window_ptr, space, space, space, space, space, space, space, space);

    //print menu
    wattron(menu_window_ptr, A_BOLD);
    wattron(menu_window_ptr, COLOR_PAIR(2));
    mvwprintw(menu_window_ptr, y0, x0, "choose the option:");
    wattroff(menu_window_ptr, A_BOLD);
    wattroff(menu_window_ptr, COLOR_PAIR(2));
    mvwprintw(menu_window_ptr, y0+1, x0,
            "print daily plans\t\t\tprint weekly plans\t\t\tprint monthly plans\t\t\tprint annual plans");
    mvwprintw(menu_window_ptr, y0+2, x0,
            "add a daily plan\t\t\tadd a weekly plan\t\t\tadd a monthly plan\t\t\tadd an annual plan");
    mvwprintw(menu_window_ptr, y0+3, x0,
            "update current entry\t\t\tremove an entry\t\t\t\tmark a plan done\t\t\tquit the program");

    refresh();
}

//create a window that contains prompts for user
void View::create_output_window()
{
    int rows = 2;
    int columns = line_width;
    int y0 = 5;
    int x0 = 0;
    output_window_ptr = newwin(rows, columns, y0, x0);
    //make window's borders invisible
    int space = ( int )' ';
    wborder(output_window_ptr, space, space, space, space, space, space, space, space);
    refresh();
}

//create window that contains data entered by a user
void View::create_input_window()
{
    int rows = 1;
    int columns = line_width;
    int y_0 = 7;
    int x_0 = 0;
    input_window_ptr = newwin(rows, columns, y_0, x_0);
    //make window's borders invisible
    int space = ( int )' ';
    wborder(input_window_ptr, space, space, space, space, space, space, space, space);
    refresh();
}

//clear text in a box for user's input
void View::clear_input_window() const
{
    std::string clear_line(line_width, ' ');
    int y_0 = 0;
    int x_0 = 0;
    mvwprintw(input_window_ptr, y_0, x_0, clear_line.c_str());
    wrefresh(input_window_ptr);
}

//clear text in a box that contains prompts for user
void View::clear_output_window() const
{
    std::string clear_line(line_width, ' ');
    int y_0 = 0;
    int x_0 = 0;
    mvwprintw(output_window_ptr, y_0, x_0, clear_line.c_str());
    mvwprintw(output_window_ptr, y_0+1, x_0, clear_line.c_str());
    wrefresh(output_window_ptr);
}

//create line that contains columns' names
std::string View::create_header_line()
{
    int id_length = 5;
    int desc_length = 102;
    int cat_length = 28;
    int done_length = 5;

    std::stringstream buffer;
    buffer
            << std::setw(id_length)
            << std::left
            << "id"
            << std::setw(desc_length)
            << std::left
            << "description"
            << std::setw(cat_length)
            << std::left
            << "category"
            << std::setw(done_length)
            << std::left
            << std::boolalpha
            << "done  ";

    return buffer.str();
}

//get plan description from a user
std::string View::get_description_from_user() const
{
    char cstr_description[101];
    std::string description;

    echo();

    mvwprintw(output_window_ptr, 0, 0, "Enter a description.");
    wrefresh(output_window_ptr);
    mvwgetnstr(input_window_ptr, 0, 0, cstr_description, 100);

    clear_input_window();
    clear_output_window();

    description = cstr_description;

    noecho();

    return description;
}

//get plan category from a user
std::string View::get_category_from_user() const
{
    char cstr_category[26];
    std::string category;

    echo();
    mvwprintw(output_window_ptr, 0, 0, "Enter a category.");
    wrefresh(output_window_ptr);
    mvwgetnstr(input_window_ptr, 0, 0, cstr_category, 25);
    clear_input_window();
    clear_output_window();

    category = cstr_category;

    noecho();

    return category;
}

//get month from a user to create a monthly plan
std::string View::get_month_from_user() const
{
    echo();
    mvwprintw(output_window_ptr, 0, 0, "Enter a month.");
    wrefresh(output_window_ptr);

    char cstr_month[10];
    mvwgetnstr(input_window_ptr, 0, 0, cstr_month, 9);
    clear_input_window();
    clear_output_window();
    noecho();

    std::string month = cstr_month;
    return month;
}

//get year from a user to create an annual plan
std::string View::get_year_from_user() const
{
    char cstr_year[5];
    std::string str_year;

    echo();
    mvwprintw(output_window_ptr, 0, 0, "Enter a year.");
    wrefresh(output_window_ptr);
    mvwgetnstr(input_window_ptr, 0, 0, cstr_year, 4);
    clear_input_window();
    clear_output_window();
    noecho();

    std::string year = cstr_year;
    return year;
}

//get date from a user to create a daily or weekly plan
std::array<std::string, 3> View::get_date_from_user() const
{
    char cstr_day[3];
    char cstr_month[3];
    char cstr_year[5];

    boost::gregorian::date d;

    echo();
    mvwprintw(output_window_ptr, 0, 0, "Enter a date.");
    mvwprintw(output_window_ptr, 1, 0, "Day:");
    wrefresh(output_window_ptr);
    mvwgetnstr(input_window_ptr, 0, 0, cstr_day, 2);
    clear_input_window();
    clear_output_window();

    mvwprintw(output_window_ptr, 0, 0, "Enter a date.");
    mvwprintw(output_window_ptr, 1, 0, "Month:");
    wrefresh(output_window_ptr);
    mvwgetnstr(input_window_ptr, 0, 0, cstr_month, 2);
    clear_input_window();
    clear_output_window();

    mvwprintw(output_window_ptr, 0, 0, "Enter a date.");
    mvwprintw(output_window_ptr, 1, 0, "Year:");
    wrefresh(output_window_ptr);
    mvwgetnstr(input_window_ptr, 0, 0, cstr_year, 4);
    clear_input_window();
    clear_output_window();

    std::string str_day = cstr_day;
    std::string str_month = cstr_month;
    std::string str_year = cstr_year;

    return std::array<std::string, 3> {str_day, str_month, str_year};
}

//choose menu option
char View::get_menu_option_from_user() const
{
    //enable using special keyboard keys (e.g. arrows)
    keypad(menu_window_ptr, true);
    char ch;
    const int number_of_menu_options = 12;
    char menu_options[number_of_menu_options] = {'d', 'w', 'm', 'y', 'a', 'e', 'o', 'n', 'u', 'r', 'k', 'q'};
    int key_pressed_by_user;
    int highlighted_menu_option = 0;
    int number_of_options_in_one_menu_row = 4;
    int row = 1; //first row in menu window that contains options not headers
    int column_width = 28; //number of columns that should be highlighted
    int option_width = 40; //number of columns between first char of one menu option to the first char of next option in the same row

    while (true) {
        //find menu option that should be highlighted
        for (int i = 0; i<number_of_menu_options; ++i) {
            if (i==highlighted_menu_option) {
                //menu has 3 rows with 4 options in each row; find place that should be highlighted
                if (i<number_of_options_in_one_menu_row) {
                    mvwchgat(menu_window_ptr, row, i%number_of_options_in_one_menu_row*option_width, column_width,
                            A_REVERSE, 0, NULL);
                }
                else if (i>=number_of_options_in_one_menu_row && i<2*number_of_options_in_one_menu_row) {
                    mvwchgat(menu_window_ptr, row+1, i%number_of_options_in_one_menu_row*option_width, column_width,
                            A_REVERSE, 0, NULL);
                }
                else {
                    mvwchgat(menu_window_ptr, row+2, i%number_of_options_in_one_menu_row*option_width, column_width,
                            A_REVERSE, 0, NULL);
                }
            }
        }
        //check whether a user pressed an arrow or enter key
        key_pressed_by_user = wgetch(menu_window_ptr);
        switch (key_pressed_by_user) {
        case KEY_LEFT:
            highlighted_menu_option--;
            if (highlighted_menu_option==-1) {
                highlighted_menu_option = 0;
            }
            break;
        case KEY_RIGHT:
            highlighted_menu_option++;
            if (highlighted_menu_option==number_of_menu_options) {
                highlighted_menu_option = number_of_menu_options-1;
            }
            break;
        default:
            break;
        }

        //if user pressed enter get the highlighted menu option
        if (key_pressed_by_user==( int )'\n') {
            ch = menu_options[highlighted_menu_option];
            break;
        }

        //before next change of highlighted option, clear highlight of a previous one
        clear_highlight_in_menu();
    }

    //after choosing a menu option clear a highlight
    clear_highlight_in_menu();

    keypad(menu_window_ptr, false);
    return ch;
}

//clear highlight in the entire menu
void View::clear_highlight_in_menu() const
{
    int number_of_rows_in_menu = 3;
    int first_column = 0;
    for (int i = 0; i<number_of_rows_in_menu; ++i) {
        mvwchgat(menu_window_ptr, i+1, first_column, -1, A_NORMAL, 0, NULL);
    }
}

int View::choose_plan_to_change(int number_of_plans) const
{
    //enable using special keyboard keys (e.g. arrows)
    keypad(plans_window_ptr, true);
    int key_pressed_by_user;
    int highlighted_plan = 0;
    int first_row = 3; //first row in plans window that contains plans not headers
    int column_width = 141; //number of columns that should be highlighted
    int first_column = 0; //first column in a row

    while (true) {
        //find plan that should be highlighted
        for (int i = 0; i<number_of_plans; ++i) {
            if (i==highlighted_plan) {
                mvwchgat(plans_window_ptr, highlighted_plan+first_row, first_column, column_width, A_REVERSE, 0, NULL);
            }
        }
        key_pressed_by_user = wgetch(plans_window_ptr);

        //check whether a user pressed an arrow or enter key
        switch (key_pressed_by_user) {
        case KEY_UP:
            highlighted_plan--;
            if (highlighted_plan==-1) {
                highlighted_plan = 0;

            }
            //before highlight the next plan, uncheck previous selected one
            mvwchgat(plans_window_ptr, first_row+highlighted_plan+1, first_column, -1, A_NORMAL, 0, NULL);
            break;
        case KEY_DOWN:
            highlighted_plan++;
            if (highlighted_plan==number_of_plans) {
                highlighted_plan = number_of_plans-1;

            }
            //before highlight the next plan, uncheck previous selected one
            mvwchgat(plans_window_ptr, first_row+highlighted_plan-1, first_column, -1, A_NORMAL, 0, NULL);
            break;
        default:
            break;
        }

        //if user pressed enter get the highlighted menu option
        if (key_pressed_by_user==( int )'\n') {
            break;
        }
    }

    //clear highlight in the entire plans window
    for (int j = 0; j<number_of_plans; ++j) {
        mvwchgat(plans_window_ptr, first_row+j, first_column, -1, A_NORMAL, 0, NULL);
    }

    keypad(plans_window_ptr, false);
    return highlighted_plan;
}

