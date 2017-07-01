import math
import random
import sys
import os

### CONFIG ###
### EDIT THIS PART BELOW ###
### GUI UPDATE: Encapsulated Main Script to function, to be called from GUI interface.
"""
For lack of UI, just set your desired parameters here. This obviously needs to be improved for the sake of a better workflow.
Edit: UI Added.
"""

gridsize = [16, 16, 16] # Use values no lower than 2. High values take longer to generate, and cost more memory.
minbounds = [-100.0, -100.0, -100.0] # bottom left back coordinate of the grid
maxbounds = [100, 100, 100] # top right front coordinate of the grid
filename = "VF_GridNorm.fga" # Desired filename. Make sure to include .fga. Files are currently overwritten without checks or confirmations, and output to the same directory this script was run from.
directionbias = [0, 0, 0] # To make vector fields generally point in a specific direction, define the direction here...
directionstrength = 0.0 # ...and use this parameter to scale it.
scalenoiseamount = 0.05 # This is used to give some variation to the generated vectors' lengths. A factor of 1.0 for example would mean that instead of length 1, output vectors get a random length between 0 and 2.
directionnoiseamount = 0 # Adds uniform unidirectional noise to the generator's result, for nicer swirlies.
mainscalefactor = 10 # Generated vectors are (or should be) within 0 - 1 units of length. This variable lets you scale them all uniformly.

generator = 3 # Which noise generator to use. See below:
"""
Currently implemented generators are:

0: Uniform				- Total chaos, great for turbulence.
1: UniformNormalized	- Same as above, but all vectors are the same length. 
2: Grid					- Some kind of alternating grid pattern. Not really great yet, but can gives some cool results when rotating quickly.
3: GridNormalized		- Same as above, but all vectors are the same length.
4: Radial				- All vectors point away from the center.
"""

### END CONFIG ###
### NO EDITING BELOW THIS LINE NECESSARY ###
### Unless you know what you're doing, of course. Be my guest! ###

### Definitions ###

# Write line to file
def writevector(file, vector):
	vec = [""] * 3
	for c in range(0, 3):
		if str(vector[c])[0] == "-":
			vec[c] = str(vector[c])[0:9]
		else:
			vec[c] = str(vector[c])[0:8]
		#vec[c] = str(vector[c])
	file.write(vec[2] + ", " + vec[1] + ", " + vec[0] + ",\n")

### Vector functions because apparently python doesn't have any built in, wtf.

# Normalize a vector
def normalizeVector(vector):
	mag = magnitudeVector(vector)
	return [vector[0] / max(mag, 0.0000000001), vector[1] / max(mag, 0.0000000001), vector[2] / max(mag, 0.0000000001)]

# Vector Magnitude
def magnitudeVector(vector):
	return math.sqrt(math.pow(vector[0], 2) + math.pow(vector[1], 2) + math.pow(vector[2], 2))

# Distance between two points
def distance(a, b):
	return magnitudeVector(subtractVector(a, b))

# Add two vectors
def addVector(a, b):
	return [ a[0] + b[0], a[1] + b[1], a[2] + b[2] ]

# Subtract two vectors
def subtractVector(a, b):
	return [ a[0] - b[0], a[1] - b[1], a[2] - b[2] ]

# Multiply two vectors
def multiplyVector(a, b):
	return [ a[0] * b[0], a[1] * b[1], a[2] * b[2] ]

# Dot product between two vectors
def dotVector(vectorA, vectorB):
	return vectorA[0] * vectorB[0] + vectorA[1] * vectorB[1] + vectorA[2] * vectorB[2]

# Cross product between two vectors
def crossVector(a, b):
	return [ (a[1] * b[2] - a[2] * b[1]), (a[2] * b[0] - a[0] * b[2]), (a[0] * b[1] - a[1] * b[0])]

# Scalar Product
def scaleVector(a, s):
	return [a[0] * s, a[1] * s, a[2] * s]

# Convert grid index to location within bounds
def getGridLocation(loc):
	v = loc
	for i in range(0,3):
		step = (maxbounds[i] - minbounds[i]) / (gridsize[i] - 1)
		v[i] = minbounds[i] + step * v[i]
	return v

### Generator Definitions ###

# Uniform Noise
def Uniform(v):
	vec = [random.uniform(-1.0, 1.0), random.uniform(-1.0, 1.0), random.uniform(-1.0, 1.0)]
	return vec

def UniformNormalized(v):
	return normalizeVector(Uniform(v))

# Grid Alternate
def Grid(v):
	a = math.fmod(v[1] + v[2], 3) - 1
	b = math.fmod(v[2] + v[0], 3) - 1
	c = math.fmod(v[0] + v[1], 3) - 1
	return [a, b, c]

def GridNormalized(v):
	return normalizeVector(Grid(v))

# Radial expansion
def Radial(v):
	vec = getGridLocation(v)
	vec = scaleVector( vec, 1 / max(distance([0,0,0], v), 0.000001))
	return vec

### Main Script ###
###Edit: This Section was Encapsulated into a function###
def makeVectorField(gridsize,minbounds,maxbounds,generator,filename,directionbias,directionstrength,scalenoiseamount,directionnoiseamount,mainscalefactor):
	bounds = [gridsize, minbounds, maxbounds]
	vectors = []
	gridcenter = [None] * 3

	generatortypes = [
		### Currently available vector field generator types
		Uniform,					# 0
		UniformNormalized,			# 1
		Grid,						# 2
		GridNormalized,				# 3
		Radial,						# 4
		]

	gen = generatortypes[generator] # TODO: write a selector for this//  //EDIT: Made drop-downmenu in Tkinter

	for x in range(gridsize[2]):
		for y in range(gridsize[1]):
			for z in range(gridsize[0]):
				vec = gen([x, y, z])
				vec = scaleVector(vec, mainscalefactor)
				vec = addVector(vec, scaleVector(vec, random.uniform(-scalenoiseamount, scalenoiseamount)))
				vec = addVector(vec, scaleVector(UniformNormalized([x, y, z]), directionnoiseamount))
				vec = addVector(vec, scaleVector(directionbias, directionstrength))
				print str(vec)
				vectors.append(vec)

	f = open(filename, 'w')
	f.truncate()
	for vec in bounds:
		writevector(f, vec)
	for vec in vectors:
		writevector(f, vec)

	os.startfile(filename)
	#sys.exit()