import os

def generate_UE_file(file_name, dir):
    
    if os.path.isfile(dir+"\\"+file_name+".h") or os.path.isfile(dir+"\\"+file_name+".cpp"):
        print("The file already exist")
        return

    header_file = open(dir+"\\"+file_name+".h", "w+")
    cpp_file = open(dir+"\\"+file_name+".cpp", "w+")
    wrtie_UE_header_file(header_file, file_name)
    write_UE_cpp_file(cpp_file, file_name)




def wrtie_UE_header_file(file, file_name):
    file.write("#pragma once\n\n")
    file.write("#include \"" + file_name+".generated.h\"\n\n")

def write_UE_cpp_file(file, file_name):
    file.write("#include \"" + file_name+".h\"\n")
    file.write("#include UE_INLINE_GENERATED_CPP_BY_NAME(" + file_name + ")")



if __name__ == "__main__":
    while(1):
        print("Dir: ")
        dir = input()
        if dir == "exit" or dir == "x":
            break
        while(1):
            print("FileName: ")
            file_name = input()
            if file_name == "exit" or file_name == "x":
                break
            generate_UE_file(file_name, dir)

