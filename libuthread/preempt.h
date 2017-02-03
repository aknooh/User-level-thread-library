#ifndef _PREEMPT_H
#define _PREEMPT_H

#include <stdbool.h>

#ifdef _UTHREAD_PRIVATE

/*
 * preempt_start - Start thread preemption
 */
void preempt_start(void);

/*
 * preempt_save - Save current preemption status and disable preemption
 * @level: Pointer to structure in which to save the current preemption status
 */
void preempt_save(sigset_t *level);

/*
 * preempt_restore - Restore saved preemption status
 * @level: Pointer to structure from which to restore the preemption status
 */
void preempt_restore(sigset_t *level);

/*
 * preempt_enable - Enable preemption
 */
void preempt_enable(void);

/*
 * preempt_disable - Disable preemption
 */
void preempt_disable(void);

/*
 * preempt_disabled - Check if preemption is disabled
 *
 * Return: true if preemption is currently disabled, false otherwise
 */
bool preempt_disabled(void);

#else
#error "Private header, can't be included from applications directly"
#endif

#endif /* _PREEMPT_H */
