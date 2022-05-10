#include "efefManager.h"

efef::manager* efef::manager::inst = nullptr;

efef::manager* const efef::manager::instance()
{
    return inst;
}

void efef::manager::update()
{
    for (uint i = 0; i < sockets.size(); ++i)
        sockets[i]->update();
}

efef::manager::manager()
{
}

efef::manager::~manager()
{
}
