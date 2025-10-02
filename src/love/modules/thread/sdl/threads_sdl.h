#pragma once

#include "../threads_love.h"
#include "../Thread.h"
#include <SDL/SDL.h>

namespace love {
namespace thread {
namespace sdl1 {

// ------------------------------------------------------------------
// Mutex
// ------------------------------------------------------------------
struct SDLMutex : public Mutex {
    SDL_mutex* m;

    SDLMutex();
    ~SDLMutex() override;

    void lock() override;
    void unlock() override;
};

// ------------------------------------------------------------------
// Conditional
// ------------------------------------------------------------------
struct SDLConditional : public Conditional {
    SDL_cond* c;

    SDLConditional();
    ~SDLConditional() override;

    void signal() override;
    void broadcast() override;
    bool wait(Mutex* mutex, int timeout=-1) override;
};

// ------------------------------------------------------------------
// Thread
// ------------------------------------------------------------------
struct SDLThread : public Thread {
    SDL_Thread* thread;
    Threadable* t;

    SDLThread(Threadable* t);
    ~SDLThread() override;

    bool start() override;
    void wait() override;
    bool isRunning() override;

private:
    static int thread_runner(void* data);
};

// ------------------------------------------------------------------
// Factory functions
// ------------------------------------------------------------------
Mutex* newMutex();
Conditional* newConditional();
Thread* newThread(Threadable* t);

} // namespace sdl1
} // namespace thread
} // namespace love
