#ifndef FS_H
#define FS_H

#define FS_MAX_NAME   32
#define FS_FILE_SIZE  512
#define FS_MAX_NODES  64

typedef enum { FS_FILE, FS_DIR } fs_type_t;

typedef struct fs_node {
    char            name[FS_MAX_NAME];
    fs_type_t       type;
    struct fs_node *parent;
    struct fs_node *children;
    struct fs_node *next;
    char            data[FS_FILE_SIZE];
    unsigned int    size;   // bytes in file
    unsigned int    pos;    // read/write cursor
} fs_node_t;

// call at boot
void         fs_init(void);

// shell
void         fs_list(void);
int          fs_mkdir(const char *name);
int          fs_chdir(const char *name);
const char  *fs_pwd_name(void);

// file I/O
fs_node_t   *fs_open(const char *name, int create);
unsigned int fs_read(fs_node_t *f, char *buf, unsigned int len);
unsigned int fs_write(fs_node_t *f, const char *buf, unsigned int len);

#endif
