class TreeNode:
    def __init__(self, name, cpp_object=None, children=None, parent=None):
        self.name = name
        self.cpp_object = cpp_object
        self.children = children or []
        self.checked = False
        self.parent = parent
        self.id = self.generate_id()
        self.expanded = False
        self.is_printable_in_table_view = True

        for child in self.children:
            child.parent = self

    def generate_id(self):
        if self.parent is None:
            return self.name
        return f"{self.parent.id}/{self.name}"