#include "efefManager.h"

efef::manager* efef::manager::inst = nullptr;

const efef::manager* const efef::manager::instance()
{
    return inst;
}

void efef::manager::update()
{
}

efef::manager::manager()
{
}

efef::manager::~manager()
{
}
