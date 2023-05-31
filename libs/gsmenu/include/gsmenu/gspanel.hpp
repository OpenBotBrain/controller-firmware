// Copyright © 2022 Santiago Goldaraz. All rights reserved.
#pragma once

#include <cstdint>
#include <ssd1306/gscommon-graph.hpp>

#define ELEMS(x)  (sizeof(x) / sizeof((x)[0]))

class GSPanel
{
    public:
        static constexpr uint32_t DEFAULT_UPDATE_TIME_MS = 25;

        enum class EventType : uint8_t
        {
            NONE = 0,
            MOVE_UP,
            MOVE_DOWN,
            ENTER,
            GO_BACK
        };

        GSPanel() {}

        virtual void on_startup(GSCommonGraph& lcd) = 0;
        virtual void on_shutdown(GSCommonGraph& lcd) = 0;
        virtual GSPanel* update(GSCommonGraph& lcd) = 0;
        virtual void event(EventType event) = 0;
};

class GSPanelHandler
{
    public:
        GSPanelHandler() {}

        void update(GSCommonGraph& lcd)
        {
            if (m_next_panel != m_actual_panel &&
                m_next_panel != nullptr)
            {
                if (m_actual_panel != nullptr)
                {
                    m_actual_panel->on_shutdown(lcd);
                }
                // Save last panel in case we want to return back
                m_last_panel = m_actual_panel;
                m_actual_panel = m_next_panel;
                m_actual_panel->on_startup(lcd);
            }

            if (m_actual_panel != nullptr)
            {
                m_next_panel = m_actual_panel->update(lcd);
            }
        }

        void event(GSPanel::EventType event)
        {
            if (m_actual_panel != nullptr)
            {
                m_actual_panel->event(event);
            }

            // Load last panel in case we want to go back
            if (event == GSPanel::EventType::GO_BACK)
            {
                m_next_panel = m_last_panel;
            }
        }

        void set_next_panel(GSPanel* next)
        {
            m_next_panel = next;
        }

    private:
        GSPanel* m_next_panel;
        GSPanel* m_actual_panel {nullptr};
        GSPanel* m_last_panel {nullptr};
};
