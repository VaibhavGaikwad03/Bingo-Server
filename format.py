import os
import glob
import re

files = glob.glob('include/handlers/*.h') + glob.glob('src/handlers/*.cpp')

for f in files:
    with open(f, 'r') as file:
        content = file.read()
    
    # Fix class definitions and functions (e.g. `class XYZ {` -> `class XYZ\n{`)
    content = re.sub(r'([^\n\{\}])\s*\{\s*\n', r'\1\n{\n', content)
    
    # Fix inline empty braces `) {}` -> `)\n    {\n    }`
    content = re.sub(r'\)\s*\{\}', r')\n    {\n    }', content)

    # Fix namespace definitions `namespace XYZ {` -> `namespace XYZ\n{`
    
    with open(f, 'w') as file:
        file.write(content)

print("Formatted to Allman Style")
