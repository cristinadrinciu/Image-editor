The project includes .c and .h files: the main image_editor.c, its header image_editor.c, a header for the structures struc.h and commands.c where are implemented the functions from commands.h.

The project works like an image editor that can LOAD, SAVE, SELECT, CROP and apply filters for Grayscal images and RGB images, in PPM and PGM format.

I defined the structure image that has all the needed information for an image: dimensions, type, the matrix of the image, the coordinates of the selection and the loaded variable that counts if there is a loaded image or not.
THe RGB images are saved in a RGB matrix, a structure that has 3 fields: for the the red, green and blue pixel.

The program receives commands from the terminal. 

LOAD: Receiving a file name, it opens the file and gets the image and it's type. Before each LOAD operation, if there was a previous image loaded it is freed (if the pointer of the matrix isn't NULL). The type of the image is saved and based on it, the image is saved RGB or Grayscale, in binary or ascii format. The selection is automatically the image itself. I used two helping functions for the Grayscale and RGB.

SAVE: Receiving a file name, it opens the file and prints(saves) the image in the format needed. If the ascii word is present in the format, then the image is saved as ascii, outherwise, as binary. The file is opened as text or binary, depending on the case. Then the type, dimensions and the max values are printed, after that the matrix. I used two helping functions for the Grayscale and RGB. 

SELECT: Receiving some coordinates, they are verified if they are valid. If positive, they are atributed to the image selection coordinates.

SELECT ALL: The image coordinates are changed to the full image: 0, 0, rows, columns.

CROP: Based on the selection, the image is cropped. The selection is transfered into an auxiliar matrix, the original is freed and redimensioned on the selection, then the values are put back in the image matrix. At the end the auxiliar one is freed.

EQUALIZE: The operations needs a Grayscale image. It equalizes the tones in tha black and white images, based on an algorithm. 

HISTOGRAM: Receiving the number of bins and the maximum '*' for printing, the operation prints the histogram of the tones of black and white from a grayscale image.

APPLY_SHARPEN, APPLY_EDGE, APPLY_BLUR, APPLY_GAUSSIAN_BLUR: The apply operation receives a filter. Based on it the image is modified. The way the filters are applied is by multiplying the image matrix of the RGB image with the specific kernel.

ROTATE: Receiving the angle, the image or the selection is rotated to the specific angle. The only possible angles are +/-0, +/-90, +/-180, +/-270, +/-360. I used helping functions for the Grayscale and RGB and also rotating functions for the full image and selection for 90 right, 90 left and 180, based on all the angles receivd can be satisfied. For this operations, the selection must be square if it is not the full image.

EXIT: The operation frees the memory from all resources that had been used and it stops the program.

In commands.c are implemented functions that help building the main function, the program itself.
The commands are read with fgets as strings, which means it reads to the '\n' character. The functions from commands.c help indentify the right operation (verif_NAME_OPERATION) and calls the right function for the operation(call_NAME_OPERATION), with also extracting the parametres pf the command, such as the angle, file name, ascii format, coordinates.

In the main function is built the program with calling all the functions needed and printing all the error messages.
