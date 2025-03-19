# Marching-Squares

    For the Marching Squares algorithm implemented by me, I have chosen a multi-function approach.
    It fulfills all the criteria specified in the in the title, being built through dynamic memory management, modeling
    modeling and manipulation of images in PPM-P3 format, resizing of read and processed images. Contour matrices are used which
    help the Marching Squares algorithm to work properly.
        In order to realize its functionality, we chose a structure based on commands and functions, called within the main function.
        READ command, realizes the dynamic allocation of a pixel time structure, named in my implementation , image (pixel structure **image)
    and realizes the populating of the matrix, with the necessary information, to process the image. Initially, the image dimensions, the maximum value
    of pixels and image type.
        The function WRITE, realizes the display of the image, whether processed or not.
        The INIT_CONTUR command, calls the init_outline function which displays the 16 patterns necessary for image processing, based on the Marching Squares algorithm.
        The GRID function, builds in memory the grid associated to the read image, and displays it on the screen.
        The MARCH command realizes the actual modification of the image, built with the created contours.
        The RESIZE function, enlarges the input image by x4.
        EXIT terminates the program.
        My algorithm catches error situations, so, the program signals improper allocations and error cases
    specific to erroneous inputs.
        In addition to the functions mentioned in the problem statement, I created others that help modularize the algorithm and allow a better understanding
    of the code, everything being more intuitive and easier to follow.
        We implemented the functions free_matrix_pixel and free_matrix_int (of type void), which help to free the memory for the matrices of type struct pixel,
    respectively of type int.
        The assign function assigns the value of the passed parameter to the 3 colors of a pixel.
        The functions outlineX and outline_matrix_X (where X is a value from 0 to 15), help to display the outlines and to build in memory the image based on 
    them.
        The function allocation_matrix_int, receives as parameters a matrix of type int and its dimensions, and allocates it dynamically, in order to be used,
    throughout the algorithm.
        Through the harmonious combination of all the commands and functions associated with the C programming language, the algorithm implemented by me successfully realizes Marching
    Squares, transforming images into contours (two-dimensional scalar maps).
