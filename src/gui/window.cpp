#include <gui/window.h>

using namespace common;

namespace gui{

    Window::Window(Widget* parent,int32_t x,int32_t y,
        int32_t w, int32_t h,
        uint8_t r, uint8_t g, uint8_t b)
        : CompositeWidget(parent,x,y,w,h,r,g,b){
            Dragging = false;
    }
    Window::~Window(){

    }
    void Window::OnMouseDown(int32_t x, int32_t y, uint8_t button){
        Dragging = button == 1;
        CompositeWidget::OnMouseDown(x,y,button);
    }
    void Window::OnMouseUp(int32_t x, int32_t y, uint8_t button){
        Dragging = false;
        CompositeWidget::OnMouseUp(x,y,button);
    }
    void Window::OnMouseMove(int32_t oldx, int32_t oldy, int32_t x,int32_t y){
        if(Dragging){
            this->x += x-oldx;
            this->y += y-oldy;
        }
        CompositeWidget::OnMouseMove(oldx, oldy, x,y);
    }

}