#include <minix/drivers.h>
#include <minix/vtreefs.h>
#include <sys/stat.h>
#include <time.h>
#include <assert.h>
 
static void my_init_hook(void)
{	
	/* This hook will be called once, after VTreeFS has initialized.
	 */
	struct inode_stat file_stat;
	struct inode *inode;
 
	/* We create one regular file in the root directory. The file is
	 * readable by everyone, and owned by root. Its size as returned 
by for
	 * example stat() will be zero, but that does not mean it is 
empty.
	 * For files with dynamically generated content, the file size 
is
	 * typically set to zero.
	 */
	file_stat.mode = S_IFREG | 0444;
	file_stat.uid = 0;
	file_stat.gid = 0;
	file_stat.size = 3;
	file_stat.dev = NO_DEV;
 
	/* Now create the actual file. It is called "test" and does not 
have an
	 * index number. Its callback data value is set to 1, allowing 
it to be
	 * identified with this number later.
	 */
	inode = add_inode(get_root_inode(), "test", NO_INDEX, 
&file_stat, 0,
		(cbdata_t) 1);
 
	assert(inode != NULL);
}
 /* ssize_t (*read_hook)(struct inode *inode, char *ptr, size_t len,
            off_t off, cbdata_t cbdata);
*/

/*static int my_read_hook(struct inode *inode, off_t offset, char **ptr,
	size_t *len, cbdata_t cbdata) */

static int my_read_hook(struct inode *inode, char *ptr,	size_t len, off_t offset, cbdata_t cbdata)
{
	/* This hook will be called every time a regular file is read. 
We use
	 * it to dyanmically generate the contents of our file.
	 */
	static char data[26];
	const char *str;
	time_t now;
 
	/* We have only a single file. With more files, cbdata may help
	 * distinguishing between them.
	 */
	assert((int) cbdata == 1);
 
	/* Generate the contents of the file into the 'data' buffer. We 
could
	 * use the return value of ctime() directly, but that would make 
for a
	 * lousy example.
	 */
	/* time(&now);
 
	str = ctime(&now);
 
	strcpy(data, str); */
 
	/* If the offset is beyond the end of the string, return EOF. */
	if (offset > 3 /*strlen(data)*/) {
	/*	 len = 0; */ 
 
		return 0;
	} 
 
	/* Otherwise, return a pointer into 'data'. If necessary, bound 
the
	 * returned length to the length of the rest of the string. Note 
that
	 * 'data' has to be static, because it will be used after this 
function
	 * returns.
	 */

	data[0]='h'; data[1]='i'; data[2]='\0';

        memmove(ptr, data, 3);

	/* ptr = data; */ // + offset;
 
	/* if (len > strlen(data) - offset)
		*len = strlen(data) - offset;
 */
	return 3;
}
static int my_chstat_hook(struct inode* inode, struct inode_stat* stat, cbdata_t cbdata)
{
set_inode_stat(inode, stat);
return 0;
}

 
/* The table with callback hooks. */
struct fs_hooks my_hooks = {
	my_init_hook,
	NULL, /* cleanup_hook */
	NULL, /* lookup_hook */
	NULL, /* getdents_hook */
	my_read_hook,
	NULL, /* rdlink_hook */
NULL, /* rdlink_hook */
NULL, /* rdlink_hook */
NULL, /* rdlink_hook */
NULL, /* rdlink_hook */
NULL, /* rdlink_hook */
	my_chstat_hook,/* rdlink_hook */
	NULL  /* message_hook */
};
 
int main(void)
{
	struct inode_stat root_stat;
 
	/* Fill in the details to be used for the root inode. It will be 
a
	 * directory, readable and searchable by anyone, and owned by 
root.
	 */
	root_stat.mode = S_IFDIR | 0555;
	root_stat.uid = 0;
	root_stat.gid = 0;
	root_stat.size = 0;
	root_stat.dev = NO_DEV;
 
	/* Now start VTreeFS. Preallocate 10 inodes, which is more than 
we'll
	 * need for this example. No indexed entries are used.
	 */
	/* start_vtreefs(&my_hooks, 10, &root_stat, 0); */
        run_vtreefs(&my_hooks, 10, 0, &root_stat, 0, 0);

	/* The call above never returns. This just keeps the compiler 
happy. */
	return 0;
}

