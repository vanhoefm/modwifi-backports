#ifndef _COMPAT_LINUX_OF_H
#define _COMPAT_LINUX_OF_H 1

#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,34) || defined(CONFIG_OF)
#include_next <linux/of.h>
#else
struct device_node;
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
#ifdef CONFIG_OF
extern struct device_node *of_get_child_by_name(const struct device_node *node,
						const char *name);
#else
static inline struct device_node *of_get_child_by_name(
					const struct device_node *node,
					const char *name)
{
	return NULL;
}
#endif /* CONFIG_OF */
#endif /* LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0) */

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
#ifndef CONFIG_OF
static inline struct device_node *of_find_node_by_name(struct device_node *from,
	const char *name)
{
	return NULL;
}
#endif /* CONFIG_OF */
#endif /* LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0) */

#endif	/* _COMPAT_LINUX_OF_H */
