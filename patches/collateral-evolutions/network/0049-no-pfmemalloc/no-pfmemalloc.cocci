@r1@
struct page *page;
expression E1;
@@
 return E1
+#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0)
 || page->pfmemalloc
+#endif /* if LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0) */
 ;
