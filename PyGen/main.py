import imgui
import OpenGL.GL as gl
from imgui.integrations.glfw import GlfwRenderer
import glfw
#locals
import Classes.parser as parser
import Classes.render_tree as renderer
from pygccxml import declarations

def main():
    # Parse C++ file and create tree
    # python parts
    cpp_file = r"Data\tst.h"  # Replace with your C++ header file path
    user_classes = parser.parse_cpp_file(cpp_file)

    if not glfw.init():
        return

    window = glfw.create_window(1200, 800, "C++ Hierarchy Viewer", None, None)

    if not window:
        glfw.terminate()
        return

    glfw.make_context_current(window)
    imgui.create_context()
    impl = GlfwRenderer(window)

    imgui.set_next_window_size(1180, 780)
    imgui.set_next_window_position(1000, 10)
    selected_class_index = 0
    tree = None
    _last_chosen_class_name = ""
    while not glfw.window_should_close(window):
        glfw.poll_events()
        impl.process_inputs()
        imgui.new_frame()

        # Create a window
        imgui.begin("Select Main Class")
        # Create a combo box to select the main class
        class_names = [decl.name for decl in user_classes.declarations]
        _, selected_class_index = imgui.combo("Main Class", selected_class_index, class_names)

        # Display the selected class
        imgui.text(f"Selected Class: {class_names[selected_class_index]}")
        chosen_class_name = class_names[selected_class_index]
        imgui.end()
        if _last_chosen_class_name != chosen_class_name :
            tree = parser.create_tree_from_cpp_for_class(user_classes, chosen_class_name)
        _last_chosen_class_name = chosen_class_name
        # Create a window
        imgui.begin("C++ Hierarchy Viewer window")
        # Render tree and table in c++ generated code
        # because of the padding bytes in the structs that needs to be the same on both sides of the compiler 
        if tree is not None:
            # the code of render_tree_and_table below must be generated in c++ the python code is just a way of using pygccxml to generate the tree node structure
            renderer.render_tree_and_table(tree)
        imgui.end()

        gl.glClearColor(0.1, 0.1, 0.1, 1)
        gl.glClear(gl.GL_COLOR_BUFFER_BIT)

        imgui.render()
        impl.render(imgui.get_draw_data())
        glfw.swap_buffers(window)

    impl.shutdown()
    glfw.terminate()

if __name__ == "__main__":
    main()