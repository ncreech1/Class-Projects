# Nicholas Creech
# CS365 HW7
# 04/01/21

class Score:
	
	def __init__(self):
		self.course = None
		self.date = None
		self.value = 0

class Course:
	
	def __init__(self):
		self.name = ''
		self.rating = 0.0
		self.slope = 0.0

class Golfer:

	def __init__(self):
		self.name = ''
		self.scores = []
		self.handicap = 0.0

	def calcHandicap(self):
		diffs = []
		average = 0
		
		# Sort scores by date in ascending order and retrieve most recent 20
		recentScores = sorted(self.scores, key = lambda score: (score.date.year, score.date.month, score.date.day))[-20:]
		
		# Create list of differentials
		for v in recentScores:
			diffs.append((v.value - v.course.rating) * 113 / v.course.slope)

		# Find average of 10 lowest differentials
		for d in sorted(diffs)[0:10]:
			average += d
		average /= 10

		# Calculate handicap
		self.handicap = 0.96 * average
