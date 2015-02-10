import sys
import subprocess
import re
import glob
import os
import threading
import time 
from support_frog import compute

#####################################################
# Pre Configuration
#####################################################
pathToKleeRoot = "/home/qirong/Desktop/KLEE_SOURCE"   # Path to klee root directory, for replay test cases
kleeInclude = "tools/KLEE_SOURCE/include/klee"   # Path to include files, for llvm compilation
KLEE_TIMEOUT = 10 
MAX_TESTS = 10

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
	reg = re.search(r'(\(\s*(.*)\s*\))', item, re.M|re.I)
	if reg:
		#print reg.group(2)+" - "+item[0:-1]
		if reg.group(2).strip().lower() != "void" and reg.group(2)!="":
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

targetFunc = kleeFunc[int(i)]

argType = []
funcType = ""
funcName = ""
argName = []
argIsPointer = []

################################
# 4 - func type, name & arg type
################################
print "\n4. Extracting func type, name and argument"
reg = re.search(r'(\w+)\s+(.*)\(\s*(.*)\s*\)\s*$', kleeFunc[int(i)], re.M|re.I)
p_pointer_var1 = re.compile(r'^\*.*')
p_pointer_var2 = re.compile(r'.*\[\]$')
p_pointer_type = re.compile(r'.*\*$')

if reg:
	funcType = reg.group(1).strip()
	funcName = reg.group(2).strip()
	for pair in reg.group(3).split(','):
		argArr = pair.strip().split(' ')
		if len(argArr) == 2:

			# Check if argument is pointer type
				# Case 1: variable begin with *
			if p_pointer_var1.match(argArr[1]):
				argType.append(argArr[0].strip())
				argIsPointer.append(True)
				name = argArr[1].strip().replace('*','') 
				argName.append( name )

				# Case 2: variable end with []
			elif p_pointer_var2.match(argArr[1]):
				argType.append(argArr[0].strip())
				argIsPointer.append(True)
				name = argArr[1].strip().replace('[]','') 
				argName.append(name)

				# Case 3: type end with *
			elif p_pointer_type.match(argArr[0]):
				arg_type = argArr[0].strip().replace('*','')
				argType.append(arg_type)
				argIsPointer.append(True)
				argName.append(argArr[1].strip())

				# Case 4: not pointer type
			else:
				argType.append( argArr[0].strip() )
				argIsPointer.append(False)
				argName.append(argArr[1].strip())

		# Case 5: pointer * in the middle
		elif len(argTypeArr)==3 and argArr[1]=="*":
			argType.append(argArr[0].strip())
			argIsPointer.append(True)
			argName.append(argArr[2].strip())

		else:
			argTypeStr = argArr[0]+argArr[1]
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
	# Inital include files and declaration
	appendCode = "\n#include \"klee.h\"\n"
	appendCode += "#include \"ansi_prefix.PPCEABI.bare.h\"\n"
	appendCode += "int main() {\n"

	# Add symbolic functions
	for i,atype in enumerate(argType):
		symbol = "a"+str(i)
		if argIsPointer[i]:
			# User input for symbolize size of pointer type argument
			# Example: char a0[size];
			print "Argument ",argName[i]," is pointer of type: ",atype
			while True:
				size = raw_input("   Please enter size of unit to symbolize: ")
				try:
					size = int(size)
				except ValueError:
					print "Error: Invalid input, Only accept integer"
					pass
				else:
					if size >0:
						break
			appendCode += "\t"+atype+" "+symbol+"["+str(size)+"];\n"

			# Symbolize 
			# Example: klee_make_symbolic(a0, sizeof(a0), "a0")
			appendCode += "\tklee_make_symbolic("+symbol+",sizeof("+symbol+"),\""+symbol+"\");\n"

			# Add string NULL terminater
			# Example: klee_assume(a0[size-1]=='\0');
			if argType[i]=="char":
				appendCode += "\tklee_assume("+symbol+"["+str(size-1)+"]==\'\\0\');\n"
		else:			
			appendCode += "\t"+atype+" "+symbol+";\n"
			appendCode += "\tklee_make_symbolic(&"+symbol+",sizeof("+symbol+"),\""+symbol+"\");\n"

	# Call target function
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
	appendCode += "\treturn;\n"
	appendCode += "}"
	
	print appendCode

	f.write(appendCode)

################################
# 6 - Compile to object file
################################

print "\n6. LLVM linking"
linkAddr = []
linkAddr.append(kleeInclude)

linkCmd = "llvm-gcc --emit-llvm -c -g"
for addr in linkAddr:
	linkCmd += " -I"+addr
linkCmd += " "+testFileName

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
raw_input()

################################
# 7 - Klee run
################################

print "\n7. KLEE run"
if os.path.isfile(testFileObject):
	
	# Set klee timeout
	proc = subprocess.Popen(["klee",testFileObject])
	start = time.time()
	timeout = KLEE_TIMEOUT 
	while proc.poll() is None:
		time.sleep(0.1)
		now = time.time()
		if (now-start) > timeout:
		        print "KLEE timeout..."
			returncode = proc.returncode 
			proc.kill()
	returncode = proc.returncode

	#returncode = subprocess.call("klee "+testFileObject, shell=True)
	print "return code: "+str(returncode)
	if returncode != 0:
		print "Klee failed."
		sys.exit() 
else:
	print "Object file does not exist."
raw_input()

################################
# 8 - Replaying test cases
################################

print "\n8. Replaying test cases"

# Gather testcases file for read
temp_tests = "temp_klee_test_cases.txt" # temporary file to store test case values
if os.path.isfile(temp_tests):
	subprocess.call('rm '+temp_tests, shell=True)
tests = glob.glob('klee-last/test*.ktest')

# Build Regex to extract argument value
p = re.compile(r'^object\s*(\d+):\sdata:\s(.*)\n$')
testnum = 0

# Build Regex to match gcov uncovered line
p_gcov = re.compile('^\s+#####')

# Initialize matrix for Fault Localization
F = []
M = []

for test in tests[:MAX_TESTS]:

	# Display test case values, write to temp file klee_tests
	subprocess.call('ktest-tool --write-ints '+test+' >'+temp_tests, shell=True)
	print "***Test case "+str(testnum)+" for function: ", targetFunc					
	testnum += 1
	argcount = 0
	argval = []

	# Find test case value for each argument
	with open(temp_tests,'r') as f:
		for line in f:
			a = p.findall(line)	
			# If match one parameter value
			if a:
				try:
					argstr = argName[ int(a[0][0]) ]+" = " +a[0][1]
					print argstr
					argval.append(a[0][1])
					argcount += 1
				except Exception:
					print "Invalid test case input"

	# Judge if argument number valid
	if argcount != len(argName):
		print "ERROR: Invalid argument number of only ",argcount
		print "Press Enter to Continue..."
		raw_input()
		continue	

	# Build replay test file
	replayFileName = target + '.replay.c'
	if os.path.isfile(replayFileName):
		subprocess.call('rm '+replayFileName, shell=True)
	subprocess.call('cp '+target+' '+replayFileName, shell=True)

	with open(replayFileName, 'ab') as f:
		appendCode = "\n#include <stdio.h>\n"
		appendCode += "int main(){\n"
		if funcType.lower()=='void':
			appendCode+='\t'+funcName+'('
		else:
			appendCode+='\t'+funcType+' returnval='+funcName+'('
		for i,a in enumerate(argval):

			# For string type argument, change single quote to double quote
			if argIsPointer[i]:
				a = re.sub("\'",'\"',a)
				
			appendCode+=a
			if i< len(argval)-1:
				appendCode+=', '
		appendCode+=');\n'
		if funcType.lower() =='void':
			appendCode+='\treturn;\n}'
		else:
			#FIXME print result is not only integer
			appendCode+=r'    printf("Return value is %d\n",returnval);'
			
			appendCode+='\n\treturn;\n}'
		f.write(appendCode)
	
	# Compile test file
	replayOutput = target + '.replay'
	returnval = subprocess.call('gcc -w -fprofile-arcs -ftest-coverage '+replayFileName+' -o '+replayOutput, shell=True)
	if returnval != 0:
		print 'ERROR: gcc compile for repalying failed, press ENTER to continue...'
		raw_input()
		continue
	
	# Run test file
	returnCode = subprocess.call('./'+replayOutput, shell=True)
	if returnCode == 0:
		print "ERROR: replaying test case failed, press ENTER to continue..."
		raw_input()
		continue

	# Wait for user input to judge fail(F)/pass(P)
	print '**Test case pass? [y/n]:  '
	f = raw_input()

	# Write to Matrix F[]
	while f.lower()!='y' and f.lower()!='n':
		print "Invalid input, please enter \'y\' if pass, \'n\' if fail\n"
		f = raw_input()

	if f.lower()=='y':
		F.append(False)
	elif f.lower()=='n':
		F.append(True)
	
	# Get coverage
	returnval = subprocess.call('gcov '+replayFileName, shell=True)
	if returnval != 0:
		print 'ERROR: gcov failed, press ENTER to continue...'
		raw_input()
		continue

	# RE for not covered lines, marked with"#####" in gcov
	gcovFileName = replayFileName+'.gcov'
	lineCoverage = []
	
	# Read gcov file, write to coverage matrix M
	with open(gcovFileName,'r') as f:
		for i,line in enumerate(f):
			# ignore first 5 lines, not source code
			if i < 5:
				continue
			if p_gcov.match(line) :
				lineCoverage.append(False)
			else:
				lineCoverage.append(True)
				
	M.append(lineCoverage)

################################
# 9 - Running tarantula
################################

# Set 'Live' (valid) test case, true if valid
testNum = len(F)
L = [True]*testNum

# Set 'Coverage' statement, true if the statement is coverage
stmtNum = len(M[0])
C = [True]*stmtNum

suspiciousness = compute(M,F,L,C)

print suspiciousness
