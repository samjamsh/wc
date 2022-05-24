lines, characters, = "",""
try:
    import sys;
    sys.exit(f"python3 {sys.argv[0]} file") if len(sys.argv) == 1 else print("",end=""); 
    filename = sys.argv[1:];
    for file in filename:
        with open(file,"rb") as file_:
            fileread=file_.read()
            lines = fileread.count("\n".encode("ascii")); 
            characters = len(fileread);
            print(f"o arquivo '{file}' tem {lines} linhas e {characters} caracteres")

except FileNotFoundError:
    import sys
    print("File not found")

finally:      
   pass
