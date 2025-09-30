# Marching Squares Algorithm – Image Contour Extraction (PPM-P3)

**Author:** SERBAN Victor Gabriel  
**Group:** 313CA  

---

##  Overview

This project implements the **Marching Squares** algorithm using a **modular and command-driven architecture**.  
It operates on **PPM-P3 images**, enabling:

- **Dynamic memory management**
- **Contour detection using predefined 16 Marching Squares patterns**
- **Grid-based image processing**
- **Image resizing (×4 zoom)**
- **Error-safe execution with validation and cleanup routines**

---

##  Core Architecture

The program is structured around a sequence of **commands**, each mapped to a dedicated function.  
Execution flow is controlled within `main()`, allowing sequential image processing operations.

| Command | Description |
|---------|-------------|
| `READ <file>` | Dynamically allocates and loads pixel matrix from PPM-P3 image |
| `WRITE <file>` | Saves or displays the processed image |
| `INIT_CONTUR` | Displays the 16 predefined Marching Squares contour patterns |
| `GRID` | Builds and prints the scalar grid derived from the image |
| `MARCH` | Applies the Marching Squares algorithm to generate contours |
| `RESIZE` | Scales the image by **×4** via pixel replication |
| `EXIT` | Frees all resources and terminates execution |

---

##  Implementation Details

###  Memory Management
- `struct pixel **image` stores the RGB data
- `allocation_matrix_int()` dynamically allocates integer-based contour/grids
- `free_matrix_pixel()` and `free_matrix_int()` safely release resources

###  Image Assignment Utilities
- `assign()` copies a scalar intensity to all RGB channels of a pixel
- `outlineX()` and `outline_matrix_X()` (X = 0–15) store and print the 16 Marching Square patterns

###  Marching Squares Logic
The algorithm:
1. Builds a **binary grid** based on intensity thresholding
2. Iterates through **2×2 blocks**
3. Matches each configuration against one of the **16 contour cases**
4. Constructs the final outline matrix

---

##  Error Handling

The program includes **robust validation**:

- Invalid file access → reported gracefully  
- Improper allocations → caught and handled  
- Illegal command usage → warning messages  

All allocated structures are **freed before exit**, ensuring **zero memory leaks**.

---

##  Conclusion

This implementation of **Marching Squares** successfully transforms scalar images into **clean contour maps**, preserving structure through modular, maintainable C code.

- Fully dynamic  
- Modular and readable  
- Memory-safe  
- Extensible for further enhancements

---

