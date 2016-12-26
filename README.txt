    Imperial Messengers

    Author: Brandon Shea
    Compiler: Clang Version 19.10.24728 for x86
    Summary: 
        Figure out how long it takes to send a message from the capitol 
        throughout the Empire.

        Dijktra's Algorith is utilized to figure out the shortest routes 
        from the capitol to each city.

    Instructions:
        ./main <filename> 
        Supply a file that fits the given input specs. The program will 
        return the expected time to deliver the message throughout the 
        Imperial cities.

    Notes:
        - INT_MAX is used to fill the initial distance results array. 
        - Any path with that value, is considered in-accessible.

    TimeTrack:
        Enviroment and version control set up   - 2 hrs
        File I/O and verifying valid inputs     - 4 hrs
        Dijktra's Implementation                - 4 hrs
        Testing and Documentation               - 4 hrs