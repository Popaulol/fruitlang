import os
from pathlib import Path

with open("out.dot", "w") as out:
    out.write("digraph inheritance {\n")
    for path in Path('.').rglob('*.h'):
        print(path)
        with open(path, "r") as f:
            for line in f.readlines():
                if "class" not in line:
                    continue
                if "friend" in line:
                    continue
                line = line.strip().split()
                class_name = ""
                parents = []
                is_enum = False
                for i, tok in enumerate(line):
                    if tok == "enum":
                        is_enum = True
                    if tok == "class":
                        class_name = line[i + 1]
                    if tok == "public" or tok == "protected" or tok == "private":
                        parents.append(line[i + 1].strip(","))
                out.write(f"{class_name} [color={'red' if is_enum else 'black'}];\n")
                for child in parents:
                    out.write(f"{class_name} -> {child};\n")

    out.write("}\n")

os.system("dot -Ksfdp -Tsvg -O out.dot")
