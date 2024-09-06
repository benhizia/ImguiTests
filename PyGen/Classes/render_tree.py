import imgui
from pygccxml import declarations

def render_tree_and_table(nodes):
    visible_nodes = []
    row_height = imgui.get_text_line_height() + 4  # Add some padding

    def render_node(node, depth=0):
        flags = imgui.TREE_NODE_DEFAULT_OPEN if node.children else imgui.TREE_NODE_LEAF
        
        imgui.push_id(node.id)
        
        # Start a new group to ensure consistent height
        imgui.begin_group()
        
        indent = depth * 20  # Adjust indent based on depth
        imgui.dummy(indent, 0)  # Add horizontal spacing
        imgui.same_line()
        
        changed, node.checked = imgui.checkbox("", node.checked)
        imgui.same_line()
        
        # Use selectable instead of tree_node for consistent height
        is_selected = node.expanded
        is_expanded = imgui.selectable(node.name, is_selected, flags)[1]
        imgui.set_item_default_focus()
        node.expanded = is_expanded
        
        imgui.end_group()
        
        if is_expanded:
            visible_nodes.append((node, depth))
            if node.children:
                for child in node.children:
                    render_node(child, depth + 1)
        else:
            visible_nodes.append((node, depth))
        
        imgui.pop_id()

    # Render tree view
    imgui.begin_child("tree_view", width=400)
    for node in nodes:
        render_node(node)
    imgui.end_child()

    imgui.same_line()
    render_table_view(visible_nodes, row_height)

def render_table_view(visible_nodes, row_height):
    imgui.begin_child("table_view")
    if imgui.begin_table("details_table", 4, imgui.TABLE_BORDERS | imgui.TABLE_ROW_BACKGROUND):
        imgui.table_setup_column("Name")
        imgui.table_setup_column("Type")
        imgui.table_setup_column("Offset")
        imgui.table_setup_column("Size")
        imgui.table_headers_row()

        for node, depth in visible_nodes:
            imgui.table_next_row(row_flags=imgui.TABLE_ROW_NONE, min_row_height=row_height)
            
            imgui.table_next_column()
            imgui.dummy(depth * 20, 0)  # Add indent to align with tree view
            imgui.same_line()
            imgui.text(node.name)
            
            imgui.table_next_column()
            if isinstance(node.cpp_object, declarations.class_t):
                imgui.text("Class")
                imgui.table_next_column()
                imgui.text("N/A")
                imgui.table_next_column()
                imgui.text(f"{node.cpp_object.byte_size} bytes")
            elif isinstance(node.cpp_object, declarations.variable_t):
                imgui.text(f"{node.cpp_object.decl_type.decl_string}")
                imgui.table_next_column()
                imgui.text(f"{node.cpp_object.byte_offset} bytes")
                imgui.table_next_column()
                imgui.text(f"-")
            elif isinstance(node.cpp_object, (declarations.free_function_t, declarations.member_function_t)):
                imgui.text("Function")
                imgui.table_next_column()
                imgui.text("-")
                imgui.table_next_column()
                imgui.text("-")

        imgui.end_table()
    imgui.end_child()