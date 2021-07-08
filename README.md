# Path-Detection

Objective : An input image will be given. The image will have a source marked with green , destination marked with red and obstacles marked with white. The aim is to                 generate a path from the source to the destination.

Concepts Used : The task heavily makes use of Open CV C++ library and is basically an implementation of RRT and RRT Connect Algorithm. In the ROS part we make the turtle                 in the simulator move from the source to the destination.

The file trs.cpp implements rrt on the given image named tut.jpg. Typing ./trs on the command line will write the output file to output.png . The line in green in output.png is the final path from the source to the destionation. The source is is marked in green and the destination in red and obstacles in white. rrtconnect.cpp implements the rrt connect algorithm. Turtlesim program is written in the ros folder.
