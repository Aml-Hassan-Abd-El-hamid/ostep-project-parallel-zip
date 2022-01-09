# ostep-project-parallel-zip

In that project, we reimplemented the function zip to be working as a parallel program.
we first started by giving each thread a file to work on. 
we build a threaded array with a size equal to the number of files given by the user, give every thread the original zip function to be their routine and passed the file name as an argument, then used pthread_join() to wait for each thread and take their return value which was a pointer to ret struct.
And then after that, we tried to solve the issue of the inequality of the size of the files given by the user.
we used the producer-consumer for this, 
our producer is a thread that takes a pointer to the pointer argv as an argument and a function called to produce, that function would open each file and read using mmap() function and then chop it to the size of a default page size that we defined in the main, and then the information about those pages would be put in a buffer.
our consumer threads would be passed the consume function as their working routine, the consume function would then the page information from the buffer and used to access them and send them to the zip function.
