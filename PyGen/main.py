import imgui
import OpenGL.GL as gl
from imgui.integrations.glfw import GlfwRenderer
import glfw
#locals
import Classes.parser as parser
import Classes.render_tree as renderer

def main():
    if not glfw.init():
        return

    window = glfw.create_window(1200, 800, "C++ Hierarchy Viewer", None, None)
    if not window:
        glfw.terminate()
        return

    glfw.make_context_current(window)

    imgui.create_context()
    impl = GlfwRenderer(window)

    # Parse C++ file and create tree
    cpp_file = r"Data\tst.h"  # Replace with your C++ header file path
    global_ns = parser.parse_cpp_file(cpp_file)
    tree = parser.create_tree_from_cpp(global_ns)

    while not glfw.window_should_close(window):
        glfw.poll_events()
        impl.process_inputs()

        imgui.new_frame()

        imgui.set_next_window_size(1180, 780)
        imgui.set_next_window_position(10, 10)
        imgui.begin("C++ Hierarchy Viewer", flags=imgui.WINDOW_NO_RESIZE | imgui.WINDOW_NO_MOVE)
        renderer.render_tree_and_table(tree)
        imgui.begin_group()
        #EasyWidgetExample()
        imgui.end_group()
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