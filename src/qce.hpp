#pragma once

#include "main.hpp"

class Qce : public nanogui::Screen {

public:
    Qce();
    ~Qce();

    virtual void draw(NVGcontext *ctx);
    virtual void drawContents();
    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers);
    void setIntensity(const float intensity) { mIntensity = intensity; }
    void bindShader();

private:
    nanogui::GLShader mShader;
    float mIntensity;
};
