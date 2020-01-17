#include <iostream>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "Plan.h"

Plan::Plan(std::string description, std::string category)
        :description {std::move(description)}, category {std::move(category)}
{
}

Plan::Plan(int id, std::string description, std::string category, bool done)
        :id {id}, description {std::move(description)}, category {std::move(category)}, done {done}
{
}

std::string Plan::print() const
{
    int id_length = 5;
    int desc_length = 102;
    int cat_length = 28;
    int done_length = 5;

    std::stringstream buffer;
    buffer
            << std::setw(id_length)
            << std::left
            << id
            << std::setw(desc_length)
            << std::left
            << description
            << std::setw(cat_length)
            << std::left
            << category
            << std::setw(done_length)
            << std::left
            << std::boolalpha
            << done;
    return buffer.str();
}

void Plan::set_description(const std::string& description)
{
    Plan::description = description;
}

void Plan::set_category(const std::string& category)
{
    Plan::category = category;
}

int Plan::get_id() const
{
    return id;
}

const std::string& Plan::get_description() const
{
    return description;
}

const std::string& Plan::get_category() const
{
    return category;
}

void Plan::set_done(bool done)
{
    Plan::done = done;
}

bool Plan::is_done() const
{
    return done;
}

