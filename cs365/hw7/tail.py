import sys

# Nicholas Creech
# CS365 HW7
# 04/01/21

# Make sure user provides 2 args
if len(sys.argv) != 3:
	print('usage: python tail.py <file> <lines>')
	sys.exit()

# Print last n lines of file
with open(sys.argv[1], 'r') as f:
	n = int(sys.argv[2])
	for line in f.readlines()[-n:]:
		print(line, end = '')		
