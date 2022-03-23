void update_process_times(int user_tick)
{
        struct task_struct *p = current;   /* current running process */
        int cpu = smp_processor_id(), system = user_tick ^ 1;

        update_one_process(p, user_tick, system, cpu);
        if (p->pid) {
                if (--p->counter <= 0) {
                        p->counter = 0;
                        p->need_resched = 1;
                }
                if (p->nice > 0)
                        kstat.per_cpu_nice[cpu] += user_tick;
                else
                        kstat.per_cpu_user[cpu] += user_tick;
                kstat.per_cpu_system[cpu] += system;
        } else if (local_bh_count(cpu) || local_irq_count(cpu) > 1)
                kstat.per_cpu_system[cpu] += system;
}