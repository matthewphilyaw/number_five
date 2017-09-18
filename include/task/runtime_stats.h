//
// Created by Tommy Philyaw on 9/15/17.
//

#ifndef N5_DISPLAY_H
#define N5_DISPLAY_H

#define RUNTIME_STATS_TIMER_INST              TIM21
#define RUNTIME_STATS_TIMER_ENABLE_CLOCK()    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM21)
#define RUNTIME_STATS_TIMER_IRQN              TIM21_IRQn
#define RUNTIME_STATS_TIMER_IRQHANDLER        TIM21_IRQHandler
#define RUNTIME_STATS_TIMER_FREQ              100000

void runtime_stats_create_task(void);

#endif //N5_DISPLAY_H
