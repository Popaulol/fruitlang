from __future__ import annotations

from dataclasses import dataclass, field
from pprint import pprint

keywords = {
    "if": "if_",
    "else": "else_",

    "for": "for_",
    "while": "while_",
    "do": "do_",

    "mod": "mod",

    "fn": "fn",
    "proc": "proc",
}


@dataclass
class TreePart:
    set_chars: str
    key: str
    children: dict[str, TreePart] = field(default_factory=dict)

    def render_dot(self, f):
        newline = '\\n'
        f.write(f"{'node_' + self.set_chars} [label=\"{'Set Chard: ' + self.set_chars + newline + 'Key:' + self.key}\"];\n")

        for k, child in self.children.items():
            child.render_dot(f)
            f.write(f"{'node_' + self.set_chars} -> {'node_' + child.set_chars} [label=\"{k}\"];\n")

    def generate_c(self, f):
        if self.key:
            f.write(f"return {{TokenType::{self.key}, text, current_pos - 1}};\n")
        if len(self.set_chars) != 0:
            f.write(f"if (text.length() <= {len(self.set_chars)}) goto bail;\n")
        f.write(f"switch (text.at({len(self.set_chars)})) {{\n")
        for k, child in self.children.items():
            f.write(f"case '{k}':\n")
            child.generate_c(f)
            f.write("break;\n")
        f.write("}\n")


root = TreePart("", "")

for keyword, key in keywords.items():
    curr = root
    for char in keyword:
        nxt = curr.children.get(char, TreePart(curr.set_chars + char, ""))
        curr.children[char] = nxt
        curr = nxt
        if nxt.set_chars == keyword:
            nxt.key = key

pprint(root)

with open("keywords.dot", "w") as f:
    f.write("digraph {\n")
    root.render_dot(f)
    f.write("}")

with open("keywords.inc", "w") as f:
    root.generate_c(f)
    f.write("bail: \n")
