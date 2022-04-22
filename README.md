# csce311-project-4

# My program currently uses mmap and shared memory along with semaphores in order to send a file name to the server, the server then opens the file and maps the contents back to the client, and then the client then changes all of the letters to uppercase. I haven't used threading yet but I am attempting that, but the threads will essentially break up the file so it doesnt take a linear time to change all roughly 30000 lines to full uppercase.
