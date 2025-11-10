#include "gldraw.h"
#include <GL/gl.h>



void GL::SetupOrtho()
{
    // Save and push matrices
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // Set up orthographic projection
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glOrtho(0, 1920, 1080, 0, -1, 1);

    // Disable depth test for 2D drawing
    glDisable(GL_DEPTH_TEST);

    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void GL::RestoreGL()
{
    // Restore matrices
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    // Re-enable depth test
    glEnable(GL_DEPTH_TEST);

    // Disable blending
    glDisable(GL_BLEND);
}

void GL::DrawFilledRect(float x, float y, float width, float height, const GLubyte color[3])
{
    glColor3ub(color[0], color[1], color[2]);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void GL::DrawOutline(float x, float y, float width, float height, float lineWidth, const GLubyte color[3])
{
    glLineWidth(lineWidth);
    glColor3ub(color[0], color[1], color[2]);

    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void GL::DrawRectangle(float x, float y, float width, float height, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);               // top-left
    glVertex2f(x + width, y);       // top-right
    glVertex2f(x + width, y + height); // bottom-right
    glVertex2f(x, y + height);      // bottom-left
    glEnd();
}