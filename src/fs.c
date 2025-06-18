#include "fs.h"
#include "util.h"
#include "vga.h"
#include <stddef.h>

#define FS_NAME_MAX 64
static fs_node_t nodes[FS_MAX_NODES];
static int        node_count;
static fs_node_t *cwd;

// bootstrap root
void fs_init(void) {
    node_count = 1;
    fs_node_t *r = &nodes[0];
    r->type     = FS_DIR;
    strcpy(r->name, "/");
    r->parent   = r;
    r->children = NULL;
    r->next     = NULL;
    r->size     = 0;
    r->pos      = 0;
    cwd         = r;
}

// list children of cwd
void fs_list(void) {
    for (fs_node_t *n = cwd->children; n; n = n->next) {
        print(n->name);
        if (n->type == FS_DIR) print("/");
        print("  ");
    }
    print("\n");
}

// make new dir under cwd
int fs_mkdir(const char *name) {
    // exist?
    for (fs_node_t *n = cwd->children; n; n = n->next)
        if (strcmp(n->name, name) == 0) return -1;
    if (node_count >= FS_MAX_NODES) return -1;
    fs_node_t *d = &nodes[node_count++];
    d->type     = FS_DIR;
    strcpy(d->name, name);
    d->parent   = cwd;
    d->children = NULL;
    d->next     = cwd->children;
    cwd->children = d;
    d->size = d->pos = 0;
    return 0;
}

// change cwd
int fs_chdir(const char *name) {
    if (strcmp(name, "..") == 0) {
        cwd = cwd->parent;
        return 0;
    }
    for (fs_node_t *n = cwd->children; n; n = n->next)
        if (n->type == FS_DIR && strcmp(n->name, name) == 0) {
            cwd = n;
            return 0;
        }
    return -1;
}

// open (or create) file in cwd
fs_node_t *fs_open(const char *name, int create) {
    for (fs_node_t *n = cwd->children; n; n = n->next)
        if (n->type == FS_FILE && strcmp(n->name, name) == 0) {
            n->pos = 0;
            return n;
        }
    if (!create || node_count >= FS_MAX_NODES) return NULL;
    fs_node_t *f = &nodes[node_count++];
    f->type     = FS_FILE;
    strcpy(f->name, name);
    f->parent   = cwd;
    f->children = NULL;
    f->next     = cwd->children;
    cwd->children = f;
    f->size = f->pos = 0;
    return f;
}

// read up to len bytes from f->data
unsigned int fs_read(fs_node_t *f, char *buf, unsigned int len) {
    if (!f || f->type != FS_FILE) return 0;
    unsigned int rem = f->size - f->pos;
    unsigned int n   = rem < len ? rem : len;
    memcpy(buf, f->data + f->pos, n);
    f->pos += n;
    return n;
}

// append up to available space
unsigned int fs_write(fs_node_t *f, const char *buf, unsigned int len) {
    if (!f || f->type != FS_FILE) return 0;
    unsigned int space = FS_FILE_SIZE - f->size;
    unsigned int n     = len < space ? len : space;
    memcpy(f->data + f->size, buf, n);
    f->size += n;
    return n;
}

const char *fs_pwd_name(void) {
    /* if cwd->name is exactly "S", just return it */
    if (strcmp(cwd->name, "S") == 0) {
        return cwd->name;
    }

    /* otherwise prepend 'S' */
    static char buf[FS_NAME_MAX + 2];
    size_t len = strlen(cwd->name);
    if (len > FS_NAME_MAX) len = FS_NAME_MAX;  /* avoid overflow */

    buf[0] = 'S';
    /* copy name and terminating NUL in one go */
    memcpy(&buf[1], cwd->name, len + 1);

    return buf;
}