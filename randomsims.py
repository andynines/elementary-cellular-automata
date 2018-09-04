"""
randomsims.py
Copyright (c) 2018 andynines
MIT License
"""

# Imports

import os
import random
import sys
import time



# Constants

EXECUTABLE_NAME = "./ecaterm"

HABITAT_WIDTH = 80
GENERATION_BUFFER_SIZE = 25

DELAY = 5



# Simulation generator

def commandSim():
    os.system("{} {} {} {} {} {} {}".format(
        EXECUTABLE_NAME,
        random.randint(0, 255),
        HABITAT_WIDTH,
        GENERATION_BUFFER_SIZE,
        random.choice(["wrap", "dead", "alive"]),
        random.randint(0, HABITAT_WIDTH),
        random.choice(["even", "random"])))



def main():
    try:
        while True:
            print()
            commandSim()
            time.sleep(DELAY)
    except KeyboardInterrupt:
        print()
        sys.exit(0)



if __name__ == "__main__":
    main()
