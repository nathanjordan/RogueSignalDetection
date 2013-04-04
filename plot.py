import matplotlib.pyplot as plt

f = open("out.csv")

values = f.read().splitlines()

plt.plot( values )

plt.ylabel( "decibels" )

plt.show()
