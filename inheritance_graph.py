import itertools
import os
from pathlib import Path


def get_file_color(file_name: str) -> str:
    return (
        "gray" if "llvm" in file_name else
        "red" if "<" in file_name else
        "green" if file_name.endswith(".h") else
        "blue" if file_name.endswith(".cpp") else
        "pink"
    )


with open("out.dot", "w") as out:
    with open("includes.dot", "w") as includes:
        out.write("digraph inheritance {\n")
        includes.write("digraph includes {\n")
        for path in itertools.chain(Path('.').rglob('*.h'), Path(".").rglob("*.cpp")):
            print(path)
            with open(path, "r") as f:
                for line in f.readlines():
                    if "#include" in line:
                        #if "llvm" in line or "<" in line:
                        #    continue
                        name = line.replace("#include", "").strip().replace('"', "").replace("ast/", "").replace("../",
                                                                                                                 "").strip()
                        includes.write(
                            f'"{os.path.basename(path)}" [fillcolor={get_file_color(os.path.basename(path))}];\n')
                        includes.write(f'"{name}" [fillcolor={get_file_color(name)}];\n')
                        includes.write(f'"{os.path.basename(path)}" -> "{name}" [color="{get_file_color(name)}"];\n')
                        continue

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
                    class_name = class_name.replace(";", "")
                    out.write(f"{class_name} [color={'red' if is_enum else 'black'}];\n")
                    for child in parents:
                        out.write(f"{class_name} -> {child};\n")

        out.write("}\n")
        includes.write("}\n")

os.system("dot -Ksfdp -Tsvg -O out.dot")
os.system("dot -Kdot -Tsvg -O includes.dot")
