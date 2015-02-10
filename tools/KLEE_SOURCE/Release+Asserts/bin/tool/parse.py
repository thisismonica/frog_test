import sys
import os.path
import subprocess
import re

#####################################################
# Function Definition Block
#####################################################

def isNumber(s):
        try:
                int(s)
                return True
        except ValueError:
                pass

        try:
                import unicodedata
                unicodedata.numeric(s)
                return True
        except (TypeError, ValueError):
                pass

        return False

################################
# 1 - Parse argument
################################

if len(sys.argv) < 2:
	print "Missing argument. Exit."
	sys.exit()	

target = sys.argv[1]

# Check if file exists
if not os.path.isfile(target):
	print "File does not exist. Exit."
	sys.exit()

# Astyle parse
print "\n1. Astyle target file: "+target
subprocess.call("astyle --style=allman "+target,shell=True)

func = []
kleeFunc = []

################################
# 2 -Grap functions 
################################

with open(target, 'r') as f:

	# Extract all functions using RegExp
	print "\n2. Extract all functions"
	for line in f:
		reg = re.match(r'^\w+\s+\w+\s*\(.*\)\s*$', line, re.M|re.I)
		if reg:
			func.append(reg.group())
			print line[0:-1] 

################################
# 3 - Screen functions
################################

print "\n3. List functions that does not take void as arg"
for item in func:
	reg = re.search(r'(\(\s+(.*)\s+\))', item, re.M|re.I)
	if reg:
		#print reg.group(2)+" - "+item[0:-1]
		if reg.group(2).strip().lower() != "void":
			kleeFunc.append(item[0:-1].strip())
			#print item[0:-1]

# If there is no function to test, exit
if len(kleeFunc) == 0:
	print "No matched function found."
	sys.exit()

# Ask for which function to test
for i,func in enumerate(kleeFunc):
	print str(i)+". "+func

i = raw_input("\nFunction id: ")

while(not isNumber(i) or int(i)<0 or int(i)>len(kleeFunc)):
	print "Invalid Input."
	i = raw_input("\nFunction id: ")

argType = []
funcType = ""
funcName = ""

################################
# 4 - func type, name & arg type
################################
print "\n4. Extracting func type, name and argument"
reg = re.search(r'(\w+)\s+(.*)\(\s+(.*)\s+\)\s*$', kleeFunc[int(i)], re.M|re.I)
if reg:
	funcType = reg.group(1).strip()
	funcName = reg.group(2).strip()
	for pair in reg.group(3).split(','):
		argTypeArr = pair.strip().split(' ')
		if len(argTypeArr) == 2:
			argType.append(argTypeArr[0].strip())
		else:
			argTypeStr = argTypeArr[0]+argTypeArr[1]
			argType.append(argTypeStr.strip())

print funcType
print funcName
print argType

################################
# 5 - Create test file
################################

testFileName = target+".test.c"
testFileObject = target+".test.o"
print "\n5. Creating tmp file - "+testFileName
subprocess.call("cp "+target+" "+testFileName, shell=True)

# Test if creation is successful
if not os.path.isfile(testFileName):
	print "Test file is not created successfully."
	sys.exit()

# Append Main Function
with open(testFileName, "ab") as f:
	#appendCode = funcType[i]+" main() {\n"
	appendCode = "#include \"klee.h\"\n"
	appendCode += "#include \"ansi_prefix.PPCEABI.bare.h\"\n"
	appendCode += "OS_FUN * OS_System_List;\n"
	appendCode += "volatile UNSIGNED TMD_System_Clock;\n"
	appendCode += "int main() {\n"
	for i,atype in enumerate(argType):
		symbol = "a"+str(i)
		appendCode += "\t"+atype+" "+symbol+";\n"
		appendCode += "\tklee_make_symbolic(&"+symbol+",sizeof("+symbol+"),\""+symbol+"\");\n"
	if funcType.lower() == "void":
		appendCode += "\t"+funcName+"("
	else:
		appendCode += "\t"+funcType+" result="+funcName+"("
	for i,atype in enumerate(argType):
		symbol = "a"+str(i)
		appendCode += symbol
		if i < len(argType)-1:
			appendCode += ","
	appendCode += ");\n"
	appendCode += "\treturn 0;\n"
	appendCode += "}"
	
	print appendCode

	f.write(appendCode)

################################
# 6 - Compile to object file
################################

print "\n6. LLVM linking"
linkAddr = []
linkAddr.append("/home/kuan/Downloads/Xuji-files-list")
#linkAddr.append("/usr/include/x86_64-linux-gnu/")
#linkAddr.append("/home/tianyu/Desktop/myplat/Nuc2Plat/")

linkCmd = "llvm-gcc --emit-llvm -c -g"
for addr in linkAddr:
	linkCmd += " -I"+addr
linkCmd += " "+testFileName

#linkCmd += " && echo llvm-link -o tptc.o "+testFileObject

# Delete old object
if os.path.isfile(testFileObject):
	subprocess.call("rm "+testFileObject,shell=True)

# Double check on llvm command
print linkCmd
raw_input("\nMake sure that above command is correct. Press Enter to continue...\n")

returncode = subprocess.call(linkCmd, shell=True)
print "return code: "+str(returncode)
if returncode != 0:
	print "Compilation failed."
	sys.exit() 

################################
# 7 - Klee run
################################

print "\n7. KLEE run"
if os.path.isfile(testFileObject):
	returncode = subprocess.call("klee "+testFileObject, shell=True)
	print "return code: "+str(returncode)
	if returncode != 0:
		print "Klee failed."
		sys.exit() 
else:
	print "Object file does not exist."

