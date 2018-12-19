# Finding the Shortest Path using multi-threading
### General
        The purpose of this  program was to use and and implement the Floyd-Warshall All-Pairs-Shortest-Path
algorithm. What this algorithm does is it finds all the possible from every node to every other node, up to n
nodes. This method was to be implemented using a single threaded methodology and a multi thread
methodology. The function is then to be tested on 10 nodes, 100, nodes, 1000 nodes, and 10 000 nodes. The time
to complete each task will then be taken and graphed to explain and show the speed difference that may araise
when comparing the two threading models. The results will then be graphed to give a clear understanding about
what is going on behind the scenes. This data was all run on my 4 core computer with 8 gigs on RAM, time vary
from system to system and accounting for the number of processor had. Also in should be noted that I work
along side Daniel Doucett, so we may some similar code not all will be the same.

### Findings
        Based on the data found it can be seen that single thread process seemed to be the winner. The single thread out
preformed the multi thread on all counts. It seem that the more threads that were created using the multi threaded
methodology the slower the system got. Some reasons for this could be the fact that the computer used only had
four processors. Thus when threads context switching happened many of threads were waiting for their turn
when semaphore locked them out of updating the graph matrix. Since the single thread did not have to wait and
simple went through each section of the graph matrix it was to process this data more in efficiently. Though as
the amount nodes got larger in did begin to slow down the system, and tested on 10 000 nodes cause process to
run well beyond 30 min till it was killed.

        Some possible solutions to optimize this project would be to instead of creating a thread for every node, only
create a certain amount and divide the task up accordingly. This way there wont be as many context switching
and the thread will have less waiting to do. Another solution would be run this program on computer with more
processor this way all threads can be shared over more processors and may have a slightly reduced wait time.
Over all I believe this project was a success. It showed the point that not all processes need to be made mulit
thread to increase their efficiency. Sometimes maybe better to use a single thread, or just fewer threads.

#### Authors
###### Tye Borden
###### Daniel Doucett
