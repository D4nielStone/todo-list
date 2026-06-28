#pragma once
#include <bgui.hpp>
#include <memory>
#include <iostream>
#include "api/task.hpp"
#include <string>

struct task {
    int id;
    std::string title;
    bool completed;
};

typedef std::unordered_map<int, bgui::checkbox*> taskmap;

class tlb {
private:
    taskmap g_task_map;
    tlb() = default;
public:
    taskmap& get_taskmap() {
        return g_task_map;
    }
    static tlb& get_instance() {
        static tlb todo_list;
        return todo_list;
    }
    static void create_task(const std::string& title) {
        if (title.empty()) return;
        static int task_id = 0; task_id++;
        bool completed = false;

        auto& task = bgui::get_layout().add<bgui::checkbox>(nullptr, completed, title, 0.35f);
        task.get_label().style.layout.require_mode(bgui::mode::stretch, bgui::mode::wrap_content);
        task.get_label().style.layout.align = bgui::vec<2Ul, bgui::alignment>{bgui::alignment::start, bgui::alignment::center};
        get_instance().get_taskmap()[task_id] = &task;
        
        auto& close = task.add<bgui::button>("Remove", 0.3f, [&task](){
            get_instance().get_taskmap().erase(task_id);
            bgui::get_layout().remove(&task);
        });
        close.style.layout.require_mode(bgui::mode::wrap_content, bgui::mode::stretch);
        close.style.visual.border.normal = 0.0f;
        close.style.visual.background.normal = 0.0f;
    }
    void setup_interface() {
        bgui::style_manager::get_instance().apply_theme(bgui::dark_theme());

        // The main page
        auto& root = bgui::get_layout();
        root.style.layout.require_mode(bgui::mode::match_parent, bgui::mode::match_parent);
        root.style.layout.padding = bgui::vec4i{5, 5, 5, 5};
        auto& top_div = root.add<bgui::linear>(bgui::orientation::vertical);
        top_div.style = {
            .layout = {
                .size_mode = std::make_optional<bgui::vec<2Ul, bgui::mode>>({bgui::mode::match_parent, bgui::mode::wrap_content}),
            },
            .visual = {
                .background = {
                    .normal = bgui::vec4{0.06, 0.06, 0.06, 1.f}
                },
                .visible = true
            }
        };
        top_div.add<bgui::text>("Todo List", 1.f).style.layout.require_mode(bgui::mode::stretch, bgui::mode::wrap_content);
        auto& input_div = top_div.add<bgui::linear>(bgui::orientation::horizontal);
        input_div.style.layout.require_mode(bgui::mode::match_parent, bgui::mode::wrap_content);
        input_div.style.layout.set_padding(5, 5);
        auto& ia = input_div.add<bgui::input_area>("", 0.35f, create_task, "Task label here");
        auto& btn = input_div.add<bgui::button>("Add", 0.35f, [&ia](){
            std::string title = ia.get_buffer();
            if (!title.empty()) {
                create_task(title);
                ia.set_buffer("");
            }
        });
        btn.style.layout.require_mode(bgui::mode::wrap_content, bgui::mode::wrap_content);
        btn.style.visual.border.normal = 0.0f;
        btn.style.visual.background.normal = 0.1f;
    }
};