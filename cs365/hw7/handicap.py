import sys
import datetime
from golfer import *

# Nicholas Creech
# CS365 HW7
# 04/01/21

golfers = {}
courses = {}

# User must supply two files
if len(sys.argv) != 3:
	print('usage: python handicap.py <score-file> <course-file>')
	sys.exit() 

# Read courses from course file
with open(sys.argv[2], 'r') as f:
	newCourse = Course()
	for line in f:
		fields = line.strip().split()
		if len(fields) == 0:
			continue
		if fields[0] == 'Course':
			newCourse.name = ' '.join(fields[1:])
		elif fields[0] == 'Rating':
			newCourse.rating = float(fields[1])
			newCourse.slope = float(fields[3])
			courses[newCourse.name] = newCourse
			newCourse = Course()

# Read scores from score file
with open(sys.argv[1], 'r') as f:
	for line in f:
		month = 0
		day = 0
		year = 0
		newGolfer = Golfer()
		newScore = Score()

		for i, v in enumerate(line.strip().split()):
			if i == 0:
				month = int(v)
			elif i == 1:
				day = int(v)
			elif i == 2:
				year = int(v)
			elif i == 3:
				if not (v in golfers):
					newGolfer.name = v
				else:
					newGolfer = golfers[v]
			elif i == 4:
				newScore.value = int(v)
				
		date = datetime.datetime(year, month, day)
		newScore.course = courses[' '.join(line.strip().split()[5:])]
		newScore.date = date
		newGolfer.scores.append(newScore)
		golfers[newGolfer.name] = newGolfer


# Calculate handicap for each golfer
for key, val in golfers.items():
	val.calcHandicap()

# Print each golfer and their handicap
for val in sorted(golfers.values(), key = lambda golfer: golfer.handicap):
	print('{:5.2f} {:s}'.format(val.handicap, val.name))
