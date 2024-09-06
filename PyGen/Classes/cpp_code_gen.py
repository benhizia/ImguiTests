import os
import sys
from pygccxml import utils
from pygccxml import declarations
from pygccxml import parser

# Configuration
xml_generator_path = r"X:\GitRepos\castxml-windows\castxml\bin\castxml.exe"
xml_generator = parser.castxml.CastXMLGenerator(xml_generator_path)

# Parse the C++ file
cpp_file = "../Tests/example_struct.hpp"
declarations = parser.parse([cpp_file], xml_generator)
global_namespace = declarations[0]

def generate_imgui_code(cls):
    code = []
    code.append(f"void render_{cls.name}(const {cls.name}& obj) {{")
    code.append(f"    if (ImGui::TreeNode(\"{cls.name}\")) {{")
    
    for member in cls.variables():
        member_name = member.name
        member_type = member.type.decl_string
        
        code.append(f"        ImGui::Text(\"{member_name}: {member_type}\");")
        
        if isinstance(member.type, declarations.declarated_t):
            code.append(f"        render_{member.type.declaration.name}(obj.{member_name});")
        elif isinstance(member.type, declarations.array_t):
            code.append(f"        if (ImGui::TreeNode(\"{member_name}\")) {{")
            code.append(f"            for (int i = 0; i < {member.type.size}; ++i) {{")
            code.append(f"                ImGui::Text(\"[%d]\", i);")
            code.append(f"                render_{member.type.base.declaration.name}(obj.{member_name}[i]);")
            code.append(f"            }}")
            code.append(f"            ImGui::TreePop();")
            code.append(f"        }}")
    
    code.append("        ImGui::TreePop();")
    code.append("    }")
    code.append("}")
    return "\n".join(code)

# Generate code for all classes
generated_code = []
for cls in global_namespace.classes():
    generated_code.append(generate_imgui_code(cls))

# Write generated code to file
with open("generated_imgui_code.cpp", "w") as f:
    f.write("\n\n".join(generated_code))

print("Code generation complete. Output written to generated_imgui_code.cpp")