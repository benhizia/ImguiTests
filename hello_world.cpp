// Part of ImGui Bundle - MIT License - Copyright (c) 2022-2024 Pascal Thomet - https://github.com/pthom/imgui_bundle
#include "immapp/immapp.h"
#ifdef IMGUI_BUNDLE_WITH_IMPLOT
#include "implot/implot.h"
#endif
#include "imgui_md_wrapper.h"
#include "node.h"
#include <cmath>

// Function to render the tree view
void RenderTreeView(std::shared_ptr<Node> node) {
    if (ImGui::TreeNode(node->name.c_str())) {
        for (auto& child : node->children) {
            RenderTreeView(child);
        }
        ImGui::TreePop();
    }
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


void Gui()
{

    // Create a simple tree structure
    auto root = std::make_shared<Node>("Root");
    auto child1 = std::make_shared<Node>("Child 1");
    auto child2 = std::make_shared<Node>("Child 2");
    auto grandchild1 = std::make_shared<Node>("Grandchild 1");

    child1->AddChild(grandchild1);
    root->AddChild(child1);
    root->AddChild(child2);

    // Render the tree view (this should be inside your rendering loop)
    RenderTreeView(root);
}


int main(int , char *[])
{
    HelloImGui::SimpleRunnerParams runnnerParams;
    runnnerParams.guiFunction = Gui;
    runnnerParams.windowSize = {600, 1200};

    ImmApp::AddOnsParams addOnsParams;
    addOnsParams.withMarkdown = true;
    addOnsParams.withImplot = true;

    ImmApp::Run(runnnerParams, addOnsParams);
    return 0;
}
