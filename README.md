# Basic-File-System# Emulated File System

A C-based implementation of a file system running on an emulated disk. This project simulates disk blocks, bitmaps for space management, and directory entries (inodes) for file storage.

## Project Structure

The project is divided into several modular components:

- **disk.c/h**: Emulates physical hardware by providing block-level read/write access to a file.
- **bitmap.c/h**: Manages a bit array to track which disk blocks are allocated or free.
- **fs.c/h**: Implements the high-level file system logic (Superblock, Dirents, Read/Write).
- **main.c**: Provides a shell interface to interact with the system.

---

## Compilation and Setup

To build the project, compile all C files together using:

```bash
make
```

---

## Usage Guide

### 1. Initialize the Disk
You can create a new virtual disk or open an existing one:

```bash
# Create a new disk with 1024 blocks
./main mydisk.img 1024

# Open an existing disk
./main mydisk.img
```

### 2. Shell Commands
Once the `main>` prompt appears, use these commands to manage the filesystem:

- **format**: Erases the disk and sets up the Superblock and Bitmap.
- **mount**: Loads the filesystem into memory for use.
- **ls**: Lists all files, their sizes, and block locations.
- **copyin <local_file> <fs_name>**: Imports a file from your OS into the emulated disk.
- **copyout <fs_name> <local_file>**: Exports a file from the emulated disk to your OS.
- **cat <fs_name>**: Prints the file content to the screen.

---

## Specifications

### File Metadata (Inodes)
Each file is represented by a `struct fs_dirent`. It contains the filename, size, and direct pointers to data blocks.

```c
struct fs_dirent {     
    uint8_t isvalid;
    char name[MAXFILENAME];
    uint32_t size;                
    uint32_t blk[POINTERS_PER_ENT];
};
```

---

## Important Note
- The system supports single-level directories (no subfolders).