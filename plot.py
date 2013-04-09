import matplotlib.pyplot as plt

f = open("spikeWindow.txt")

values = f.read().splitlines()

plt.plot( values )

plt.ylabel( "decibels" )

plt.show()
