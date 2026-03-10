#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "fs.h"
#include "disk.h"

#define STRSZ  1024   // max string size


/**
 * do_copyout - read contents of 'fsname' in our filesystem do a real file 'filename'
 * 		if 'filename'= "" (empty string) it will copy to stdout.
 * 		uses fs_open to get file id (dirent number) and then fs_read to read its contents
 * 		returns 1 if ok; or 0 in case of error
 */
int do_copyout(char *fsname, char *filename) {
	int offset = 0, result, fid;
	char buffer[1000];
	FILE *fout = stdout;
	if (strlen(filename)>0)
		if ( (fout = fopen(filename, "w")) == NULL) {
			perror(filename);
			return 0;
		}

	fid = fs_open(fsname);
	if (fid==-1) {
		printf("can't open %s in my disk\n", fsname);
		return 0;
	}

	while (1) {
		result = fs_read(fid, buffer, sizeof(buffer), offset);
		if (result <= 0) break;
		fwrite(buffer, result, 1, fout);
		offset += result;
	}
	if (strlen(filename)>0) fclose(fout);
	printf("%d bytes copied\n", offset);
	return 1;
}

/**
 * do_copyin - copy a real file 'filename' to 'fsname' in our filesystem
 * 		returns 1 if ok;  0 if error 
*/
int do_copyin( const char *filename, char *fsname ) {
    int offset = 0, result, actual, fid;
    char buffer[1000];

    FILE *fin = fopen( filename, "r" );
    if (fin==NULL) {
        perror(filename);
        return 0;
    }
    fid = fs_create( fsname );
	if (fid == -1) {
		printf("can't create %s in my disk\n", fsname);
		return 0;
	}
    while (1) {
        result = fread( buffer, 1, sizeof(buffer), fin );
        if (result <= 0) break;
        actual = fs_write( fid, buffer, result, offset );
		if (actual>0) offset += actual;
        if (actual < result) {
            printf( "fs_write returned %d (expected %d)\n", actual, result );
			break;
        }
    }
    printf( "%d bytes copied\n", offset );

    fclose( fin );
    return 1;
}

/**
 * MAIN 
 * just a shell to work as a test program for the file system
 */
int main(int argc, char*argv[]) {
	char line[STRSZ];
	char cmd[STRSZ];
	char arg1[STRSZ];
	char arg2[STRSZ];
	int args, nblocks;

	if (argc != 3 && argc != 2) {
		printf("use: %s diskfile  to use an existing disk\n", argv[0]);
		printf("use: %s diskfile nblocks  to create a new disk with nblocks\n", argv[0]);
		return 1;
	}
	if (argc == 3){
		nblocks = atoi(argv[2]);
		if(nblocks == 1){
			printf("Cannot create Disk with size 1!\n");
			return 1;
		}
	}
	else
		nblocks = -1;

	if (!disk_init(argv[1], nblocks)) {
		printf("unable to initialize %s: %s\n", argv[1], strerror(errno));
		return 1;
	}

	printf("opened emulated disk image %s with %d blocks\n", argv[1],
			disk_size());

	while (1) { 
		printf("main> "); fflush(stdout);

		if (!fgets(line, sizeof(line), stdin)) 
			break;
		if (line[0] == '\n')  // empty line
			continue;
		line[strlen(line) - 1] = '\0';
		args = sscanf(line, "%s %s %s", cmd, arg1, arg2);
		if (args == 0)
			continue;

		if (!strcmp(cmd, "format")) {
			if (args == 1)
				if (fs_format())
					printf("disk formatted.\n");
				else
					printf("format failed!\n");
			else
				printf("use: format\n");
		} else if (!strcmp(cmd, "mount")) {
			if (args == 1) {
				if (fs_mount()) {
					printf("disk mounted.\n");
				} else {
					printf("mount failed!\n");
				}
			} else {
				printf("use: mount\n");
			}
		} else if (!strcmp(cmd, "umount")) {
			if (args == 1) {
				if (fs_umount()) {
					printf("disk unmounted.\n");
				}
			} else {
				printf("use: umount\n");
			}
		} else if (!strcmp(cmd, "debug")) {
			if (args == 1) {
				fs_debug();
			} else {
				printf("use: debug\n");
			}
		} else if (!strcmp(cmd, "ls")) {
			if (args == 1) {
				fs_lsdir();
			} else {
				printf("use: ls\n");
			}
		} else if (!strcmp( cmd, "create" )) {
            if (args == 2) {
                int num = fs_create( arg1 );
                if (num >= 0) {
                    printf( "created inode %d\n", num );
                } else {
                    printf( "create failed!\n" );
                }
            } else {
                printf( "use: create <name>\n" );
            }
		} else if (!strcmp( cmd, "copyin" )) {
            if (args == 3) {
                if (do_copyin( arg1, arg2 )) {
                    printf( "copied file %s to fs %s\n", arg1, arg2 );
                } else {
                    printf( "copy failed!\n" );
                }
            } else {
                printf( "use: copyin <filename> <fsname>\n" );
            }
        } else if (!strcmp( cmd, "copyout" )) {
            if (args == 3) {
                if (do_copyout( arg1, arg2)) {
                    printf( "copied fs %s to file %s\n", arg1, arg2 );
                } else {
                    printf( "copy failed!\n" );
                }
            } else {
                printf( "use: copyout <fsname> <filename>\n" );
            }
		} else if (!strcmp(cmd, "cat")) {
			if (args == 2) {
				if (!do_copyout(arg1, "")) {
					printf("cat failed!\n");
				}
			} else {
				printf("use: cat <fsname>\n");
			}
		
		} else if (!strcmp(cmd, "help") || !strcmp(cmd, "?")) {
			printf("Commands:\n");
			printf("    format\n");
			printf("    mount\n");
			printf("    umount\n");
			printf("    debug\n");
			printf("    ls\n");
			printf("    create <name>\n" );
			printf("    cat <fsname>\n");
			printf("    copyin  <file> <name>\n" );
            printf("    copyout <name> <file>\n" );
			printf("    help or ?\n");
			printf("    quit or exit\n");
		} else if (!strcmp(cmd, "quit") || !strcmp(cmd, "exit")) {
			break;
		} else {
			printf("unknown command: %s\n", cmd);
			printf("type 'help' for a list of commands.\n");
		}
	}
	//fs_umount ?
	printf("closing emulated disk.\n");
	disk_close();

	return EXIT_SUCCESS;
}