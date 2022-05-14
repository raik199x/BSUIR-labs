import random

words = ["raik199x", "is", "god", "Basak", "best", "teacher", "thanks", "more", "random", "words", "where", "is", "money", "lebowsky", "love", "pizza", "dota", "bad", "game"]
path = input("create file with name: ")
number = int(input("How many words to random: "))

with open(path,"w") as file:
    for i in range(number):
        randomIndex = random.randint(0,len(words)-1)
        file.write(words[randomIndex])
        if i % 8 == 0 and not i == 0 :
            file.write('\n')
        else:
            file.write(' ')
