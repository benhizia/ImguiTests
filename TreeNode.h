#pragma once
#include <string>
#include <vector>
#include <memory>

class TreeNode {
public:
    TreeNode(const std::string& name, void* cpp_object = nullptr,
        std::vector<std::shared_ptr<TreeNode>> children = {},
        TreeNode* parent = nullptr)
        : name(name), cpp_object(cpp_object), children(children),
        checked(false), parent(parent), expanded(false),
        is_printable_in_table_view(true) {

        id = generateId();

        for (auto& child : children) {
            child->parent = this;
        }
    }

    std::string generateId() {
        if (parent == nullptr) {
            return name;
        }
        return parent->id + "/" + name;
    }

    std::string name;
    void* cpp_object;
    std::vector<std::shared_ptr<TreeNode>> children;
    bool checked;
    TreeNode* parent;
    std::string id;
    bool expanded;
    bool is_printable_in_table_view;
};