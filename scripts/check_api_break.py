import subprocess
import sys
from lxml import etree

def collect_names(root):
    """Collect all names and mark if they are WIP."""
    names = {}
    for enum in root.findall(".//enum"):
        names[enum.get("name")] = enum.find("wip") is not None
        for entry in enum.findall("entry"):
            names[entry.get("name")] = entry.find("wip") is not None
    for msg in root.findall(".//message"):
        is_wip = msg.find("wip") is not None
        names[msg.get("name")] = is_wip
        for field in msg.findall("field"):
            names[field.get("name")] = is_wip or field.find("wip") is not None
    return names

def get_base_commit():
    return subprocess.check_output(
        ["git", "merge-base", "origin/master", "HEAD"], text=True
    ).strip()

def get_changed_xml_files(base):
    changed = subprocess.check_output(
        ["git", "diff", "--name-only", base], text=True
    ).splitlines()
    return [f for f in changed if f.endswith(".xml")]

def parse_xml(content):
    return etree.fromstring(content.encode() if isinstance(content, str) else content)

def main():
    base = get_base_commit()
    xml_files = get_changed_xml_files(base)
    if not xml_files:
        print("No XML files changed.")
        return

    for xml in xml_files:
        if xml.endswith("development.xml"):
            continue

        try:
            old_content = subprocess.check_output(
                ["git", "show", f"{base}:{xml}"], text=True
            )
            new_content = open(xml).read()
        except subprocess.CalledProcessError:
            continue # new file or removed, ignore

        old_root = parse_xml(old_content)
        new_root = parse_xml(new_content)

        old_names = collect_names(old_root)
        new_names = collect_names(new_root)

        print(old_names)
        print(new_names)

        removed = [n for n in old_names if n not in new_names]
        added = [n for n in new_names if n not in old_names]

        renamed = []
        for r in removed:
            # allow if old item was WIP
            if old_names.get(r):
                continue
            renamed.append(r)
        for a in added:
            # allow if new item is WIP
            if new_names.get(a):
                continue
            renamed.append(a)

        if renamed:
            print(f"Name changes detected in {xml}:")
            for name in renamed:
                print(f" - {name}")
            sys.exit(1)

    print("No name changes detected.")

if __name__ == "__main__":
    main()
