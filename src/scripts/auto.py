import os

max_width = 18
pattern = '''/**
 * Copyright (c) 2006-2025 LOVE Development Team
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 **/

namespace love
{{
    // [{0}]
    const unsigned char {1}[] = 
    {{
    {2}
    }}; // [{0}]
}} // love
'''

def tohex(c, counter):
    if counter % max_width == 0:
        return f"\n\t0x{ord(c):02x}, "
    return f"0x{ord(c):02x}, "

def auto(name):
    src = f"{name}.lua"
    dst = f"{name}.lua.h"
    cpp_name = f"{name}_lua"
    
    if not os.path.exists(src):
        print(f"{name}: Error - file not found")
        return

    with open(src, "rb") as src_file:
        out_data = ""
        counter = 0

        for line in src_file:
            line = line.decode('utf-8').strip()
            if line:
                counter = -1 
                out_data += ''.join([tohex(c, counter) for c in line])
                out_data += "0x0a,"
            else:
                out_data += "\n\t0x0a,"

    with open(dst, "wb") as out_file:
        out_file.write(pattern.format(src, cpp_name, out_data).encode('utf-8'))
    
    print(f"{name}: Success")

if __name__ == "__main__":
    if len(os.sys.argv) <= 1:
        print("Usage: python auto.py <name1> <name2> .. <nameN>")
        exit()

    for arg in os.sys.argv[1:]:
        name = arg.replace("scripts/", "").replace(".lua", "")
        
        if "/" in name:
            print(f"{name}: Error - not in scripts directory")
        else:
            try:
                auto(name)
            except Exception as e:
                print(f"{name}: Error - {str(e)}")
