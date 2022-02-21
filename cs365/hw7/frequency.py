import sys
import operator

# Nicholas Creech
# CS365 HW7
# 04/01/21

dict = {}

if len(sys.argv) != 2:
	print('usage: python frequency.py <file>')
	sys.exit()

# Create dictionary of words in file
with open(sys.argv[1]) as f:
	for line in f:
		fields = line.strip().split()
		for word in fields:
			if word in dict:
				dict[word] = dict[word] + 1
			else:
				dict[word] = 1

# Print key/value pairs in ascending order sorted by frequency first, name second
for pair in sorted(dict.items(), key = operator.itemgetter(1, 0)):
	print('{0[0]:<20s}{0[1]:>5d}'.format(pair))
