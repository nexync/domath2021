import numpy as np

file1 = open("c_vals", "rb")
cv = np.load(file1)

file2 = open("params", "rb")
par = np.load(file2)

x = cv[np.where(cv>1)]
y = par[np.where(cv>1)]



# x = []
# y = []
# for i in range(10):
# 	file = open("c_vals" + str(i), "rb")
# 	arr = np.load(file)
# 	for val in arr:
# 		x.append(val)

# 	file2 = open("params" + str(i), "rb")
# 	arr2 = np.load(file2)
# 	for val in arr2:
# 		y.append(val)

# x = np.array(x)
# y = np.array(y)

filea = open("c_vals", "wb")
np.save(filea, x)
filea.close

fileb = open("params", "wb")
np.save(fileb, y)
fileb.close