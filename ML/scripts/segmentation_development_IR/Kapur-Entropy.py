#Kapur, Sahoo, Wong entropy thresholding method
import math



def determineThreshold(weights):
	max_temp = max(weights)	
	min_temp = min(weights)
	t = min_temp	
	t_opt = 0
	MJ = sys.float_info.max
	for i in range(int(min_temp), (1 + int(max_temp))):
		class1Distribution,class2Distribution = determineGrayLevelDistribution(t, weights)
		ranking = HbPlusHw(class1Distribution,class2Distribution)
		if ranking < MJ:
			MJ = ranking
			t_opt = t
			t += 1
		else:
			t += 1
	return t_opt




#take the passed in weights and given threshold.
#separate into the two classes.  determine p0/Pt
#for each element p0 to pt in class 1, and pt+1/(1-Pt)
#for all the elements in the second class.
#Return those 2 sets of values.
def determineGrayLevelDistribution(weights, t):
	class1 = []
	class2 = []
	class1AlreadyDone = []
	class2AlreadyDone = []	
	class1Distr = []
	class2Distr = []	
	for i in range(0, len(weights)):
		if weights[i] < t:
			class1.append(weights[i])
		else:
			class2.append(weights[i])
	class1Pt = (len(class1) + 0.) / (len(weights) + 0.)
	class2Pt = 1.0 - class1Pt
	for i in range(0, len(class1)):
		if class1AlreadyDone.count(class1[i]) == 0:
			occurences = class1.count(class1[i])
			probOverTotal = occurences / class1Pt
			class1Distr.append(probOverTotal)
			class1AlreadyDone.append(class1[i])
	for i in range(0, len(class2)):
		if class2AlreadyDone.count(class2[i]) == 0:
			occurences = class2.count(class2[i])			
			probOverTotal = occurences / class2Pt
			class2Distr.append(probOverTotal)
			class2AlreadyDone.append(class2[i])
	return class1Distr, class2Distr


def HbPlusHw(class1Distr, class2Distr):
	Hb = 0.
	Hw = 0.
	for i in range(0, len(class1Distr)):
		Hb += (class1Distr[i] * math.log(class1Distr[i]))
	for i in range(0, len(class2Distr)):
		Hw += (class2Distr[i] * math.log(class2Distr[i]))
	Hb *= -1.
	Hw *= -1.
	finalValue = Hb + Hw
	return finalValue
