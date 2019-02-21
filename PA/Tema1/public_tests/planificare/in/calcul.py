

def pw3(n):
    return n**3

variables = [33, 4, 1, 4, 12, 4, 37, 19, 5, 11, 3, 29, 11, 11, 33, 5, 1, 25, 26, 11, 14, 32, 20, 11, 8, 21, 14, 17, 7, 17, 22, 1, 38]
total = 0

for i in variables:
    total += pw3(i)

print(total)
