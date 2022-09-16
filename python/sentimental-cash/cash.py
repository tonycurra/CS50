# cash
from cs50 import get_float

# get input amount
while True:
    try:
        amount = get_float("Change owed: ")
        if amount > 0:
            break
    except ValueError:
        print("input a positive number")

# count change coins
coins = 0
for i in [0.2500, 0.1000, 0.0500, 0.0100]:
    n = amount // i
    coins += n
    amount -= (n * i)

print(coins)
