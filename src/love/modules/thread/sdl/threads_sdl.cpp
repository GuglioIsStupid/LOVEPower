#include "../threads_love.h"
#include "../Thread.h"
#include <SDL/SDL.h>

namespace love {
namespace thread {
namespace sdl1 {

// Derived Mutex for SDL1
struct SDLMutex : public Mutex {
    SDL_mutex* m;

    SDLMutex() { m = SDL_CreateMutex(); }
    ~SDLMutex() override { SDL_DestroyMutex(m); }

    void lock() override { SDL_mutexP(m); }
    void unlock() override { SDL_mutexV(m); }
};

// Derived Conditional for SDL1
struct SDLConditional : public Conditional {
    SDL_cond* c;

    SDLConditional() { c = SDL_CreateCond(); }
    ~SDLConditional() override { SDL_DestroyCond(c); }

    void signal() override { SDL_CondSignal(c); }
    void broadcast() override { SDL_CondBroadcast(c); }

    bool wait(Mutex* mutex, int timeout=-1) override {
        if (!mutex) return false;
        SDL_mutex* mtx = static_cast<SDLMutex*>(mutex)->m;
        if (timeout < 0) return SDL_CondWait(c, mtx) == 0;
        else return SDL_CondWaitTimeout(c, mtx, timeout) == 0;
    }
};

// Thread wrapper for SDL1
struct SDLThread : public Thread {
    SDL_Thread* thread;
    Threadable* t;

    SDLThread(Threadable* t) : thread(nullptr), t(t) {}\
    ~SDLThread() override {}

    bool start() override {
        thread = SDL_CreateThread(thread_runner, t);
        return thread != nullptr;
    }

    void wait() override {
        if(thread) SDL_WaitThread(thread, nullptr);
    }

    bool isRunning() override {
        return thread != nullptr;
    }

private:
    static int thread_runner(void* data) {
        Threadable* t = static_cast<Threadable*>(data);
        t->threadFunction();
        return 0;
    }
};

} // namespace sdl1

// Factory functions
Mutex* newMutex() { return new sdl1::SDLMutex(); }
Conditional* newConditional() { return new sdl1::SDLConditional(); }
Thread* newThread(Threadable* t) { return new sdl1::SDLThread(t); }

} // namespace thread
} // namespace love
