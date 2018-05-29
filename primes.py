from random import randint

def findPrime(p):
	k = 3000
	composite = False

	for v in range(k):
		a = (pow(randint(2,p-2),(p-1),p))
		if(a != 1):
			composite = True
	if composite:
		return 0
	else:
		return p

lower = 10000000000000000
upper = 999999999999999999

open("primes.txt",'w').close()
file = open("primes.txt", 'a')

p = randint(lower, upper)
while findPrime(p) == 0:
	p = randint(lower, upper)
file.write(str(p) + "\n")

c = randint(lower, upper)
while findPrime(c) == 0:
	c = randint(lower, upper)
	while c == p:
		c = randint(lower, upper)
file.write(str(c))