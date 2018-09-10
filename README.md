# BruinNav

Given a start location and end destination, BruinNav uses a data set of
longitude and latitude coordinates corresponding to the end points of various
street segments in LA to find the most optimal route.

## Algorithms implemented
I implemented a binary tree data structure using templates so that it could 
store and associate any data type. I also used the STL priority_queue as a 
min heap by overloading the < operator so that I could explore street segments
with lower costs first. This allowed my algorithm to more quickly determine the 
shortest route from Point A to Point B by using a heuristic similar to the A* 
path-finding algorithm to assign weights based on the accumulated distance from 
the starting point to the current point and the distance from the current point
to the ending point.

## Result
BruinNav reports identical routes to the ones given by Google Maps for walking
since my program makes no assumptions about whether the street segment is a road 
for cars or just a sidewalk.
