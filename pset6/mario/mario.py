from cs50 import get_int

height = -1
while height < 1 or height > 8:
    height = get_int("Height:\n")

for i in range(height):
    j = 1
    while j < height - i:
        j += 1
        print(" ", end="")
    for k in range(i + 1):
        print("#", end="")
    print("  ", end="")
    for l in range(i + 1):
        print("#", end="")
    print()
