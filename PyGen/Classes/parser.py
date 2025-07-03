from pygccxml import utils
from pygccxml import declarations
from pygccxml import parser
import Classes.TreeNode as Tree
import os
def parse_cpp_file(filename):
    # Find the location of the castxml binary
    generator_path, generator_name = utils.find_xml_generator()

    # Path to MSVC compiler (cl.exe)
    compiler_path = r"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.37.32822\bin\Hostx86\x86\cl.exe"
    
    # Configure the parser
    xml_generator_config = parser.xml_generator_configuration_t(
        xml_generator_path=generator_path,
        include_paths=[r"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.37.32822\include", r"C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\ucrt" ],
        xml_generator=generator_name,
        compiler_path=compiler_path,
        cflags="-std=c++17"  # Adjust this to match your C++ standard
    )
    # Parse the C++ file
    file_full_path = os.path.abspath(filename)
    decls = parser.parse([file_full_path], xml_generator_config)
    # Get the global namespace
    # here we must filter out the namespaces that are not needed
    # using a gui to be built later
    global_ns = declarations.get_global_namespace(decls)
    # Find all user-defined classes
    user_classes = global_ns.classes(header_file = file_full_path)
    return user_classes

def create_tree_from_cpp_for_class(ns, class_name):
    def create_node(decl):
        node = Tree.TreeNode(decl.name, cpp_object=decl)
        if isinstance(decl, declarations.class_t):
            for member in decl.public_members:
                if isinstance(member, (declarations.class_t, declarations.variable_t, declarations.free_function_t, declarations.member_function_t)):
                    child_node = create_node(member)
                    node.children.append(child_node)
        return node

    root_nodes = []
    # adding a dummy node as title to the tree and for correct spacing and aligning with the table header row
    dummy_node = Tree.TreeNode("C++ Hierarchy Viewer")
    dummy_node.is_printable_in_table_view = False   # This is to avoid printing the dummy node in the table view
    root_nodes.append(dummy_node)
    # Create a list of class names from ns.declarations
    class_names = [decl.name for decl in ns.declarations]
    if class_name in class_names:
        for decl in ns.declarations:
            if decl.name == class_name:
                root_nodes.append(create_node(decl))
    return root_nodes

