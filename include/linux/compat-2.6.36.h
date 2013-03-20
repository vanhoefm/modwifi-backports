#ifndef LINUX_26_36_COMPAT_H
#define LINUX_26_36_COMPAT_H

#include <linux/version.h>

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36))

#include <linux/usb.h>
#include <pcmcia/cistpl.h>
#include <pcmcia/ds.h>
#include <linux/pm_qos_params.h>
#include <linux/smp_lock.h>

#define kparam_block_sysfs_write(a)
#define kparam_unblock_sysfs_write(a)

/* mask va_format as RHEL6 backports this */
#define va_format compat_va_format

struct va_format {
	const char *fmt;
	va_list *va;
};

#define device_rename(dev, new_name) device_rename(dev, (char *)new_name)

#ifdef CONFIG_COMPAT_USB_URB_THREAD_FIX
#define usb_scuttle_anchored_urbs LINUX_BACKPORT(usb_scuttle_anchored_urbs)
#define usb_get_from_anchor LINUX_BACKPORT(usb_get_from_anchor)
#define usb_unlink_anchored_urbs LINUX_BACKPORT(usb_unlink_anchored_urbs)

extern void usb_unlink_anchored_urbs(struct usb_anchor *anchor);
extern struct urb *usb_get_from_anchor(struct usb_anchor *anchor);
extern void usb_scuttle_anchored_urbs(struct usb_anchor *anchor);
#endif

/**
 * pcmcia_read_config_byte() - read a byte from a card configuration register
 *
 * pcmcia_read_config_byte() reads a byte from a configuration register in
 * attribute memory.
 */
static inline int pcmcia_read_config_byte(struct pcmcia_device *p_dev, off_t where, u8 *val)
{
        int ret;
        conf_reg_t reg = { 0, CS_READ, where, 0 };
        ret = pcmcia_access_configuration_register(p_dev, &reg);
        *val = reg.Value;
        return ret;
}

/**
 * pcmcia_write_config_byte() - write a byte to a card configuration register
 *
 * pcmcia_write_config_byte() writes a byte to a configuration register in
 * attribute memory.
 */
static inline int pcmcia_write_config_byte(struct pcmcia_device *p_dev, off_t where, u8 val)
{
	conf_reg_t reg = { 0, CS_WRITE, where, val };
	return pcmcia_access_configuration_register(p_dev, &reg);
}

struct pm_qos_request_list {
	u32 qos;
	void *request;
};

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))

#define pm_qos_add_request(_req, _class, _value) do {			\
	(_req)->request = #_req;					\
	(_req)->qos = _class;						\
	pm_qos_add_requirement((_class), (_req)->request, (_value));	\
    } while(0)

#define pm_qos_update_request(_req, _value)				\
	pm_qos_update_requirement((_req)->qos, (_req)->request, (_value))

#define pm_qos_remove_request(_req)					\
	pm_qos_remove_requirement((_req)->qos, (_req)->request)

#else

#define pm_qos_add_request(_req, _class, _value) do {			\
	(_req)->request = pm_qos_add_request((_class), (_value));	\
    } while (0)

#define pm_qos_update_request(_req, _value)				\
	pm_qos_update_request((_req)->request, (_value))

#define pm_qos_remove_request(_req)					\
	pm_qos_remove_request((_req)->request)

#endif

/*
 * Dummy printk for disabled debugging statements to use whilst maintaining
 * gcc's format and side-effect checking.
 */
/* mask no_printk as RHEL6 backports this */
#define no_printk(a, ...) compat_no_printk(a, ##__VA_ARGS__)
static inline __attribute__ ((format (printf, 1, 2)))
int no_printk(const char *s, ...) { return 0; }

#ifndef alloc_workqueue
#define alloc_workqueue(name, flags, max_active) __create_workqueue(name, flags, max_active, 0)
#endif

#define EXTPROC	0200000
#define TIOCPKT_IOCTL		64

static inline void tty_lock(void) __acquires(kernel_lock)
{
#ifdef CONFIG_LOCK_KERNEL
	/* kernel_locked is 1 for !CONFIG_LOCK_KERNEL */
	WARN_ON(kernel_locked());
#endif
	lock_kernel();
}
static inline void tty_unlock(void) __releases(kernel_lock)
{
	unlock_kernel();
}
#define tty_locked()           (kernel_locked())

#define usleep_range(_min, _max)	msleep((_max) / 1000)

#define __rcu

static inline void pm_wakeup_event(struct device *dev, unsigned int msec) {}

static inline bool skb_defer_rx_timestamp(struct sk_buff *skb)
{
	return false;
}

static inline void skb_tx_timestamp(struct sk_buff *skb)
{
}

/*
 * System-wide workqueues which are always present.
 *
 * system_wq is the one used by schedule[_delayed]_work[_on]().
 * Multi-CPU multi-threaded.  There are users which expect relatively
 * short queue flush time.  Don't queue works which can run for too
 * long.
 *
 * system_long_wq is similar to system_wq but may host long running
 * works.  Queue flushing might take relatively long.
 *
 * system_nrt_wq is non-reentrant and guarantees that any given work
 * item is never executed in parallel by multiple CPUs.  Queue
 * flushing might take relatively long.
 */
#define system_wq LINUX_BACKPORT(system_wq)
extern struct workqueue_struct *system_wq;
#define system_long_wq LINUX_BACKPORT(system_long_wq)
extern struct workqueue_struct *system_long_wq;
#define system_nrt_wq LINUX_BACKPORT(system_nrt_wq)
extern struct workqueue_struct *system_nrt_wq;

void compat_system_workqueue_create(void);
void compat_system_workqueue_destroy(void);

#define schedule_work LINUX_BACKPORT(schedule_work)
int schedule_work(struct work_struct *work);
#define schedule_work_on LINUX_BACKPORT(schedule_work_on)
int schedule_work_on(int cpu, struct work_struct *work);
#define compat_schedule_delayed_work LINUX_BACKPORT(compat_schedule_delayed_work)
int schedule_delayed_work(struct delayed_work *dwork,
			  unsigned long delay);
#define compat_schedule_delayed_work_on LINUX_BACKPORT(compat_schedule_delayed_work_on)
int schedule_delayed_work_on(int cpu,
			     struct delayed_work *dwork,
			     unsigned long delay);
#define flush_scheduled_work LINUX_BACKPORT(flush_scheduled_work)
void flush_scheduled_work(void);

enum {
	/* bit mask for work_busy() return values */
	WORK_BUSY_PENDING       = 1 << 0,
	WORK_BUSY_RUNNING       = 1 << 1,
};

#define work_busy LINUX_BACKPORT(work_busy)
extern unsigned int work_busy(struct work_struct *work);

#define br_port_exists(dev)	(dev->br_port)

#else

static inline void compat_system_workqueue_create(void)
{
}

static inline void compat_system_workqueue_destroy(void)
{
}

/*
 * This is not part of The 2.6.37 kernel yet but we
 * we use it to optimize the backport code we
 * need to implement. Instead of using ifdefs
 * to check what version of the check we use
 * we just replace all checks on current code
 * with this. I'll submit this upstream too, that
 * way all we'd have to do is to implement this
 * for older kernels, then we would not have to
 * edit the upstrema code for backport efforts.
 */
#define br_port_exists(dev)	(dev->priv_flags & IFF_BRIDGE_PORT)

#endif /* (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)) */

#endif /* LINUX_26_36_COMPAT_H */
