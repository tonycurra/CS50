# Create half piramid, user prompt number from 1 to 8
while True:
    try:
        n = int(input("Height: "))
        if n > 0 and n < 9:
            break
    except ValueError:
        print("Insert int btw 1 and 8")

# print half pyramid
for i in range(n):
    # print space
    for k in range(n - i - 1):
        print(" ", end="")
    for j in range(i + 1):
        print("#", end="")
    print("")

