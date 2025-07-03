#include <imgui.h>
#include <vector>
#include <string>
#include "aris.h"

void render_treeviewx(int n)
{
    // 
    ImGuiTextFilter     Filter;
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_F, ImGuiInputFlags_Tooltip);
    ImGui::PushItemFlag(ImGuiItemFlags_NoNavDefaultFocus, true);
    if (ImGui::InputTextWithHint("##Filter", "incl,-excl", Filter.InputBuf, IM_ARRAYSIZE(Filter.InputBuf), ImGuiInputTextFlags_EscapeClearsAll))
        Filter.Build();
    ImGui::PopItemFlag();

    // Using those as a base value to create width/height that are factor of the size of our font
    const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
    const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

    static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;
    static ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_SpanTextWidth|ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_DefaultOpen;
        
    if (ImGui::BeginTable("3ways", 3, flags))
    {
        // The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
        ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
        ImGui::TableHeadersRow();

        // Simple storage to output a dummy file-system.
        struct MyTreeNode
        {
            const char* Name;
            const char* Type;
            int             Size;
            int             ChildIdx;
            int             ChildCount;
            static void DisplayNode(const MyTreeNode* node, const MyTreeNode* all_nodes, const ImGuiTextFilter& Filter)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                const bool is_folder = (node->ChildCount > 0);
                if (is_folder)
                {
                    bool open = ImGui::TreeNodeEx(node->Name, tree_node_flags);
                    ImGui::TableNextColumn();
                    ImGui::TextDisabled("--");
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(node->Type);
                    if (open)
                    {
                        for (int child_n = 0; child_n < node->ChildCount; child_n++)
                            if (Filter.PassFilter(node->Name)) // Filter root node
                                DisplayNode(&all_nodes[node->ChildIdx + child_n], all_nodes, Filter);
                        ImGui::TreePop();
                    }
                }
                else
                {
                    ImGui::TreeNodeEx(node->Name, tree_node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen);
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", node->Size);
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(node->Type);
                }
            }
        };
        static const MyTreeNode nodes[] =
        {
            { "Root",                         "Folder",       -1,       1, 3    }, // 0
            { "Music",                        "Folder",       -1,       4, 2    }, // 1
            { "Textures",                     "Folder",       -1,       6, 3    }, // 2
            { "desktop.ini",                  "System file",  1024,    -1,-1    }, // 3
            { "File1_a.wav",                  "Audio file",   123000,  -1,-1    }, // 4
            { "File1_b.wav",                  "Audio file",   456000,  -1,-1    }, // 5
            { "Image001.png",                 "Image file",   203128,  -1,-1    }, // 6
            { "Copy of Image001.png",         "Image file",   203256,  -1,-1    }, // 7
            { "Copy of Image001 (Final2).png","Image file",   203512,  -1,-1    }, // 8
        };

        MyTreeNode::DisplayNode(&nodes[0], nodes, Filter);

        ImGui::EndTable();
    }
}


void render_my_app()
{
    static ImVector<int> active_tabs;
    static int next_tab_id = 0;
    if (next_tab_id == 0) // Initialize with some default tabs
        for (int i = 0; i < 3; i++)
            active_tabs.push_back(next_tab_id++);

    static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable |
                                            ImGuiTabBarFlags_FittingPolicyResizeDown|
                                            ImGuiTabBarFlags_TabListPopupButton |
                                            ImGuiTabBarFlags_NoCloseWithMiddleMouseButton
                                            ;
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
    {
        // Submit our regular tabs
        for (int n = 0; n < active_tabs.Size; n++)
        {
            bool open = true;
            char name[16];
            snprintf(name, IM_ARRAYSIZE(name), "%04d", active_tabs[n]);
            if (ImGui::BeginTabItem(name, nullptr, ImGuiTabItemFlags_None))
            {
                render_treeviewx(n);
                ImGui::EndTabItem();
            }
        }

        ImGui::EndTabBar();
    }
}
