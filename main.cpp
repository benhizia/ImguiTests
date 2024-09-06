#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <GLFW/glfw3.h>

class Calculator {
public:
    int Add(int a, int b) {
        return a + b;
    }

    int Subtract(int a, int b) {
        return a - b;
    }

    int Multiply(int a, int b) {
        return a * b;
    }

    float Divide(int a, int b) {
        if(b == 0) {
            throw std::invalid_argument("Divide by zero exception");
        }
        
        return (float)a / (float)b;
    }
};

int main()
{
    Calculator calculator;

    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui Calculator", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    int a = 0, b = 0, result = 0;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Calculator");

        ImGui::InputInt("a", &a);
        ImGui::InputInt("b", &b);

        if (ImGui::Button("Add")) {
            result = calculator.Add(a, b);
        }
        ImGui::SameLine();
        if (ImGui::Button("Subtract")) {
            result = calculator.Subtract(a, b);
        }
        ImGui::SameLine();
        if (ImGui::Button("Multiply")) {
            result = calculator.Multiply(a, b);
        }
        ImGui::SameLine();
        if (ImGui::Button("Divide")) {
            try {
                result = calculator.Divide(a, b);
            } catch (std::invalid_argument&) {
                result = 0;
            }
        }

        ImGui::Text("Result: %d", result);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}