A project on simulating 2 types of caches namely FA cache and DM cache with varying line sizes and different memory access patterns.
This project was done by
Mostafa Gaafar, 900214463
Ismaiel Sabet 900221277, 
Ahmed Abdeen, 
and Youssef Badawy, who are all students in the American university in Cairo. This project was made in correspondance with the requirements for CSCE 2303 (computer organization and assembly).

There are 6 memory generation functions which each produce a different pattern of memory access, 
you are advised to check the, before running in order to decide on your test cases. 

To run the code on the command line simply produce an executable file from the cpp file using
g++ P2.cpp -o P2.exe
Then type ./P2 x y z t 
The X can be 1 or 2, if its 1 the simulation will run a direct mapped cache, if its 2 it will run a fully associative cache.
The Y represents the size of the line used.
The Z represents what memory generation function you wish to run.
The T represents how many iteration you want the simulation to run for.

The simulators are made as a vector or lines (line being a struct) each line contains a valid bit, a tag, and a data. 
An attempt to simulate the fully associative cache with an unordered map was made however due to the fact that unordered maps reorders its elements based on keys we found that its not the best implementation,
an issue was posted detailing this attempt. 

Data cllected as well as the graphs can be found in their respective folder, the analysis and conclusions can be found in the report. If you faced any problems with this simulator please raise
an issue and our team will respond to it as quickly as we can

