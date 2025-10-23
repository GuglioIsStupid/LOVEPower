#include "common/version.h"
#include "common/runtime.h"
#include "common/Variant.h"
#include "modules/love/love.h"
#include <SDL.h>

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

#include <vector>

template class std::vector<std::string>;

static void get_app_arguments(int argc, char** argv, int &new_argc, char** &new_argv)
{
    // guh
    return;
}

static int love_preload(void*, void*, const char *name)
{
    return 0;
}

enum DoneAction
{
    DONE_QUIT,
    DONE_RESTART
};

static DoneAction runlove(int argc, char** argv, int &retval)
{
    retval = 0;
    return DONE_QUIT;
}

int main(int argc, char** argv)
{
    // guh

    int retval = 0;
    DoneAction done = DONE_QUIT;

    do
    {
        done = runlove(argc, argv, retval);
    } while (done == DONE_RESTART);

    return retval;
}
