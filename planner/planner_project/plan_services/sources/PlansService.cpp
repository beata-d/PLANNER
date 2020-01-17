#include "Plan.h"
#include "PlansService.h"
#include "View.h"


void PlansService::create_plan(View& view)
{
    description = view.get_description_from_user();
    category = view.get_category_from_user();
}

//mark a plan as done or uncheck if you make a mistake
void PlansService::mark_done(Plan& plan)
{
    plan.set_done(!plan.is_done());
}

//change description or category of a plan
void PlansService::update_plan(Plan& plan, View& view)
{
    description = view.get_description_from_user();
    if (description.length()>0) {
        plan.set_description(description);
    }
    category = view.get_category_from_user();
    if (category.length()>0) {
        plan.set_category(category);
    }
}

//clear a vector that contains plans before reading new ones
void PlansService::read_plans(View&, std::vector<std::unique_ptr<Plan>>& plans)
{
    plans.clear();
}

const std::string& PlansService::get_description() const
{
    return description;
}

const std::string& PlansService::get_category() const
{
    return category;
}

void PlansService::remove_plan(const Plan&)
{

}



