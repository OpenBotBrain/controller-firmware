// Copyright © 2022 Santiago Goldaraz. All rights reserved.
#pragma once

#include <cstdint>

#include <gsmenu/gspanel.hpp>
#include <gscope/gscope.hpp>
#include <algorithm>

class GSListPanel : public GSPanel
{
    public:
        struct ContextList
        {
            const char* name;
            GSPanel* panel;
        };

        GSListPanel(const char* name, const ContextList* list, int size) : GSPanel(),
            m_menu_name(name), m_context_list(list), m_context_elements(size) { }

        virtual void on_startup(GSCommonGraph& lcd) override
        {
            m_selected_index = 0;
            m_next_index = 0;
            m_select_item = false;
            m_sub_item_start = 0;

            lcd.cp437(true);
            draw_menu(lcd);
        }

        virtual void on_shutdown(GSCommonGraph&) override
        {

        }

        virtual GSPanel* update(GSCommonGraph& lcd) override
        {
            GSPanel* next = nullptr;

            if (m_selected_index != m_next_index)
            {
                m_selected_index = m_next_index;
                draw_menu(lcd);
            }

            // Check if Enter is selected, then change to new panel
            if (m_select_item)
            {
                m_select_item = false;
                if (m_context_list[m_selected_index].panel != nullptr)
                {
                    next = m_context_list[m_selected_index].panel;
                }
                else
                {
                    GSDebug("index %d does not have a panel associated", m_selected_index);
                }
            }

            return next;
        }

        void event(EventType event) override
        {
            switch (event)
            {
                default:

                    break;
                case GSPanel::EventType::MOVE_DOWN:
                {
                    int next = m_selected_index + 1;
                    if (next >= m_context_elements)
                    {
                        next = 0;
                    }
                    m_next_index = next;
                    break;
                }
                case GSPanel::EventType::MOVE_UP:
                {
                    int next = m_selected_index - 1;
                    if (next < 0)
                    {
                        next = m_context_elements - 1;
                    }
                    m_next_index = next;
                    break;
                }
                    break;
                case GSPanel::EventType::ENTER:
                    m_select_item = true;
                    break;
            }
        }

    private:
        const char* m_menu_name;
        const ContextList* m_context_list;
        const int m_context_elements;

        int m_selected_index;
        int m_next_index;
        bool m_select_item;
        int m_sub_item_start;

        static constexpr uint8_t MAIN_MENU_SIZE_HEIGHT = 16;
        static constexpr uint8_t SUBMENU_START_HEIGHT = 20;
        static constexpr uint8_t TEXT_SEPARATION_HEIGHT = 10;

        void draw_menu(GSCommonGraph& lcd)
        {
            lcd.clearDisplay();
            lcd.fillRect(0, 0, lcd.width(), MAIN_MENU_SIZE_HEIGHT, GSCommonGraph::Color::WHITE);

            // Write the top menue name
            lcd.setTextSize(1);
            lcd.setTextColor(GSCommonGraph::Color::BLACK);
            lcd.setCursor(3,3);     // Start at top-left corner

            lcd.printText(m_menu_name);

            // Write the options
            int drop_menue_size = lcd.height() - SUBMENU_START_HEIGHT;
            int max_item = drop_menue_size % TEXT_SEPARATION_HEIGHT;

            int sub_item_start = std::max(m_sub_item_start , 1 + m_selected_index - max_item);
            if (sub_item_start > m_selected_index)
            {
                sub_item_start = m_selected_index;
            }
            int sub_item_stop = sub_item_start + max_item;

            m_sub_item_start = sub_item_start;

            lcd.setTextSize(1);
            lcd.setTextColor(GSCommonGraph::Color::WHITE);
            int sub_item_height = SUBMENU_START_HEIGHT;

            for (int i = sub_item_start; i < sub_item_stop; i++)
            {
                if (i < m_context_elements)
                {
                    const char* marker = (i == m_selected_index) ? "->" : "  ";
                    lcd.setCursor(2, sub_item_height);
                    lcd.printText(marker);
                    lcd.printText(m_context_list[i].name);

                    sub_item_height += TEXT_SEPARATION_HEIGHT;
                }
            }

            lcd.update();
        }
};