#pragma once
#include <vector>
#include <string>
#include <memory>

struct Node {
    std::string name;
    std::vector<std::shared_ptr<Node>> children;

    Node(const std::string& name) : name(name) {}
    void AddChild(std::shared_ptr<Node> child) {
        children.push_back(child);
    }
};
