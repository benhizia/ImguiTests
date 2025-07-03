#include <imgui.h>
#include <vector>
#include <string>
#include "demo.h"
#include <implot/implot.h>
#include <imgui_md_wrapper.h>

void render_table_view(const std::vector<std::pair<Node*, int>>& visible_nodes, float row_height) {
    ImGui::BeginChild("table_view");

    if (ImGui::BeginTable("details_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50);  // Offset by 50 pixels

        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Type");
        ImGui::TableSetupColumn("Offset");
        ImGui::TableSetupColumn("Size");
        ImGui::TableHeadersRow();

        for (const auto& [node, depth] : visible_nodes) {
            if (!node->is_printable_in_table_view)
                continue;

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);  // Offset by 2 pixels
            ImGui::TableNextRow(ImGuiTableRowFlags_None, row_height);
            ImGui::TableNextColumn();

            ImGui::Dummy(ImVec2(depth * 20, 0));  // Add indent to align with tree view
            ImGui::SameLine();
            ImGui::Text("%s", node->name.c_str());

            ImGui::TableNextColumn();
            if (node->cpp_object.type == "class") {
                ImGui::Text("Class");
                ImGui::TableNextColumn();
                ImGui::Text("N/A");
                ImGui::TableNextColumn();
                ImGui::Text("%zu bytes", node->cpp_object.byte_size);
            }
            else if (node->cpp_object.type == "variable") {
                ImGui::Text("%s", node->cpp_object.type.c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%zu bytes", node->cpp_object.byte_offset);
                ImGui::TableNextColumn();
                ImGui::Text("-");
            }
            else if (node->cpp_object.type == "function") {
                ImGui::Text("Function");
                ImGui::TableNextColumn();
                ImGui::Text("-");
                ImGui::TableNextColumn();
                ImGui::Text("-");
            }
        }

        ImGui::EndTable();
    }
    ImGui::EndChild();
}

void render_node(Node* node, int depth, std::vector<std::pair<Node*, int>>& visible_nodes) {

    ImGuiTreeNodeFlags flags = node->children.empty() ? ImGuiSelectableFlags_AllowDoubleClick : ImGuiSelectableFlags_AllowDoubleClick;
    flags |= ImGuiSelectableFlags_None;
    ImGui::PushID(node->id);

    ImGui::BeginGroup();
    float indent = depth * 20;  // Adjust indent based on depth
    ImGui::Dummy(ImVec2(indent, 0));  // Add horizontal spacing
    ImGui::SameLine();

    // Checkbox for node checked status
    //ImGui::Checkbox("", &node->checked);
    //ImGui::SameLine();

    // Use selectable to handle selection and expansion
    bool is_selected = node->expanded;
    bool is_expanded = ImGui::Selectable(node->name.c_str(), is_selected, flags);
    ImGui::SetItemDefaultFocus();
    node->expanded = is_expanded;

    ImGui::EndGroup();

    if (is_expanded) {
        visible_nodes.push_back({ node, depth });
        if (!node->children.empty()) {
            for (Node* child : node->children) {
                render_node(child, depth + 1, visible_nodes);
            }
        }
    }
    else {
        visible_nodes.push_back({ node, depth });
    }

    ImGui::PopID();
}
void render_tree_and_table(std::vector<Node*>& nodes) {
    
    std::vector<std::pair<Node*, int>> visible_nodes;
    float row_height = ImGui::GetTextLineHeight() + 4;  // Add some padding

    // Render the tree view
    ImGui::BeginChild("tree_view", ImVec2(400, 0));  // Fixed width of 400 for tree view
    for (Node* node : nodes) {
        render_node(node, 0, visible_nodes);  // Start rendering from depth 0
    }
    ImGui::EndChild();

    ImGui::SameLine();  // Align table view next to tree view

    // Render the table view
    render_table_view(visible_nodes, row_height);
}

// Test data creation function
std::vector<Node*> create_test_data() {
    // Create nodes representing C++ classes, variables, and functions

    // Root Node: Class A
    Node* class_A = new Node{ 1, "Class A" };
    class_A->cpp_object = { "class", 128, 0 };

    // Child Node: Variable a (int)
    Node* variable_a = new Node{ 2, "int a" };
    variable_a->cpp_object = { "variable", 4, 0 };
    class_A->children.push_back(variable_a);

    // Child Node: Variable b (float)
    Node* variable_b = new Node{ 3, "float b" };
    variable_b->cpp_object = { "variable", 4, 4 };
    class_A->children.push_back(variable_b);

    // Child Node: Function f()
    Node* function_f = new Node{ 4, "void f()" };
    function_f->cpp_object = { "function", 0, 0 };
    class_A->children.push_back(function_f);

    // Root Node: Class B
    Node* class_B = new Node{ 5, "Class B" };
    class_B->cpp_object = { "class", 64, 0 };

    // Child Node: Variable x (double)
    Node* variable_x = new Node{ 6, "double x" };
    variable_x->cpp_object = { "variable", 8, 0 };
    class_B->children.push_back(variable_x);

    // Child Node: Function g()
    Node* function_g = new Node{ 7, "void g()" };
    function_g->cpp_object = { "function", 0, 0 };
    class_B->children.push_back(function_g);

    // Combine both classes in a root node structure
    std::vector<Node*> root_nodes = { class_A, class_B };
    return root_nodes;
}

#define HelpMarker 
void render_tree_and_selectable_example()
{
    if (ImGui::TreeNode("Advanced, with Selectable nodes"))
    {
        /*HelpMarker(
            "This is a more typical looking tree with selectable nodes.\n"
            "Click to select, CTRL+Click to toggle, click on arrows or double-click to open.");*/
        static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        static bool align_label_with_current_x_position = false;
        static bool test_drag_and_drop = false;
        ImGui::CheckboxFlags("ImGuiTreeNodeFlags_OpenOnArrow", &base_flags, ImGuiTreeNodeFlags_OpenOnArrow);
        ImGui::CheckboxFlags("ImGuiTreeNodeFlags_OpenOnDoubleClick", &base_flags, ImGuiTreeNodeFlags_OpenOnDoubleClick);
        ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanAvailWidth", &base_flags, ImGuiTreeNodeFlags_SpanAvailWidth); ImGui::SameLine(); HelpMarker("Extend hit area to all available width instead of allowing more items to be laid out after the node.");
        ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanFullWidth", &base_flags, ImGuiTreeNodeFlags_SpanFullWidth);
        ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanTextWidth", &base_flags, ImGuiTreeNodeFlags_SpanTextWidth); ImGui::SameLine(); HelpMarker("Reduce hit area to the text label and a bit of margin.");
        ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanAllColumns", &base_flags, ImGuiTreeNodeFlags_SpanAllColumns); ImGui::SameLine(); HelpMarker("For use in Tables only.");
        ImGui::CheckboxFlags("ImGuiTreeNodeFlags_AllowOverlap", &base_flags, ImGuiTreeNodeFlags_AllowOverlap);
        ImGui::CheckboxFlags("ImGuiTreeNodeFlags_Framed", &base_flags, ImGuiTreeNodeFlags_Framed); ImGui::SameLine(); HelpMarker("Draw frame with background (e.g. for CollapsingHeader)");
        ImGui::CheckboxFlags("ImGuiTreeNodeFlags_NavLeftJumpsBackHere", &base_flags, ImGuiTreeNodeFlags_NavLeftJumpsBackHere);
        ImGui::Checkbox("Align label with current X position", &align_label_with_current_x_position);
        ImGui::Checkbox("Test tree node as drag source", &test_drag_and_drop);
        ImGui::Text("Hello!");
        if (align_label_with_current_x_position)
            ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

        // 'selection_mask' is dumb representation of what may be user-side selection state.
        //  You may retain selection state inside or outside your objects in whatever format you see fit.
        // 'node_clicked' is temporary storage of what node we have clicked to process selection at the end
        /// of the loop. May be a pointer to your own node type, etc.
        static int selection_mask = (1 << 2);
        int node_clicked = -1;
        for (int i = 0; i < 6; i++)
        {
            // Disable the default "open on single-click behavior" + set Selected flag according to our selection.
            // To alter selection we use IsItemClicked() && !IsItemToggledOpen(), so clicking on an arrow doesn't alter selection.
            ImGuiTreeNodeFlags node_flags = base_flags;
            const bool is_selected = (selection_mask & (1 << i)) != 0;
            if (is_selected)
                node_flags |= ImGuiTreeNodeFlags_Selected;
            if (i < 3)
            {
                // Items 0..2 are Tree Node
                bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node %d", i);
                if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                    node_clicked = i;
                if (test_drag_and_drop && ImGui::BeginDragDropSource())
                {
                    ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
                    ImGui::Text("This is a drag and drop source");
                    ImGui::EndDragDropSource();
                }
                if (i == 2 && (base_flags & ImGuiTreeNodeFlags_SpanTextWidth))
                {
                    // Item 2 has an additional inline button to help demonstrate SpanTextWidth.
                    ImGui::SameLine();
                    if (ImGui::SmallButton("button")) {}
                }
                if (node_open)
                {
                    ImGui::BulletText("Blah blah\nBlah Blah");
                    ImGui::SameLine();
                    ImGui::SmallButton("Button");
                    ImGui::TreePop();
                }
            }
            else
            {
                // Items 3..5 are Tree Leaves
                // The only reason we use TreeNode at all is to allow selection of the leaf. Otherwise we can
                // use BulletText() or advance the cursor by GetTreeNodeToLabelSpacing() and call Text().
                node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
                ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Leaf %d", i);
                if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                    node_clicked = i;
                if (test_drag_and_drop && ImGui::BeginDragDropSource())
                {
                    ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
                    ImGui::Text("This is a drag and drop source");
                    ImGui::EndDragDropSource();
                }
            }
        }
        if (node_clicked != -1)
        {
            // Update selection state
            // (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
            if (ImGui::GetIO().KeyCtrl)
                selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
            else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
                selection_mask = (1 << node_clicked);           // Click to single-select
        }
        if (align_label_with_current_x_position)
            ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
        ImGui::TreePop();
    }
}
void render_tabs_example()
{
    if (ImGui::TreeNode("TabItemButton & Leading/Trailing flags"))
    {
        static ImVector<int> active_tabs;
        static int next_tab_id = 0;
        if (next_tab_id == 0) // Initialize with some default tabs
            for (int i = 0; i < 3; i++)
                active_tabs.push_back(next_tab_id++);

        // TabItemButton() and Leading/Trailing flags are distinct features which we will demo together.
        // (It is possible to submit regular tabs with Leading/Trailing flags, or TabItemButton tabs without Leading/Trailing flags...
        // but they tend to make more sense together)
        static bool show_leading_button = true;
        static bool show_trailing_button = true;
        ImGui::Checkbox("Show Leading TabItemButton()", &show_leading_button);
        ImGui::Checkbox("Show Trailing TabItemButton()", &show_trailing_button);

        // Expose some other flags which are useful to showcase how they interact with Leading/Trailing tabs
        static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyResizeDown;
        ImGui::CheckboxFlags("ImGuiTabBarFlags_TabListPopupButton", &tab_bar_flags, ImGuiTabBarFlags_TabListPopupButton);
        if (ImGui::CheckboxFlags("ImGuiTabBarFlags_FittingPolicyResizeDown", &tab_bar_flags, ImGuiTabBarFlags_FittingPolicyResizeDown))
            tab_bar_flags &= ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyResizeDown);
        if (ImGui::CheckboxFlags("ImGuiTabBarFlags_FittingPolicyScroll", &tab_bar_flags, ImGuiTabBarFlags_FittingPolicyScroll))
            tab_bar_flags &= ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyScroll);

        if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
        {
            // Demo a Leading TabItemButton(): click the "?" button to open a menu
            if (show_leading_button)
                if (ImGui::TabItemButton("?", ImGuiTabItemFlags_Leading | ImGuiTabItemFlags_NoTooltip))
                    ImGui::OpenPopup("MyHelpMenu");
            if (ImGui::BeginPopup("MyHelpMenu"))
            {
                ImGui::Selectable("Hello!");
                ImGui::EndPopup();
            }

            // Demo Trailing Tabs: click the "+" button to add a new tab.
            // (In your app you may want to use a font icon instead of the "+")
            // We submit it before the regular tabs, but thanks to the ImGuiTabItemFlags_Trailing flag it will always appear at the end.
            if (show_trailing_button)
                if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
                    active_tabs.push_back(next_tab_id++); // Add new tab

            // Submit our regular tabs
            for (int n = 0; n < active_tabs.Size; )
            {
                bool open = true;
                char name[16];
                snprintf(name, IM_ARRAYSIZE(name), "%04d", active_tabs[n]);
                if (ImGui::BeginTabItem(name, &open, ImGuiTabItemFlags_None))
                {
                    ImGui::Text("This is the %s tab!", name);
                    ImGui::EndTabItem();
                }

                if (!open)
                    active_tabs.erase(active_tabs.Data + n);
                else
                    n++;
            }

            ImGui::EndTabBar();
        }
        ImGui::Separator();
        ImGui::TreePop();
    }
}
void render_table_and_treeview()
{
    // Using those as a base value to create width/height that are factor of the size of our font
    const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
    const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
    if (ImGui::TreeNode("Tree view"))
    {
        static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

        static ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_SpanAllColumns;
        ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanFullWidth", &tree_node_flags, ImGuiTreeNodeFlags_SpanFullWidth);
        ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanTextWidth", &tree_node_flags, ImGuiTreeNodeFlags_SpanTextWidth);
        ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanAllColumns", &tree_node_flags, ImGuiTreeNodeFlags_SpanAllColumns);

        HelpMarker("See \"Columns flags\" section to configure how indentation is applied to individual columns.");
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
                static void DisplayNode(const MyTreeNode* node, const MyTreeNode* all_nodes)
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
                                DisplayNode(&all_nodes[node->ChildIdx + child_n], all_nodes);
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

            MyTreeNode::DisplayNode(&nodes[0], nodes);

            ImGui::EndTable();
        }
        ImGui::TreePop();
    }
}

ExampleTreeNode* ExampleTree_CreateNode(const char* name, int uid, ExampleTreeNode* parent)
{
    ExampleTreeNode* node = IM_NEW(ExampleTreeNode);
    snprintf(node->Name, IM_ARRAYSIZE(node->Name), "%s", name);
    node->UID = uid;
    node->Parent = parent;
    node->IndexInParent = parent ? (unsigned short)parent->Childs.Size : 0;
    if (parent)
        parent->Childs.push_back(node);
    return node;
}

// Create example tree data
// (this allocates _many_ more times than most other code in either Dear ImGui or others demo)
ExampleTreeNode* ExampleTree_CreateDemoTree()
{
    static const char* root_names[] = { "Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pear", "Pineapple", "Strawberry", "Watermelon" };
    const size_t NAME_MAX_LEN = sizeof(ExampleTreeNode::Name);
    char name_buf[NAME_MAX_LEN];
    int uid = 0;
    ExampleTreeNode* node_L0 = ExampleTree_CreateNode("<ROOT>", ++uid, NULL);
    const int root_items_multiplier = 2;
    for (int idx_L0 = 0; idx_L0 < IM_ARRAYSIZE(root_names) * root_items_multiplier; idx_L0++)
    {
        snprintf(name_buf, IM_ARRAYSIZE(name_buf), "%s %d", root_names[idx_L0 / root_items_multiplier], idx_L0 % root_items_multiplier);
        ExampleTreeNode* node_L1 = ExampleTree_CreateNode(name_buf, ++uid, node_L0);
        const int number_of_childs = (int)strlen(node_L1->Name);
        for (int idx_L1 = 0; idx_L1 < number_of_childs; idx_L1++)
        {
            snprintf(name_buf, IM_ARRAYSIZE(name_buf), "Child %d", idx_L1);
            ExampleTreeNode* node_L2 = ExampleTree_CreateNode(name_buf, ++uid, node_L1);
            node_L2->HasData = true;
            if (idx_L1 == 0)
            {
                snprintf(name_buf, IM_ARRAYSIZE(name_buf), "Sub-child %d", 0);
                ExampleTreeNode* node_L3 = ExampleTree_CreateNode(name_buf, ++uid, node_L2);
                node_L3->HasData = true;
            }
        }
    }
    return node_L0;

}
// Demonstrate creating a simple property editor.
void ShowExampleAppPropertyEditor(bool* p_open, ImGuiDemoWindowData* demo_data)
{
    ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Example: Property editor", p_open))
    {
        ImGui::End();
        return;
    }

    static ExampleAppPropertyEditor2 property_editor;
    if (demo_data->DemoTree == NULL)
        demo_data->DemoTree = ExampleTree_CreateDemoTree();
    property_editor.Draw(demo_data->DemoTree);

    ImGui::End();
}

void DemoImplot()
{
    static std::vector<double> x, y1, y2;
    if (x.empty())
    {
        double pi = 3.1415;
        for (int i = 0; i < 1000; ++i)
        {
            double x_ = pi * 4. * (double)i / 1000.;
            x.push_back(x_);
            y1.push_back(cos(x_));
            y2.push_back(sin(x_));
        }
    }

    ImGuiMd::Render("# This is the plot of _cosinus_ and *sinus*");
#ifdef IMGUI_BUNDLE_WITH_IMPLOT
    if (ImPlot::BeginPlot("Plot"))
    {
        ImPlot::PlotLine("y1", x.data(), y1.data(), (int)x.size());
        ImPlot::PlotLine("y2", x.data(), y2.data(), (int)x.size());
        ImPlot::EndPlot();
    }
#endif

}