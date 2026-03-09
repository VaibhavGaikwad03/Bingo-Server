import os
import glob

files = glob.glob('include/handlers/*.h') + glob.glob('src/handlers/*.cpp')

for f in files:
    with open(f, 'r') as file:
        lines = file.readlines()
    
    out_lines = []
    for i, line in enumerate(lines):
        if line.strip() == '{':
            if len(out_lines) > 0:
                prev_line = out_lines[-1]
                # If prev line is entirely blank, go up further
                for p in reversed(out_lines):
                    if p.strip() != '':
                        indent = len(p) - len(p.lstrip())
                        out_lines.append((' ' * indent) + '{\n')
                        break
                else:
                    out_lines.append('{\n')
            else:
                out_lines.append(line)
        else:
            out_lines.append(line)
            
    with open(f, 'w') as file:
        file.writelines(out_lines)

print("Fixed indentation")
