from pygccxml import utils
from pygccxml import declarations
from pygccxml import parser
import Classes.TreeNode as Tree

def parse_cpp_file(filename):
    # Find the location of the castxml binary
    generator_path, generator_name = utils.find_xml_generator()

    # Path to MSVC compiler (cl.exe)
    compiler_path = r"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.37.32822\bin\Hostx86\x86\cl.exe"
    
    # Configure the parser
    xml_generator_config = parser.xml_generator_configuration_t(
        xml_generator_path=generator_path,
        xml_generator=generator_name,
        compiler_path=compiler_path,
        cflags="-std=c++17"  # Adjust this to match your C++ standard
    )

    # Parse the C++ file
    decls = parser.parse([filename], xml_generator_config)

    # Get the global namespace
    global_ns = declarations.get_global_namespace(decls)

    return global_ns

def create_tree_from_cpp(ns):
    def create_node(decl):
        node = Tree.TreeNode(decl.name, cpp_object=decl)
        if isinstance(decl, declarations.class_t):
            for member in decl.public_members:
                if isinstance(member, (declarations.class_t, declarations.variable_t, declarations.free_function_t, declarations.member_function_t)):
                    child_node = create_node(member)
                    node.children.append(child_node)
        return node

    root_nodes = []
    for decl in ns.declarations:
        if isinstance(decl, declarations.class_t):
            root_nodes.append(create_node(decl))

    return root_nodes