// Copyright © 2022 Santiago Goldaraz. All rights reserved.
#pragma once

#include <gsmenu/gspanel.hpp>

class GSMenu
{
    public:
        GSMenu(GSCommonGraph* lcd) : m_lcd(lcd) {}

        void init(GSPanel* panel)
        {
            m_panel_handler.set_next_panel(panel);
        }

        void update()
        {
            m_panel_handler.update(*m_lcd);
        }

        void event(GSPanel::EventType event)
        {
            m_panel_handler.event(event);
        }

    private:
        GSPanelHandler m_panel_handler;
        GSCommonGraph* m_lcd;
};

