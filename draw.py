from colorama import Fore
from colorama import Style


file = open("maze.txt", 'r')
file2 = open("solution.txt", 'r')
# solution = [(0, 0), (1, 1), (2, 2), (3, 3)]
solution = []
for line in file2.readlines():
    line = line.strip('\n')
    line = line.split()
    line = [int(x) for x in line]
    solution.append(line)

print(solution)

print("----------------------")
print("Maze:")
for line in file.readlines():
    line = line.strip('\n')
    line.split()
    for char in line:
        if char == "o":
            print(f'{Fore.YELLOW}' + char + " " + f'{Style.RESET_ALL}', end="")
        elif char == "-":
            print(f'{Fore.GREEN}' + char + " " + f'{Style.RESET_ALL}', end="")
        elif char == 'x':
            print(f'{Fore.BLUE}' + char + " " + f'{Style.RESET_ALL}', end="")
        elif char == 'V':
            print(f'{Fore.RED}' + char + " " + f'{Style.RESET_ALL}', end="")
    print()

file.close()
print("----------------------")
print("Solution:")
file = open("maze.txt", 'r')
for x, line in enumerate(file.readlines()):
    line = line.strip('\n')
    line.split()
    # print(line)
    for y, char in enumerate(line):
        if [x, y] in solution:
            # print(char)
            print(f'{Fore.RED}'+'+'+" "+f'{Style.RESET_ALL}', end="")
            # continue
        else:
            if char == "o":
                print(f'{Fore.YELLOW}' + char + " " + f'{Style.RESET_ALL}', end="")
            if char == "-":
                print(f'{Fore.GREEN}' + char + " " + f'{Style.RESET_ALL}', end="")
            if char == 'x':
                print(f'{Fore.BLUE}' + char + " " + f'{Style.RESET_ALL}', end="")
            if char == 'V':
                print(f'{Fore.RED}' + char + " " + f'{Style.RESET_ALL}', end="")
    print()

