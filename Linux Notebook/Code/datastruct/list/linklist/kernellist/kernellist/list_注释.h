#ifndef _LINUX_LIST_H
#define _LINUX_LIST_H

struct list_head {
	struct list_head *prev;
	struct list_head *next;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)
//struct list_head name = { &(name), &(name) }

//内联函数/内建函数  inline建议性关键字
static inline void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_add(struct list_head *new,
			      struct list_head *prev,
			      struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}


/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	prev->next = next;
}

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty() on entry does not return true after this, the entry is
 * in an undefined state.
 */
static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = NULL;
	entry->prev = NULL;
}

/**
 * list_replace - replace old entry by new one
 * @old : the element to be replaced
 * @new : the new element to insert
 *
 * If @old was empty, it will be overwritten.
 */
static inline void list_replace(struct list_head *old,
				struct list_head *new)
{
	new->next = old->next;
	new->next->prev = new;
	new->prev = old->prev;
	new->prev->next = new;
}

static inline void list_replace_init(struct list_head *old,
					struct list_head *new)
{
	list_replace(old, new);
	INIT_LIST_HEAD(old);
}

/**
 * list_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
static inline void list_del_init(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry);
}

static inline int list_is_last(const struct list_head *list,
				const struct list_head *head)
{
	return list->next == head;
}

static inline int list_empty(const struct list_head *head)
{
	return head->next == head;
}

static inline int list_empty_careful(const struct list_head *head)
{
	struct list_head *next = head->next;
	return (next == head) && (next == head->prev);
}

/**
 * list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 */
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
/*
@功能：求出type类型结构体中的成员MEMBER相对与结构体首地址的偏移地址
@算法原理：((TYPE *)0)：将地址0转换成结构体TYPE类型，因为转换成相应类型后就可以按照它的类型寻                       址访问成员规则来访问它的成员了,(只是取成员的地址,并没有取成员的值，若                       取值则会发生错误，因为起始地址为0，所以此时取到的成员并非原来的地址，而                      我们需要的也只是地址而已，所以不会发生错误)
         ((TYPE *)0)->MEMBER:取出转换类型后的结构体的成员MEMBER
        &((TYPE *)0)->MEMBER:取成员MEMBER的地址         
*/

#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})
/*
@功能：求出type类型结构体的首地址
@算法原理：typeof( ((type *)0)->member )：取得结构体member成员的类型
         const typeof( ((type *)0)->member ) *__mptr = (ptr)：定义成员member类型的          指针变量并将传入member成员地址赋值给它
         (char *)__mptr - offsetof(type,member)：member成员的地址减去它在结构体内的地          址偏移量，得出结构体首地址
         (type *)( (char *)__mptr - offsetof(type,member) )：将计算得出的结果体首地          址强制转换成结构体原本的地址类型type,恢复它的访问内存成员规则，保证正常访问数据成员
*/

#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)
/*
@功能：已知结构体任意一个成员名member和它的地址ptr，计算结构体首地址
*/

/**
 * list_for_each	-	iterate over a list
 * @pos:	the &struct list_head to use as a loop cursor.
 * @head:	the head for your list.
 */
#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_all(pos, next, head)\
	for (pos = (head)->next, next = pos->next; pos != (head);\
			pos = next, next = next->next)

#endif

