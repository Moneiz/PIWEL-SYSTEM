#ifndef __WIDGET_H
#define __WIDGET_H

#include <common/types.h>
#include <common/graphicscontext.h>
#include <drivers/keyboard.h>

namespace gui{
    class Widget : public drivers::KeyboardEventHandler{
        protected:
        Widget* parent;
        common::int32_t x;
        common::int32_t y;
        common::int32_t w;
        common::int32_t h;

        common::uint8_t r;
        common::uint8_t g;
        common::uint8_t b;
        bool Focussable;
        public:

        Widget(Widget* parent,
            common::int32_t x,
            common::int32_t y,
            common::int32_t w,
            common::int32_t h,

            common::uint8_t r,
            common::uint8_t g,
            common::uint8_t b);
        ~Widget();

        virtual void GetFocus(Widget* widget);
        virtual void ModelToScreen(common::int32_t &x, common::int32_t &y);
        virtual bool ContainsCoordinate(common::int32_t x, common::int32_t y);

        virtual void Draw(common::GraphicsContext* gc);
        virtual void OnMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button);
        virtual void OnMouseUp(common::int32_t x, common::int32_t y, common::uint8_t button);
        virtual void OnMouseMove(common::int32_t oldx, common::int32_t oldy,common::int32_t x, common::int32_t y);

    };
    class CompositeWidget : public Widget{
        private :
        Widget* children[100];
        int numChildren;
        Widget* focussedChild;
        
        public:
        CompositeWidget(Widget* parent,
            common::int32_t x,
            common::int32_t y,
            common::int32_t w,
            common::int32_t h,

            common::uint8_t r,
            common::uint8_t g,
            common::uint8_t b);
        ~CompositeWidget();


        virtual bool AddChild(Widget* child);
        virtual void GetFocus(Widget* widget);

        virtual void Draw(common::GraphicsContext* gc);
        virtual void OnMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button);
        virtual void OnMouseUp(common::int32_t x, common::int32_t y, common::uint8_t button);
        virtual void OnMouseMove(common::int32_t oldx, common::int32_t oldy,common::int32_t x, common::int32_t y);
        virtual void OnKeyDown(char);
        virtual void OnKeyUp(char);

    };
}

#endif