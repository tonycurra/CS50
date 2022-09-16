# Readability
from cs50 import get_string

text = get_string("Text: ").lower()

# count letters, words, sentences in text
letters = 0
words = 1
sentences = 0
for i in text:
    if i >= 'a' and i <= 'z':
        letters += 1
    elif i == ' ':
        words += 1
    elif i in ['.', '?', '!']:
        sentences += 1

# formula to calculate grade
L = 100 * letters / words

S = 100 * sentences / words

X = round(0.0588 * L - 0.296 * S - 15.8)

# print results
if X < 1:
    print('Before grade 1')
elif X >= 16:
    print('Grade 16+')
else:
    print(f'Grade {X}')