/*****************************************************************************
Copyright (c) 2024  Alberto Mardegan (mardy@users.sourceforge.net)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. Neither the name of copyright holders nor the names of its
   contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL COPYRIGHT HOLDERS OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES

#define _GNU_SOURCE

#include <GL/gl.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** The SDL window */
static SDL_Window * window = NULL;
/** The SDL OpenGL context */
static SDL_GLContext gl_context = NULL;
static SDL_bool done = SDL_FALSE;
static SDL_GameController *controller = NULL;

static GLuint color_tex;
static GLuint fbo;

static void handle_event(SDL_Event *event)
{
    switch (event->type) {
    case SDL_CONTROLLERBUTTONDOWN:
        switch (event->cbutton.button) {
        case SDL_CONTROLLER_BUTTON_START:
        case SDL_CONTROLLER_BUTTON_BACK:
            done = SDL_TRUE;
            break;
        }
        break;
    case SDL_QUIT:
        done = SDL_TRUE;
        break;
    case SDL_JOYDEVICEADDED:
        /* Of course, we should dispose the old one, etc etc :-) */
        controller = SDL_GameControllerOpen(event->jdevice.which);
        break;
    }
}

static void triangle(void)
{
    glBegin(GL_TRIANGLES);
    glVertex2d(-1.0,-1.0);
    glVertex2d(+1.0,-1.0);
    glVertex2d(+0.0,+1.0);
    glEnd();
}

static void square(void)
{
    glBegin(GL_QUADS);
    glTexCoord2d(0.0,0.0); glVertex2d(-1.0,-1.0);
    glTexCoord2d(1.0,0.0); glVertex2d(+1.0,-1.0);
    glTexCoord2d(1.0,1.0); glVertex2d(+1.0,+1.0);
    glTexCoord2d(0.0,1.0); glVertex2d(-1.0,+1.0);
    glEnd();
}

static void draw_frame(void)
{
    float t = SDL_GetTicks() / 1000.0;

    float angle = t * 20;

    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    glClearColor(0.0, 0.3, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(1.0, 0.0, 0.0, sinf(t) * 0.5f + 0.5f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, 256, 256);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(angle, 0.0f, 0.0f, 1.0f);

    glDisable(GL_TEXTURE_2D);
    glColor3f(0.0, 1.0, 0.0);
    triangle();

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat left = -1.0f, right = 1.0f;
    GLfloat bottom = -1.0f, top = 1.0f;
    GLfloat zNear = 1.0f, zFar = 100.0f;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(left, right, bottom, top, zNear, zFar);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, color_tex);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -5.0f);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    square();
}

int main(int argc, char **argv)
{
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_GAMECONTROLLER) != 0) {
        SDL_Log("Unable to initialize SDL video subsystem: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow("FBO example",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              640, 480, SDL_WINDOW_OPENGL);
    if (!window) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    gl_context = SDL_GL_CreateContext(window);
    if (gl_context == NULL) {
        SDL_Log("Unable to create GL context: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    glGenTextures(1, &color_tex);
    glBindTexture(GL_TEXTURE_2D, color_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    /* NULL means reserve texture memory, but texels are undefined */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 256, 256, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    /* Attach 2D texture to this FBO */
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, color_tex, 0);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (status) {
    case GL_FRAMEBUFFER_COMPLETE:
        break;
    default:
        SDL_Log("Framebuffer not complete, status = %04x", status);
        return EXIT_FAILURE;
    }

    /* Bind the default framebuffer again */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            handle_event(&event);
        }

        draw_frame();
        SDL_GL_SwapWindow(window);
    }

    return EXIT_SUCCESS;
}